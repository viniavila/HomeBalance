#include <HBUserInterface.h>
#include <QApplication>
#include <QTranslator>
#include <QLocale>
#include <QStringBuilder>
#include <QFileInfo>

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

    QFileInfo fi(":/Translations/" % QLocale().name() % ".qm");
    QTranslator translator;
    if (fi.exists()) {
        translator.load(fi.filePath());
        QApplication::installTranslator(&translator);
    }

    HBUserInterface w;
    w.show();
    return a.exec();
}
