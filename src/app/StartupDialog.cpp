#include "StartupDialog.hpp"

#include <QFileDialog>
#include <QFileInfo>
#include <QHBoxLayout>
#include <QIntValidator>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>

StartupDialog::StartupDialog(QWidget* parent)
    : QDialog(parent), m_pathEdit(new QLineEdit(this)),
      m_okButton(new QPushButton("Open", this))
{
    setWindowTitle("Open Target Binary");
    setModal(true);
    setFixedWidth(500);

    auto* title = new QLabel("Select a binary to disassemble", this);
    title->setStyleSheet("font-weight: bold;");

    auto* browseBtn = new QPushButton("Browse…", this);
    connect(browseBtn, &QPushButton::clicked, this, &StartupDialog::browse);

    connect(m_okButton, &QPushButton::clicked, this, &StartupDialog::accept);
    m_okButton->setEnabled(false);

    connect(m_pathEdit, &QLineEdit::textChanged, this,
            &StartupDialog::validate);

    auto* pathLayout = new QHBoxLayout();
    pathLayout->addWidget(m_pathEdit);
    pathLayout->addWidget(browseBtn);

    auto* buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();
    buttonLayout->addWidget(m_okButton);

    auto* layout = new QVBoxLayout(this);
    layout->addWidget(title);
    layout->addSpacing(8);
    layout->addLayout(pathLayout);
    layout->addSpacing(12);
    layout->addLayout(buttonLayout);

    auto* baseLabel = new QLabel("Base address (hex):", this);
    m_baseEdit = new QLineEdit(this);
    m_baseEdit->setPlaceholderText("400000"); // default x86-64 typical
    m_baseEdit->setText("400000");
    m_baseEdit->setValidator(new QRegularExpressionValidator(
        QRegularExpression("0[xX][0-9a-fA-F]+|[0-9a-fA-F]+"), m_baseEdit));

    auto* baseLayout = new QHBoxLayout();
    baseLayout->addWidget(baseLabel);
    baseLayout->addWidget(m_baseEdit);

    // Add to main layout
    layout->addLayout(baseLayout);
}

void StartupDialog::browse()
{
    QString path =
        QFileDialog::getOpenFileName(this, "Select Binary", {}, "Binaries (*)");

    if (!path.isEmpty())
        m_pathEdit->setText(path);
}

void StartupDialog::validate()
{
    QFileInfo fi(m_pathEdit->text());
    bool ok = fi.exists() && fi.isFile();

    // Validate base address
    bool addrOk = !m_baseEdit->text().isEmpty();
    m_okButton->setEnabled(ok && addrOk);
}

QString StartupDialog::selectedBinary() const { return m_pathEdit->text(); }

uint64_t StartupDialog::baseAddress() const
{
    bool ok = false;
    QString text = m_baseEdit->text();

    // Allow optional 0x prefix
    uint64_t addr = text.toULongLong(&ok, 16);
    if (!ok)
        return 0x400000; // fallback default
    return addr;
}
