#ifndef DIALOGINCOMECATEGORIES_H
#define DIALOGINCOMECATEGORIES_H

#include <QDialog>
#include <HBDataManager.h>

namespace Ui { class DialogIncomeCategories; }
class DialogIncomeCategoriesPrivate;

class DialogIncomeCategories : public QDialog {
    Q_OBJECT

public:
    explicit DialogIncomeCategories(HBDataManager * m, QWidget *parent = 0);
    ~DialogIncomeCategories();

signals:
    void dataModified() const;

private:
    Ui::DialogIncomeCategories * const ui;
    DialogIncomeCategoriesPrivate * const d;
};

#endif // DIALOGINCOMECATEGORIES_H
