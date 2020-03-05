#ifndef DIALOGACCOUNTSBALANCE_H
#define DIALOGACCOUNTSBALANCE_H

#include <QDialog>
#include <HBDataManager.h>

namespace Ui { class DialogAccountsBalance; }
class DialogAccountsBalancePrivate;

class DialogAccountsBalance : public QDialog {
    Q_OBJECT

public:
    explicit DialogAccountsBalance(HBDataManager * m, QWidget *parent = 0);
    virtual ~DialogAccountsBalance();
    virtual void done(int);

private:
    Ui::DialogAccountsBalance * const ui;
    DialogAccountsBalancePrivate * const d;
};

#endif // DIALOGACCOUNTSBALANCE_H
