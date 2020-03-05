#ifndef HBUSERINTERFACE_H
#define HBUSERINTERFACE_H

#include <QMainWindow>

namespace Ui { class HBUserInterface; }

class HBUserInterfacePrivate;

class HBUserInterface : public QMainWindow {
    Q_OBJECT

public:
    explicit HBUserInterface(QWidget *parent = 0);
    ~HBUserInterface();

protected:
    void closeEvent(QCloseEvent *);

private:
    Ui::HBUserInterface * const ui;
    HBUserInterfacePrivate * const d;
};

#endif // HBUSERINTERFACE_H
