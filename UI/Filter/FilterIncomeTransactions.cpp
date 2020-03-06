#include "FilterIncomeTransactions.h"
#include "ui_FilterIncomeTransactions.h"
#include <QLocale>

class FilterIncomeTransactionsPrivate {
public:
    FilterIncomeTransactions * const parent;
    Ui::FilterIncomeTransactions * const ui;

    FilterIncomeTransactionsPrivate(FilterIncomeTransactions * p, Ui::FilterIncomeTransactions * ui) :
        parent(p),
        ui(ui)
    {
    }

    void btnReset_onClicked() {
        ui->chkAccount->setChecked(false);
        ui->chkBeginDate->setChecked(false);
        ui->chkBeginValue->setChecked(false);
        ui->chkCategory->setChecked(false);
        ui->chkDescripton->setChecked(false);
        ui->chkEndDate->setChecked(false);
        ui->chkEndValue->setChecked(false);
        ui->txtAccount->setText(QString());
        ui->txtBeginDate->setDate(QDate::currentDate());
        ui->txtBeginValue->setText(QString());
        ui->txtCategory->setText(QString());
        ui->txtDescription->setText(QString());
        ui->txtEndDate->setDate(QDate::currentDate());
        ui->txtEndValue->setText(QString());
    }
};

FilterIncomeTransactions::FilterIncomeTransactions(QWidget *parent) :
    AbstractFilterDialog(parent),
    ui(new Ui::FilterIncomeTransactions),
    d(new FilterIncomeTransactionsPrivate(this, ui))
{
    ui->setupUi(this);
    d->btnReset_onClicked();
    connect(ui->btnReset, &QAbstractButton::clicked, this, std::bind(&FilterIncomeTransactionsPrivate::btnReset_onClicked, d));
}

FilterIncomeTransactions::~FilterIncomeTransactions() {
    delete d;
    delete ui;
}

void FilterIncomeTransactions::setFilterList(const HBDataModelFilterList& filter_list) {
    QLocale l;
    d->btnReset_onClicked();
    for (const HBDataModelFilter& filter : filter_list) {
        if (filter.column == 3 && filter.type == HBDataModelFilter::FILTER_GE_DATE) {
            ui->chkBeginDate->setChecked(true);
            ui->txtBeginDate->setDate(filter.value.toDate());
        }
        if (filter.column == 3 && filter.type == HBDataModelFilter::FILTER_LE_DATE) {
            ui->chkEndDate->setChecked(true);
            ui->txtEndDate->setDate(filter.value.toDate());
        }
        if (filter.column == 4 && filter.type == HBDataModelFilter::FILTER_TEXT) {
            ui->chkAccount->setChecked(true);
            ui->txtAccount->setText(filter.value.toString());
        }
        if (filter.column == 5 && filter.type == HBDataModelFilter::FILTER_TEXT) {
            ui->chkCategory->setChecked(true);
            ui->txtCategory->setText(filter.value.toString());
        }
        if (filter.column == 6 && filter.type == HBDataModelFilter::FILTER_TEXT) {
            ui->chkDescripton->setChecked(true);
            ui->txtDescription->setText(filter.value.toString());
        }
        if (filter.column == 7 && filter.type == HBDataModelFilter::FILTER_GE_FLOAT) {
            ui->chkBeginValue->setChecked(true);
            ui->txtBeginValue->setText(l.toString(filter.value.toFloat(), 'f', 2));
        }
        if (filter.column == 7 && filter.type == HBDataModelFilter::FILTER_LE_FLOAT) {
            ui->chkEndValue->setChecked(true);
            ui->txtEndValue->setText(l.toString(filter.value.toFloat(), 'f', 2));
        }
    }
}

HBDataModelFilterList FilterIncomeTransactions::filterList() const {
    HBDataModelFilterList list;
    QLocale l;
    if (ui->chkBeginDate->isChecked())
        list << HBDataModelFilter(3, HBDataModelFilter::FILTER_GE_DATE, ui->txtBeginDate->date());
    if (ui->chkEndDate->isChecked())
        list << HBDataModelFilter(3, HBDataModelFilter::FILTER_LE_DATE, ui->txtEndDate->date());
    if (ui->chkAccount->isChecked())
        list << HBDataModelFilter(4, HBDataModelFilter::FILTER_TEXT, ui->txtAccount->text());
    if (ui->chkCategory->isChecked())
        list << HBDataModelFilter(5, HBDataModelFilter::FILTER_TEXT, ui->txtCategory->text());
    if (ui->chkDescripton->isChecked())
        list << HBDataModelFilter(6, HBDataModelFilter::FILTER_TEXT, ui->txtDescription->text());
    if (ui->chkBeginValue->isChecked())
        list << HBDataModelFilter(7, HBDataModelFilter::FILTER_GE_FLOAT, l.toFloat(ui->txtBeginValue->text()));
    if (ui->chkEndValue->isChecked())
        list << HBDataModelFilter(7, HBDataModelFilter::FILTER_LE_FLOAT, l.toFloat(ui->txtEndValue->text()));
    return list;
}

