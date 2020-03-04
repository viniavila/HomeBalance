#include <HBUserInterface.h>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QApplication::setApplicationDisplayName("");
    QApplication::setApplicationName("HomeBalance");
    QApplication::setApplicationVersion("1.0");
    QApplication::setDesktopFileName("Home Balance");
    QApplication::setOrganizationDomain("com.jsoft");
    QApplication::setOrganizationName("JSoft");
    //QApplication::setWindowIcon();
    HBUserInterface w;
    w.show();
    return a.exec();
}
