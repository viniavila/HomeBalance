#ifndef FILTEREXPENSETRANSACTIONS_H
#define FILTEREXPENSETRANSACTIONS_H

#include "AbstractFilterDialog.h"
#include <HBDataModel.h>

namespace Ui { class FilterExpenseTransactions; }
class FilterExpenseTransactionsPrivate;

class FilterExpenseTransactions : public AbstractFilterDialog {
    Q_OBJECT

public:
    FilterExpenseTransactions(QWidget *parent = nullptr);
    virtual ~FilterExpenseTransactions();

    virtual void setFilterList(const HBDataModelFilterList& filter_list);
    virtual HBDataModelFilterList filterList() const;

private:
    Ui::FilterExpenseTransactions * const ui;
    FilterExpenseTransactionsPrivate * const d;
};

#endif // FILTEREXPENSETRANSACTIONS_H
