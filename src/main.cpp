#include "app/MainWindow.hpp"
#include "app/StartupDialog.hpp"

#include <QApplication>

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    StartupDialog startup;
    if (startup.exec() != QDialog::Accepted)
        return 0;

    MainWindow w(startup.selectedBinary(), startup.baseAddress());
    w.resize(1000, 700);
    w.show();

    return app.exec();
}
