#include "InputExpenseTransaction.h"
#include "ui_InputExpenseTransaction.h"
#include <DialogAccounts.h>
#include <DialogExpenseCategories.h>
#include <QCompleter>

class InputExpenseTransactionPrivate {
public:
    InputExpenseTransaction * const parent;
    Ui::InputExpenseTransaction * const ui;
    HBDataManager * m_data;

    InputExpenseTransactionPrivate(InputExpenseTransaction * p, Ui::InputExpenseTransaction * ui) :
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

    void load_groups() {
        ui->cboGroup->clear();
        ui->cboCategory->clear();
        QStringList group_list = m_data->fileInterface()->getValueList(HBCOLLECTION_EXPENSE_CATEGORIES, "group", true);
        ui->cboGroup->addItems(group_list);
        ui->cboGroup->setCurrentIndex(-1);
    }

    void cboGroup_onCurrentIndexChanged(const QString& text) {
        ui->cboCategory->clear();
        ExpenseCategoryList category_list = m_data->fileInterface()->getObjectList<ExpenseCategory>("group", text);
        QHash<QString, QString> cat_names;
        for (const ExpenseCategory& c : category_list)
            cat_names[c.name] = c.id;
        QStringList names = cat_names.keys();
        std::sort(names.begin(), names.end(), [](const QString& s1, const QString& s2){ return QString::localeAwareCompare(s1, s2) < 0; });
        for (const QString& name : names)
            ui->cboCategory->addItem(name, cat_names[name]);
        parent->adjustSize();
    }

    void completer_onHighlighted(const QString& text) {
        ExpenseTransaction first = m_data->fileInterface()->getObject<ExpenseTransaction>("description", text);
        if (!first.account_id.isEmpty()) {
            Account acc = m_data->fileInterface()->getObject<Account>(first.account_id);
            ui->cboAccount->setCurrentText(acc.name);
        }
        if (!first.category_id.isEmpty()) {
            ExpenseCategory cat = m_data->fileInterface()->getObject<ExpenseCategory>(first.category_id);
            ui->cboGroup->setCurrentText(cat.group);
            ui->cboCategory->setCurrentText(cat.name);
        }
    }

    void btnAccounts_onClicked() {
        DialogAccounts dlg(m_data, parent);
        QObject::connect(&dlg, &DialogAccounts::dataModified, parent, std::bind(&InputExpenseTransactionPrivate::load_accounts, this));
        dlg.exec();
    }

    void btnCategories_onClicked() {
        DialogExpenseCategories dlg(m_data, parent);
        QObject::connect(&dlg, &DialogExpenseCategories::dataModified, parent, std::bind(&InputExpenseTransactionPrivate::load_groups, this));
        dlg.exec();
    }
};

InputExpenseTransaction::InputExpenseTransaction(HBDataManager * m, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::InputExpenseTransaction),
    d(new InputExpenseTransactionPrivate(this, ui))
{
    ui->setupUi(this);
    ui->txtValue->setValidator(new QDoubleValidator(0, 1e6, 2, this));
    ui->txtDate->setDate(QDate::currentDate());
    d->m_data = m;
    d->load_accounts();
    d->load_groups();

    QCompleter * completer = new QCompleter(m->fileInterface()->getValueList(HBCOLLECTION_EXPENSE_TRANSACTIONS, "description", true), this);
    completer->setCompletionMode(QCompleter::InlineCompletion);
    ui->txtDescription->setCompleter(completer);

    connect(completer, static_cast<void(QCompleter::*)(const QString&)>(&QCompleter::highlighted), this, std::bind(&InputExpenseTransactionPrivate::completer_onHighlighted, d, std::placeholders::_1));
    connect(ui->btnAccounts, &QAbstractButton::clicked, this, std::bind(&InputExpenseTransactionPrivate::btnAccounts_onClicked, d));
    connect(ui->btnCategories, &QAbstractButton::clicked, this, std::bind(&InputExpenseTransactionPrivate::btnCategories_onClicked, d));
    connect(ui->cboGroup, static_cast<void(QComboBox::*)(const QString&)>(&QComboBox::currentIndexChanged), this, std::bind(&InputExpenseTransactionPrivate::cboGroup_onCurrentIndexChanged, d, std::placeholders::_1));
}

InputExpenseTransaction::~InputExpenseTransaction() {
    delete d;
    delete ui;
}

void InputExpenseTransaction::setData(const ExpenseTransaction& data) {
    Account acc = d->m_data->fileInterface()->getObject<Account>(data.account_id);
    ExpenseCategory cat = d->m_data->fileInterface()->getObject<ExpenseCategory>(data.category_id);
    ui->txtDescription->setText(data.description);
    ui->cboAccount->setCurrentText(acc.name);
    ui->cboGroup->setCurrentText(cat.group);
    ui->cboCategory->setCurrentText(cat.name);
    ui->txtDate->setDate(data.transaction_date);
    ui->txtValue->setText(QLocale().toString(data.value, 'f', 2));
}

void InputExpenseTransaction::getData(ExpenseTransaction& data) const {
    data.description = ui->txtDescription->text();
    data.account_id = ui->cboAccount->currentData().toString();
    data.category_id = ui->cboCategory->currentData().toString();
    data.transaction_date = ui->txtDate->date();
    data.value = QLocale().toFloat(ui->txtValue->text());
}

ExpenseTransaction InputExpenseTransaction::getData() const {
    ExpenseTransaction t;
    getData(t);
    return t;
}
