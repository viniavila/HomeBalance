#ifndef INPUTTRANSFERTRANSACTION_H
#define INPUTTRANSFERTRANSACTION_H

#include <QDialog>
#include <HBDataManager.h>

namespace Ui { class InputTransferTransaction; }
class InputTransferTransactionPrivate;

class InputTransferTransaction : public QDialog {
    Q_OBJECT

public:
    explicit InputTransferTransaction(HBDataManager * m_data, QWidget *parent = 0);
    ~InputTransferTransaction();

    void setData(const TransferTransaction& d);
    void getData(TransferTransaction& d) const;
    TransferTransaction getData() const;

private:
    Ui::InputTransferTransaction * const ui;
    InputTransferTransactionPrivate * const d;
};

#endif // INPUTTRANSFERTRANSACTION_H
