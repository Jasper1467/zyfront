#pragma once

#include "DisasmTypes.hpp"

#include <QAbstractTableModel>
#include <cstdint>
#include <string>
#include <vector>

class DisasmModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit DisasmModel(QObject* parent = nullptr);

    void disassembleFile(const QString& path, uint64_t baseAddress = 0x400000);

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;

    QVariant data(const QModelIndex& index, int role) const override;

    QVariant headerData(int section, Qt::Orientation orientation,
                        int role) const override;

    Qt::ItemFlags flags(const QModelIndex& index) const override;

    bool setData(const QModelIndex& index, const QVariant& value, int role) override;

    const DisasmLine& lineAt(int row) const;
    void setComment(int row, const QString& comment);

private:
    std::vector<DisasmLine> m_lines;
};
