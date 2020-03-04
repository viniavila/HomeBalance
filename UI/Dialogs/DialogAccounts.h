#ifndef DIALOGACCOUNTS_H
#define DIALOGACCOUNTS_H

#include <QDialog>
#include <HBDataManager.h>

namespace Ui { class DialogAccounts; }
class DialogAccountsPrivate;

class DialogAccounts : public QDialog {
    Q_OBJECT

public:
    explicit DialogAccounts(HBDataManager * m, QWidget *parent = 0);
    ~DialogAccounts();

signals:
    void dataModified() const;

private:
    Ui::DialogAccounts * const ui;
    DialogAccountsPrivate * const d;
};

#endif // DIALOGACCOUNTS_H
