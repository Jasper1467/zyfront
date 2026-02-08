#pragma once

#include "DisasmTypes.hpp"

#include <QWidget>

class QTableView;
class DisasmModel;

class DisasmView : public QWidget
{
    Q_OBJECT
public:
    explicit DisasmView(QWidget* parent = nullptr);

    void loadBinary(const QString& path, uint64_t baseAddress = 0x400000);

    // Returns true if the address was found and scrolled to
    bool gotoAddress(uint64_t address, bool nearest);

private slots:
    void onContextMenu(const QPoint& pos);

signals:
    void instructionSelected(const DisasmLine& line);

private:
    QTableView* m_view;
    DisasmModel* m_model;
};
