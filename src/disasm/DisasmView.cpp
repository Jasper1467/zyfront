#include "DisasmView.hpp"
#include "DisasmModel.hpp"

#include <QAbstractItemView>
#include <QApplication>
#include <QClipboard>
#include <QHeaderView>
#include <QInputDialog>
#include <QMenu>
#include <QTableView>
#include <QVBoxLayout>

DisasmView::DisasmView(QWidget* parent)
    : QWidget(parent), m_view(new QTableView(this)),
      m_model(new DisasmModel(this))
{
    m_view->setModel(m_model);
    m_view->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_view->setSelectionMode(QAbstractItemView::SingleSelection);
    m_view->horizontalHeader()->setStretchLastSection(true);
    m_view->verticalHeader()->setVisible(false);
    m_view->setAlternatingRowColors(true);
    m_view->setShowGrid(false);
    m_view->setEditTriggers(QAbstractItemView::DoubleClicked |
                            QAbstractItemView::SelectedClicked);

    m_view->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(m_view, &QWidget::customContextMenuRequested, this,
            &DisasmView::onContextMenu);

    connect(m_view->selectionModel(), &QItemSelectionModel::currentRowChanged,
            this,
            [this](const QModelIndex& current, const QModelIndex&)
            {
                if (!current.isValid())
                    return;

                const DisasmLine& line = m_model->lineAt(current.row());
                emit instructionSelected(line);
            });

    auto* layout = new QVBoxLayout(this);
    layout->addWidget(m_view);
    layout->setContentsMargins(0, 0, 0, 0);
}

void DisasmView::loadBinary(const QString& path, uint64_t baseAddress)
{
    m_model->disassembleFile(path, baseAddress);
}

bool DisasmView::gotoAddress(uint64_t address, bool nearest)
{
    int row = -1;
    int rowCount = m_model->rowCount();
    uint64_t closestAddr = 0;
    int closestRow = -1;
    uint64_t minDiff = UINT64_MAX;

    for (int i = 0; i < rowCount; ++i)
    {
        QModelIndex idx = m_model->index(i, 0);
        bool ok = false;
        uint64_t rowAddr =
            m_model->data(idx, Qt::DisplayRole).toString().toULongLong(&ok, 16);
        if (!ok)
            continue;

        if (rowAddr == address)
        {
            row = i;
            break;
        }

        if (nearest)
        {
            uint64_t diff =
                (rowAddr > address) ? rowAddr - address : address - rowAddr;
            if (diff < minDiff)
            {
                minDiff = diff;
                closestAddr = rowAddr;
                closestRow = i;
            }
        }
    }

    if (row == -1 && nearest && closestRow != -1)
        row = closestRow;

    if (row != -1)
    {
        QModelIndex idx = m_model->index(row, 0);
        m_view->scrollTo(idx, QAbstractItemView::PositionAtCenter);
        m_view->setCurrentIndex(idx);
        m_view->setFocus();
        return true;
    }

    return false; // not found
}

void DisasmView::onContextMenu(const QPoint& pos)
{
    QModelIndex index = m_view->indexAt(pos);
    if (!index.isValid())
        return;

    int row = index.row();

    QMenu menu(this);

    QAction* copyAddress = menu.addAction("Copy Address");
    QAction* copyInstruction = menu.addAction("Copy Instruction");
    menu.addSeparator();
    QAction* addComment = menu.addAction("Add / Edit Comment");
    menu.addSeparator();
    QAction* gotoAddr = menu.addAction("Goto Address");

    QAction* selected = menu.exec(m_view->viewport()->mapToGlobal(pos));

    if (!selected)
        return;

    const auto& line = m_model->lineAt(row); // we'll add this accessor

    if (selected == copyAddress)
    {
        QApplication::clipboard()->setText(
            QString("0x%1").arg(line.address, 0, 16));
    }
    else if (selected == copyInstruction)
    {
        QApplication::clipboard()->setText(
            QString::fromStdString(line.instruction));
    }
    else if (selected == addComment)
    {
        bool ok = false;
        QString text = QInputDialog::getText(
            this, "Comment", "Enter comment:", QLineEdit::Normal,
            QString::fromStdString(line.comment), &ok);

        if (ok)
            m_model->setComment(row, text);
    }
    else if (selected == gotoAddr)
    {
        gotoAddress(line.address, false);
    }
}
