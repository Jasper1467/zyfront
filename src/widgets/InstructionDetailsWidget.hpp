#pragma once

#include "../disasm/DisasmModel.hpp"
#include <QWidget>

class QLabel;
class QTextEdit;
class QFormLayout;

class InstructionDetailsWidget : public QWidget
{
    Q_OBJECT
public:
    explicit InstructionDetailsWidget(QWidget* parent = nullptr);

public slots:
    void setInstruction(const DisasmLine& line);

private:
    QLabel* m_address;
    QLabel* m_bytes;
    QLabel* m_text;

    QTextEdit* m_comment;
    QTextEdit* m_decoderDetails; // future Zydis data
};
