#include "GotoDialog.hpp"

#include <QCheckBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QRegularExpressionValidator>
#include <QVBoxLayout>

GotoDialog::GotoDialog(QWidget* parent) : QDialog(parent)
{
    setWindowTitle("Goto Address");
    setModal(true);

    auto* layout = new QVBoxLayout(this);

    // Address input
    auto* addrLayout = new QHBoxLayout();
    addrLayout->addWidget(new QLabel("Address (hex):", this));
    m_addressEdit = new QLineEdit(this);
    m_addressEdit->setPlaceholderText("400000");
    m_addressEdit->setValidator(new QRegularExpressionValidator(
        QRegularExpression("0[xX][0-9a-fA-F]+|[0-9a-fA-F]+"), this));
    addrLayout->addWidget(m_addressEdit);
    layout->addLayout(addrLayout);

    // Nearest checkbox
    m_nearestCheck =
        new QCheckBox("Go to nearest instruction if not exact", this);
    layout->addWidget(m_nearestCheck);

    // Buttons
    auto* buttonLayout = new QHBoxLayout();
    m_okButton = new QPushButton("OK", this);
    auto* cancelButton = new QPushButton("Cancel", this);
    buttonLayout->addWidget(m_okButton);
    buttonLayout->addWidget(cancelButton);
    layout->addLayout(buttonLayout);

    connect(m_okButton, &QPushButton::clicked, this, &QDialog::accept);
    connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);
}

uint64_t GotoDialog::address() const
{
    bool ok = false;
    return m_addressEdit->text().toULongLong(&ok, 16);
}

bool GotoDialog::nearestEnabled() const { return m_nearestCheck->isChecked(); }
