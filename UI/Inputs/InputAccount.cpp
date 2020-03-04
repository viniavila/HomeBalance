#include "InputAccount.h"
#include "ui_InputAccount.h"
#include <QLocale>
#include <QDoubleValidator>

InputAccount::InputAccount(HBDataManager * d, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::InputAccount)
{
    ui->setupUi(this);
    ui->cboType->addItems(d->accountTypes());
    ui->txtBalance->setValidator(new QDoubleValidator(-1e6, 1e6, 2, this));
}

InputAccount::~InputAccount() {
    delete ui;
}

void InputAccount::setData(const Account& d) {
    ui->cboType->setCurrentIndex(d.type);
    ui->txtName->setText(d.name);
    ui->txtBalance->setText(QLocale().toString(d.initial_balance, 'f', 2));
}

void InputAccount::getData(Account& d) const {
    d.type = static_cast<Account::Type>(ui->cboType->currentIndex());
    d.name = ui->txtName->text();
    d.initial_balance = QLocale().toFloat(ui->txtBalance->text());
}

Account InputAccount::getData() const {
    Account d;
    getData(d);
    return d;
}
