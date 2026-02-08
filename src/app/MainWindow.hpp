#pragma once
#include <QMainWindow>

#include <memory>

#include "../console/CommandDispatcher.hpp"

class DisasmView;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(const QString& binaryPath,
                        uint64_t baseAddress = 0x400000,
                        QWidget* parent = nullptr);

private:
    DisasmView* m_disasmView;
    QString m_binaryPath;

    std::unique_ptr<CommandDispatcher> m_dispatcher;
};
