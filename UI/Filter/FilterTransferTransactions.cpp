#include "FilterTransferTransactions.h"
#include "ui_FilterTransferTransactions.h"
#include <QLocale>

class FilterTransferTransactionsPrivate {
public:
    FilterTransferTransactions * const parent;
    Ui::FilterTransferTransactions * const ui;

    FilterTransferTransactionsPrivate(FilterTransferTransactions * p, Ui::FilterTransferTransactions * ui) :
        parent(p),
        ui(ui)
    {

    }

    void btnReset_onClicked() {
        ui->chkBeginDate->setChecked(false);
        ui->chkBeginValue->setChecked(false);
        ui->chkDestination->setChecked(false);
        ui->chkEndDate->setChecked(false);
        ui->chkEndValue->setChecked(false);
        ui->chkOrigin->setChecked(false);
        ui->txtBeginDate->setDate(QDate::currentDate());
        ui->txtBeginValue->setText(QString());
        ui->txtDestination->setText(QString());
        ui->txtEndDate->setDate(QDate::currentDate());
        ui->txtEndValue->setText(QString());
        ui->txtOrigin->setText(QString());
    }
};

FilterTransferTransactions::FilterTransferTransactions(QWidget *parent) :
    AbstractFilterDialog(parent),
    ui(new Ui::FilterTransferTransactions),
    d(new FilterTransferTransactionsPrivate(this, ui))
{
    ui->setupUi(this);
    d->btnReset_onClicked();
    connect(ui->btnReset, &QAbstractButton::clicked, this, std::bind(&FilterTransferTransactionsPrivate::btnReset_onClicked, d));
}

FilterTransferTransactions::~FilterTransferTransactions() {
    delete d;
    delete ui;
}

void FilterTransferTransactions::setFilterList(const HBDataModelFilterList& filter_list) {
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
            ui->chkOrigin->setChecked(true);
            ui->txtOrigin->setText(filter.value.toString());
        }
        if (filter.column == 5 && filter.type == HBDataModelFilter::FILTER_TEXT) {
            ui->chkDestination->setChecked(true);
            ui->txtDestination->setText(filter.value.toString());
        }
        if (filter.column == 6 && filter.type == HBDataModelFilter::FILTER_GE_FLOAT) {
            ui->chkBeginValue->setChecked(true);
            ui->txtBeginValue->setText(l.toString(filter.value.toFloat(), 'f', 2));
        }
        if (filter.column == 6 && filter.type == HBDataModelFilter::FILTER_LE_FLOAT) {
            ui->chkEndValue->setChecked(true);
            ui->txtEndValue->setText(l.toString(filter.value.toFloat(), 'f', 2));
        }
    }
}

HBDataModelFilterList FilterTransferTransactions::filterList() const {
    HBDataModelFilterList list;
    QLocale l;
    if (ui->chkBeginDate->isChecked())
        list << HBDataModelFilter(3, HBDataModelFilter::FILTER_GE_DATE, ui->txtBeginDate->date());
    if (ui->chkEndDate->isChecked())
        list << HBDataModelFilter(3, HBDataModelFilter::FILTER_LE_DATE, ui->txtEndDate->date());
    if (ui->chkOrigin->isChecked())
        list << HBDataModelFilter(4, HBDataModelFilter::FILTER_TEXT, ui->txtOrigin->text());
    if (ui->chkDestination->isChecked())
        list << HBDataModelFilter(5, HBDataModelFilter::FILTER_TEXT, ui->txtDestination->text());
    if (ui->chkBeginValue->isChecked())
        list << HBDataModelFilter(6, HBDataModelFilter::FILTER_GE_FLOAT, l.toFloat(ui->txtBeginValue->text()));
    if (ui->chkEndValue->isChecked())
        list << HBDataModelFilter(6, HBDataModelFilter::FILTER_LE_FLOAT, l.toFloat(ui->txtEndValue->text()));
    return list;
}

