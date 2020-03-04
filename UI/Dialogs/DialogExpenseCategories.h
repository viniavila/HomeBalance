#ifndef DIALOGEXPENSECATEGORIES_H
#define DIALOGEXPENSECATEGORIES_H

#include <QDialog>
#include <HBDataManager.h>

namespace Ui { class DialogExpenseCategories; }
class DialogExpenseCategoriesPrivate;

class DialogExpenseCategories : public QDialog {
    Q_OBJECT

public:
    explicit DialogExpenseCategories(HBDataManager * m, QWidget *parent = 0);
    ~DialogExpenseCategories();

signals:
    void dataModified() const;

private:
    Ui::DialogExpenseCategories * const ui;
    DialogExpenseCategoriesPrivate * const d;
};

#endif // DIALOGEXPENSECATEGORIES_H
