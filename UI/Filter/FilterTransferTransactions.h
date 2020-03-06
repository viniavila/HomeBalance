#ifndef FILTERTRANSFERTRANSACTIONS_H
#define FILTERTRANSFERTRANSACTIONS_H

#include "AbstractFilterDialog.h"
#include <HBDataModel.h>

namespace Ui { class FilterTransferTransactions; }
class FilterTransferTransactionsPrivate;

class FilterTransferTransactions : public AbstractFilterDialog {
    Q_OBJECT

public:
    FilterTransferTransactions(QWidget *parent = nullptr);
    virtual ~FilterTransferTransactions();

    virtual void setFilterList(const HBDataModelFilterList& filter_list);
    virtual HBDataModelFilterList filterList() const;

private:
    Ui::FilterTransferTransactions * const ui;
    FilterTransferTransactionsPrivate * const d;
};

#endif // FILTERTRANSFERTRANSACTIONS_H
