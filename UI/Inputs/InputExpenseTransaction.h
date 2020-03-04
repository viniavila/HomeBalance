#ifndef INPUTEXPENSETRANSACTION_H
#define INPUTEXPENSETRANSACTION_H

#include <QDialog>
#include <HBDataManager.h>

namespace Ui { class InputExpenseTransaction; }
class InputExpenseTransactionPrivate;

class InputExpenseTransaction : public QDialog {
    Q_OBJECT

public:
    explicit InputExpenseTransaction(HBDataManager * m, QWidget *parent = 0);
    ~InputExpenseTransaction();

    void setData(const ExpenseTransaction& d);
    void getData(ExpenseTransaction& d) const;
    ExpenseTransaction getData() const;

private:
    Ui::InputExpenseTransaction * const ui;
    InputExpenseTransactionPrivate * const d;
};

#endif // INPUTEXPENSETRANSACTION_H
