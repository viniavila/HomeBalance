#ifndef INPUTEXPENSECATEGORY_H
#define INPUTEXPENSECATEGORY_H

#include <QDialog>
#include <HBDataManager.h>

namespace Ui { class InputExpenseCategory; }

class InputExpenseCategory : public QDialog {
    Q_OBJECT

public:
    explicit InputExpenseCategory(HBDataManager * m, QWidget *parent = 0);
    ~InputExpenseCategory();

    void setData(const ExpenseCategory& d);
    void getData(ExpenseCategory& d) const;
    ExpenseCategory getData() const;

private:
    Ui::InputExpenseCategory * const ui;
};

#endif // INPUTEXPENSECATEGORY_H
