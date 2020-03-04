#ifndef INPUTINCOMETRANSACTION_H
#define INPUTINCOMETRANSACTION_H

#include <QDialog>
#include <HBDataManager.h>

namespace Ui { class InputIncomeTransaction; }
class InputIncomeTransactionPrivate;

class InputIncomeTransaction : public QDialog {
    Q_OBJECT

public:
    explicit InputIncomeTransaction(HBDataManager * m, QWidget *parent = 0);
    ~InputIncomeTransaction();

    void setData(const IncomeTransaction& d);
    void getData(IncomeTransaction& d) const;
    IncomeTransaction getData() const;

private:
    Ui::InputIncomeTransaction * const ui;
    InputIncomeTransactionPrivate * const d;
};

#endif // INPUTINCOMETRANSACTION_H
