#ifndef INPUTACCOUNT_H
#define INPUTACCOUNT_H

#include <QDialog>
#include <HBDataManager.h>

namespace Ui { class InputAccount; }

class InputAccount : public QDialog {
    Q_OBJECT

public:
    explicit InputAccount(HBDataManager * d, QWidget *parent = 0);
    ~InputAccount();

    void setData(const Account& d);
    void getData(Account& d) const;
    Account getData() const;

private:
    Ui::InputAccount * const ui;
};

#endif // INPUTACCOUNT_H
