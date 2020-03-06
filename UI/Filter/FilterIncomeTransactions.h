#ifndef FILTERINCOMETRANSACTIONS_H
#define FILTERINCOMETRANSACTIONS_H

#include "AbstractFilterDialog.h"
#include <HBDataModel.h>

namespace Ui { class FilterIncomeTransactions; }
class FilterIncomeTransactionsPrivate;

class FilterIncomeTransactions : public AbstractFilterDialog {
    Q_OBJECT

public:
    FilterIncomeTransactions(QWidget *parent = nullptr);
    virtual ~FilterIncomeTransactions();

    virtual void setFilterList(const HBDataModelFilterList& filter_list);
    virtual HBDataModelFilterList filterList() const;

private:
    Ui::FilterIncomeTransactions * const ui;
    FilterIncomeTransactionsPrivate * const d;
};

#endif // FILTERINCOMETRANSACTIONS_H
