#ifndef INPUTINCOMECATEGORY_H
#define INPUTINCOMECATEGORY_H

#include <QDialog>
#include <HBDataManager.h>

namespace Ui { class InputIncomeCategory; }

class InputIncomeCategory : public QDialog {
    Q_OBJECT

public:
    explicit InputIncomeCategory(HBDataManager * m, QWidget *parent = 0);
    ~InputIncomeCategory();

    void setData(const IncomeCategory& d);
    void getData(IncomeCategory& d) const;
    IncomeCategory getData() const;

private:
    Ui::InputIncomeCategory * const ui;
};

#endif // INPUTINCOMECATEGORY_H
