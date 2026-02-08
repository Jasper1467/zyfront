#pragma once

#include <QDialog>
#include <QString>

class QLineEdit;
class QPushButton;

class StartupDialog : public QDialog
{
    Q_OBJECT
public:
    explicit StartupDialog(QWidget* parent = nullptr);

    QString selectedBinary() const;
    uint64_t baseAddress() const;

private slots:
    void browse();
    void validate();

private:
    QLineEdit* m_pathEdit;
    QLineEdit* m_baseEdit;
    QPushButton* m_okButton;
};
