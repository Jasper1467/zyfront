#include "InstructionDetailsWidget.hpp"

#include "../disasm/DisasmTypes.hpp"

#include <QFormLayout>
#include <QLabel>
#include <QTextEdit>
#include <QVBoxLayout>

namespace
{
    QString operandToString(const ZydisDecodedOperand& op)
    {
        switch (op.type)
        {
        case ZYDIS_OPERAND_TYPE_REGISTER:
            return QString("REGISTER: %1")
                .arg(ZydisRegisterGetString(op.reg.value));

        case ZYDIS_OPERAND_TYPE_IMMEDIATE:
            return QString("IMMEDIATE: 0x%1").arg(op.imm.value.u, 0, 16);

        case ZYDIS_OPERAND_TYPE_MEMORY:
        {
            QString s = "MEMORY: [";
            if (op.mem.base != ZYDIS_REGISTER_NONE)
                s += ZydisRegisterGetString(op.mem.base);

            if (op.mem.index != ZYDIS_REGISTER_NONE)
                s += QString(" + %1*%2")
                         .arg(ZydisRegisterGetString(op.mem.index))
                         .arg(op.mem.scale);

            if (op.mem.disp.size != 0)
                s += QString(" + 0x%1").arg(op.mem.disp.value, 0, 16);

            s += "]";
            return s;
        }

        default:
            return "UNKNOWN";
        }
    }
} // namespace

InstructionDetailsWidget::InstructionDetailsWidget(QWidget* parent)
    : QWidget(parent)
{
    auto* layout = new QVBoxLayout(this);
    auto* form = new QFormLayout();

    m_address = new QLabel("-");
    m_bytes = new QLabel("-");
    m_text = new QLabel("-");
    m_text->setWordWrap(true);

    m_comment = new QTextEdit();
    m_comment->setPlaceholderText("Comment...");
    m_comment->setMaximumHeight(80);

    m_decoderDetails = new QTextEdit();
    m_decoderDetails->setReadOnly(true);
    m_decoderDetails->setPlaceholderText("Zydis decoder details");

    form->addRow("Address:", m_address);
    form->addRow("Bytes:", m_bytes);
    form->addRow("Instruction:", m_text);

    layout->addLayout(form);
    layout->addWidget(m_comment);
    layout->addWidget(m_decoderDetails);
}

void InstructionDetailsWidget::setInstruction(const DisasmLine& line)
{
    m_address->setText(QString("0x%1").arg(line.address, 0, 16));
    m_bytes->setText(QString::fromStdString(line.bytes));
    m_text->setText(QString::fromStdString(line.instruction));
    m_comment->setPlainText(QString::fromStdString(line.comment));

    QString details;

    details += QString("Mnemonic: %1\n")
                   .arg(ZydisMnemonicGetString(line.decoded.mnemonic));

    details += QString("Length: %1 bytes\n").arg(line.decoded.length);

    details += QString("Operands: %1\n\n").arg(line.decoded.operand_count);

    for (size_t i = 0; i < line.operands.size(); ++i)
    {
        details +=
            QString("  %1: %2\n").arg(i).arg(operandToString(line.operands[i]));
    }

    details += "\nFlags:\n";
    if (line.decoded.cpu_flags)
    {
        details += QString("  Tested:   0x%1\n")
                       .arg(line.decoded.cpu_flags->tested, 0, 16);
        details += QString("  Modified: 0x%1\n")
                       .arg(line.decoded.cpu_flags->modified, 0, 16);
    }
    else
    {
        details += "  None\n";
    }

    m_decoderDetails->setPlainText(details);
}
