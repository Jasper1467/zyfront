#include "MainWindow.hpp"

#include "../dialogs/GotoDialog.hpp"
#include "../disasm/DisasmView.hpp"

#include "../widgets/ConsoleWidget.hpp"
#include "../widgets/InstructionDetailsWidget.hpp"

#include "../console/CommandContext.hpp"
#include "../console/CommandDispatcher.hpp"
#include "../console/commands/GotoCommand.hpp"

#include <QAction>
#include <QDockWidget>
#include <QKeySequence>
#include <QMessageBox>
#include <QToolBar>

MainWindow::MainWindow(const QString& binaryPath, uint64_t baseAddress,
                       QWidget* parent)
    : QMainWindow(parent)
{
    // Central: Disassembly view
    m_disasmView = new DisasmView(this);
    setCentralWidget(m_disasmView);

    setWindowTitle(QString("zyfront — %1").arg(binaryPath));
    m_disasmView->loadBinary(binaryPath, baseAddress);

    // Dock: Instruction Details
    auto* instructionDetailsWidget = new InstructionDetailsWidget(this);

    auto* instructionDetailsDock = new QDockWidget("Instruction Details", this);

    instructionDetailsDock->setWidget(instructionDetailsWidget);
    instructionDetailsDock->setAllowedAreas(Qt::LeftDockWidgetArea |
                                            Qt::RightDockWidgetArea);

    instructionDetailsDock->setFeatures(QDockWidget::DockWidgetMovable |
                                        QDockWidget::DockWidgetClosable |
                                        QDockWidget::DockWidgetFloatable);

    addDockWidget(Qt::RightDockWidgetArea, instructionDetailsDock);

    // Selection → details panel
    connect(m_disasmView, &DisasmView::instructionSelected,
            instructionDetailsWidget,
            &InstructionDetailsWidget::setInstruction);

    // Toolbar: Navigation
    auto* toolbar = addToolBar("Navigation");

    QAction* gotoAction = toolbar->addAction("Goto Address");
    gotoAction->setShortcut(QKeySequence("Ctrl+G"));

    connect(gotoAction, &QAction::triggered, this,
            [this]()
            {
                GotoDialog dlg(this);
                if (dlg.exec() == QDialog::Accepted)
                {
                    uint64_t addr = dlg.address();
                    bool nearest = dlg.nearestEnabled();
                    if (!m_disasmView->gotoAddress(addr, nearest))
                    {
                        QMessageBox::warning(
                            this, "Address Not Found",
                            QString("The address 0x%1 was not found in this "
                                    "disassembly.")
                                .arg(addr, 0, 16));
                    }
                }
            });

    // Dock: Console
    auto* consoleDock = new QDockWidget("Console", this);
    consoleDock->setAllowedAreas(Qt::BottomDockWidgetArea);

    auto* console = new ConsoleWidget(consoleDock);
    consoleDock->setWidget(console);

    addDockWidget(Qt::BottomDockWidgetArea, consoleDock);

    consoleDock->setFeatures(QDockWidget::DockWidgetMovable |
                             QDockWidget::DockWidgetClosable |
                             QDockWidget::DockWidgetFloatable);

    m_dispatcher = std::make_unique<CommandDispatcher>();
    m_dispatcher->registerCommand(std::make_unique<GotoCommand>());

    CommandContext ctx;
    ctx.mainWindow = this;
    ctx.disasmView = m_disasmView;

    connect(console, &ConsoleWidget::commandEntered, this,
            [this, ctx, console](const QString& line) mutable
            {
                QString error;
                QString output;

                if (!m_dispatcher->dispatch(line, ctx, error))
                {
                    console->appendError(error);
                }
            });
}
