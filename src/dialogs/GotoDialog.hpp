#pragma once
#include <QDialog>

class QLineEdit;
class QCheckBox;
class QPushButton;

class GotoDialog : public QDialog
{
    Q_OBJECT
public:
    explicit GotoDialog(QWidget* parent = nullptr);

    uint64_t address() const;
    bool nearestEnabled() const;

private:
    QLineEdit* m_addressEdit;
    QCheckBox* m_nearestCheck;
    QPushButton* m_okButton;
};
