#include "ConsoleWidget.hpp"

#include <QLineEdit>
#include <QTextEdit>
#include <QVBoxLayout>

ConsoleWidget::ConsoleWidget(QWidget* parent) : QWidget(parent)
{
    auto* layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);

    m_output = new QTextEdit(this);
    m_output->setReadOnly(true);
    m_output->setPlaceholderText("zyfront console");

    m_input = new QLineEdit(this);
    m_input->setPlaceholderText("Enter command...");

    layout->addWidget(m_output, 1);
    layout->addWidget(m_input, 0);

    connect(m_input, &QLineEdit::returnPressed, this,
            [this]()
            {
                const QString cmd = m_input->text().trimmed();
                if (cmd.isEmpty())
                    return;

                appendMessage(QString("> %1").arg(cmd));
                emit commandEntered(cmd);
                m_input->clear();
            });
}

void ConsoleWidget::appendMessage(const QString& text)
{
    m_output->append(text);
}

void ConsoleWidget::appendError(const QString& text)
{
    m_output->append(QString("<span style=\"color:red\">%1</span>").arg(text));
}
