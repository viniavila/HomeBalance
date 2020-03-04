#include "InputIncomeTransaction.h"
#include "ui_InputIncomeTransaction.h"
#include <DialogAccounts.h>
#include <DialogIncomeCategories.h>
#include <QCompleter>

class InputIncomeTransactionPrivate {
public:
    InputIncomeTransaction * const parent;
    Ui::InputIncomeTransaction * const ui;
    HBDataManager * m_data;

    InputIncomeTransactionPrivate(InputIncomeTransaction * p, Ui::InputIncomeTransaction * ui) :
        parent(p),
        ui(ui)
    {

    }

    void load_accounts() {
            ui->cboAccount->clear();
            AccountList account_list = m_data->fileInterface()->getObjectList<Account>();
            QHash<QString, QString> acc_names;
            for (const Account& c : account_list)
                acc_names[c.name] = c.id;
            QStringList names = acc_names.keys();
            std::sort(names.begin(), names.end(), [](const QString& s1, const QString& s2){ return QString::localeAwareCompare(s1, s2) < 0; });
            for (const QString& name : names)
                ui->cboAccount->addItem(name, acc_names[name]);
            ui->cboAccount->setCurrentIndex(-1);
    }

    void load_categories() {
        ui->cboCategory->clear();
        IncomeCategoryList category_list = m_data->fileInterface()->getObjectList<IncomeCategory>();
        QHash<QString, QString> cat_names;
        for (const IncomeCategory& c : category_list)
            cat_names[c.name] = c.id;
        QStringList names = cat_names.keys();
        std::sort(names.begin(), names.end(), [](const QString& s1, const QString& s2){ return QString::localeAwareCompare(s1, s2) < 0; });
        for (const QString& name : names)
            ui->cboCategory->addItem(name, cat_names[name]);
        ui->cboCategory->setCurrentIndex(-1);
    }

    void completer_onHighlighted(const QString& text) {
        IncomeTransaction first = m_data->fileInterface()->getObject<IncomeTransaction>("description", text);
        if (!first.account_id.isEmpty()) {
            Account acc = m_data->fileInterface()->getObject<Account>(first.account_id);
            ui->cboAccount->setCurrentText(acc.name);
        }
        if (!first.category_id.isEmpty()) {
            IncomeCategory cat = m_data->fileInterface()->getObject<IncomeCategory>(first.category_id);
            ui->cboCategory->setCurrentText(cat.name);
        }
    }

    void btnAccounts_onClicked() {
        DialogAccounts dlg(m_data, parent);
        QObject::connect(&dlg, &DialogAccounts::dataModified, parent, std::bind(&InputIncomeTransactionPrivate::load_accounts, this));
        dlg.exec();
    }

    void btnCategories_onClicked() {
        DialogIncomeCategories dlg(m_data, parent);
        QObject::connect(&dlg, &DialogIncomeCategories::dataModified, parent, std::bind(&InputIncomeTransactionPrivate::load_categories, this));
        dlg.exec();
    }

};

InputIncomeTransaction::InputIncomeTransaction(HBDataManager * m, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::InputIncomeTransaction),
    d(new InputIncomeTransactionPrivate(this, ui))
{
    ui->setupUi(this);
    ui->txtValue->setValidator(new QDoubleValidator(0, 1e6, 2, this));
    ui->txtDate->setDate(QDate::currentDate());
    d->m_data = m;
    d->load_accounts();
    d->load_categories();

    QCompleter * completer = new QCompleter(m->fileInterface()->getValueList(HBCOLLECTION_INCOME_TRANSACTIONS, "description", true), this);
    completer->setCompletionMode(QCompleter::InlineCompletion);
    ui->txtDescription->setCompleter(completer);

    connect(completer, static_cast<void(QCompleter::*)(const QString&)>(&QCompleter::highlighted), this, std::bind(&InputIncomeTransactionPrivate::completer_onHighlighted, d, std::placeholders::_1));
    connect(ui->btnAccounts, &QAbstractButton::clicked, this, std::bind(&InputIncomeTransactionPrivate::btnAccounts_onClicked, d));
    connect(ui->btnCategories, &QAbstractButton::clicked, this, std::bind(&InputIncomeTransactionPrivate::btnCategories_onClicked, d));
}

InputIncomeTransaction::~InputIncomeTransaction() {
    delete d;
    delete ui;
}

void InputIncomeTransaction::setData(const IncomeTransaction& data) {
    Account acc = d->m_data->fileInterface()->getObject<Account>(data.account_id);
    IncomeCategory cat = d->m_data->fileInterface()->getObject<IncomeCategory>(data.category_id);
    ui->txtDescription->setText(data.description);
    ui->cboAccount->setCurrentText(acc.name);
    ui->cboCategory->setCurrentText(cat.name);
    ui->txtDate->setDate(data.transaction_date);
    ui->txtValue->setText(QLocale().toString(data.value, 'f', 2));
}

void InputIncomeTransaction::getData(IncomeTransaction& data) const {
    data.description = ui->txtDescription->text();
    data.account_id = ui->cboAccount->currentData().toString();
    data.category_id = ui->cboCategory->currentData().toString();
    data.transaction_date = ui->txtDate->date();
    data.value = QLocale().toFloat(ui->txtValue->text());
}

IncomeTransaction InputIncomeTransaction::getData() const {
    IncomeTransaction t;
    getData(t);
    return t;
}
