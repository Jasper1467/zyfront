#pragma once

#include <QWidget>

class QTextEdit;
class QLineEdit;

class ConsoleWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ConsoleWidget(QWidget* parent = nullptr);

    void appendMessage(const QString& text);
    void appendError(const QString& text);

signals:
    void commandEntered(const QString& command);

private:
    QTextEdit* m_output;
    QLineEdit* m_input;
};
