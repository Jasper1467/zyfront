#include "DisasmModel.hpp"

#include <QByteArray>
#include <QFile>
#include <Zydis/Zydis.h>

DisasmModel::DisasmModel(QObject* parent) : QAbstractTableModel(parent) {}

void DisasmModel::disassembleFile(const QString& path, uint64_t baseAddress)
{
    beginResetModel();
    m_lines.clear();

    QFile file(path);
    if (!file.open(QIODevice::ReadOnly))
    {
        endResetModel();
        return;
    }

    QByteArray data = file.readAll();
    const uint8_t* code = reinterpret_cast<const uint8_t*>(data.constData());
    size_t size = static_cast<size_t>(data.size());

    ZydisDecoder decoder;
    ZydisDecoderInit(&decoder, ZYDIS_MACHINE_MODE_LONG_64,
                     ZYDIS_STACK_WIDTH_64);

    ZydisFormatter formatter;
    ZydisFormatterInit(&formatter, ZYDIS_FORMATTER_STYLE_INTEL);

    size_t offset = 0;
    uint64_t runtimeAddress = baseAddress;

    while (offset < size)
    {
        ZydisDecodedInstruction instr;
        ZydisDecodedOperand operands[ZYDIS_MAX_OPERAND_COUNT];

        if (!ZYAN_SUCCESS(ZydisDecoderDecodeFull(
                &decoder, code + offset, size - offset, &instr, operands)))
        {
            // Skip 1 byte on failure (important for robustness)
            offset += 1;
            runtimeAddress += 1;
            continue;
        }

        char text[256];
        ZydisFormatterFormatInstruction(&formatter, &instr, operands,
                                        instr.operand_count, text, sizeof(text),
                                        runtimeAddress, nullptr);

        // Format raw bytes
        std::string bytes;
        for (uint8_t i = 0; i < instr.length; ++i)
        {
            char buf[4];
            snprintf(buf, sizeof(buf), "%02X ", code[offset + i]);
            bytes += buf;
        }

        DisasmLine line;
        line.address = runtimeAddress;
        line.bytes = bytes;
        line.instruction = text;
        line.comment = "";

        line.decoded = instr;
        line.operands.assign(operands, operands + instr.operand_count);

        m_lines.push_back(line);

        offset += instr.length;
        runtimeAddress += instr.length;
    }

    endResetModel();
}

int DisasmModel::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);
    return static_cast<int>(m_lines.size());
}

int DisasmModel::columnCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);
    return 4; // Address | Bytes | Instruction | Comment
}

QVariant DisasmModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid())
        return {};

    const auto& line = m_lines[index.row()];

    if (role == Qt::DisplayRole || role == Qt::EditRole)
    {
        switch (index.column())
        {
        case 0:
            return QString("0x%1").arg(line.address, 0, 16);
        case 1:
            return QString::fromStdString(line.bytes);
        case 2:
            return QString::fromStdString(line.instruction);
        case 3:
            return QString::fromStdString(line.comment); // editable
        default:
            return {};
        }
    }

    return {};
}

QVariant DisasmModel::headerData(int section, Qt::Orientation orientation,
                                 int role) const
{
    if (orientation != Qt::Horizontal || role != Qt::DisplayRole)
        return {};

    switch (section)
    {
    case 0:
        return "Address";
    case 1:
        return "Bytes";
    case 2:
        return "Instruction";
    case 3:
        return "Comment";
    default:
        return {};
    }
}

Qt::ItemFlags DisasmModel::flags(const QModelIndex& index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    Qt::ItemFlags f = Qt::ItemIsSelectable | Qt::ItemIsEnabled;

    if (index.column() == 3) // Comment column editable
        f |= Qt::ItemIsEditable;

    return f;
}

bool DisasmModel::setData(const QModelIndex& index, const QVariant& value,
                          int role)
{
    if (!index.isValid() || role != Qt::EditRole)
        return false;

    if (index.column() == 3)
    {
        m_lines[index.row()].comment = value.toString().toStdString();
        emit dataChanged(index, index, { Qt::DisplayRole, Qt::EditRole });
        return true;
    }

    return false;
}

const DisasmLine& DisasmModel::lineAt(int row) const { return m_lines.at(row); }

void DisasmModel::setComment(int row, const QString& comment)
{
    if (row < 0 || row >= static_cast<int>(m_lines.size()))
        return;

    m_lines[row].comment = comment.toStdString();

    QModelIndex idx = index(row, 3); // comment column
    emit dataChanged(idx, idx);
}
