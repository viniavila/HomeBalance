#include "InputTransferTransaction.h"
#include "ui_InputTransferTransaction.h"
#include <DialogAccounts.h>

class InputTransferTransactionPrivate {
public:
    InputTransferTransaction * const parent;
    Ui::InputTransferTransaction * const ui;
    HBDataManager * m_data;

    InputTransferTransactionPrivate(InputTransferTransaction * p, Ui::InputTransferTransaction * ui) :
        parent(p),
        ui(ui)
    {

    }

    void load_accounts() {
        ui->cboOrigin->clear();
        ui->cboDestination->clear();
        AccountList account_list = m_data->getObjectList<Account>();
        QHash<QString, QString> acc_names;
        for (const Account& c : account_list)
            acc_names[c.name] = c.id;
        QStringList names = acc_names.keys();
        std::sort(names.begin(), names.end(), [](const QString& s1, const QString& s2){ return QString::localeAwareCompare(s1, s2) < 0; });
        for (const QString& name : names) {
            ui->cboOrigin->addItem(name, acc_names[name]);
            ui->cboDestination->addItem(name, acc_names[name]);
        }
        ui->cboOrigin->setCurrentIndex(-1);
        ui->cboDestination->setCurrentIndex(-1);
    }

    void btnAccounts_onClicked() {
        DialogAccounts dlg(m_data, parent);
        QObject::connect(&dlg, &DialogAccounts::dataModified, parent, std::bind(&InputTransferTransactionPrivate::load_accounts, this));
        dlg.exec();
    }

};

InputTransferTransaction::InputTransferTransaction(HBDataManager * m, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::InputTransferTransaction),
    d(new InputTransferTransactionPrivate(this, ui))
{
    ui->setupUi(this);
    ui->txtValue->setValidator(new QDoubleValidator(0, 1e6, 2, this));
    ui->txtDate->setDate(QDate::currentDate());
    d->m_data = m;
    d->load_accounts();

    connect(ui->btnAccounts, &QAbstractButton::clicked, this, std::bind(&InputTransferTransactionPrivate::btnAccounts_onClicked, d));
}

InputTransferTransaction::~InputTransferTransaction() {
    delete d;
    delete ui;
}

void InputTransferTransaction::setData(const TransferTransaction& data) {
    Account origin_acc = d->m_data->getObject<Account>(data.origin_account_id);
    Account dest_acc = d->m_data->getObject<Account>(data.destination_account_id);
    ui->cboOrigin->setCurrentText(origin_acc.name);
    ui->cboDestination->setCurrentText(dest_acc.name);
    ui->txtDate->setDate(data.transaction_date);
    ui->txtValue->setText(QLocale().toString(data.value, 'f', 2));
}

void InputTransferTransaction::getData(TransferTransaction& data) const {
    data.origin_account_id = ui->cboOrigin->currentData().toString();
    data.destination_account_id = ui->cboDestination->currentData().toString();
    data.transaction_date = ui->txtDate->date();
    data.value = QLocale().toFloat(ui->txtValue->text());
}

TransferTransaction InputTransferTransaction::getData() const {
    TransferTransaction t;
    getData(t);
    return t;
}
