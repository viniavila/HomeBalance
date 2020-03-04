#include "DialogIncomeCategories.h"
#include "ui_DialogIncomeCategories.h"
#include <InputIncomeCategory.h>
#include <HBGlobalFunctions.h>

class DialogIncomeCategoriesPrivate {
public:
    DialogIncomeCategories * const parent;
    Ui::DialogIncomeCategories * const ui;
    HBDataManager * m_data;

    DialogIncomeCategoriesPrivate(DialogIncomeCategories * p, Ui::DialogIncomeCategories * ui) :
        parent(p),
        ui(ui)
    {

    }

    void btnNew_onClicked() {
        InputIncomeCategory dlg(m_data, parent);
        if (dlg.exec()) {
            IncomeCategory d = dlg.getData();
            m_data->insertObject(d);
            emit parent->dataModified();
        }
    }

    void btnEdit_onClicked() {
        QModelIndex index = ui->tableView->currentIndex();
        tableView_onDoubleClicked(index);
    }


    void btnDelete_onClicked() {
        QModelIndex index = ui->tableView->currentIndex();
        if (index.isValid()) {
            if (HBGlobalFunctions::message_confirmation(parent)) {
                m_data->deleteObject<IncomeCategory>(index.row());
                emit parent->dataModified();
            }
        }
    }

    void tableView_onDoubleClicked(const QModelIndex& index) {
        if (index.isValid()) {
            IncomeCategory d = m_data->getObject<IncomeCategory>(index.row());
            InputIncomeCategory dlg(m_data, parent);
            dlg.setWindowTitle(parent->tr("Edit Income Category"));
            dlg.setData(d);
            if (dlg.exec()) {
                dlg.getData(d);
                m_data->updateObject(index.row(), d);
                emit parent->dataModified();
            }
        }
    }
};

DialogIncomeCategories::DialogIncomeCategories(HBDataManager * m, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogIncomeCategories),
    d(new DialogIncomeCategoriesPrivate(this, ui))
{
    ui->setupUi(this);
    d->m_data = m;
    m->setView(HBCOLLECTION_INCOME_CATEGORIES, ui->tableView, 3);
    ui->tableView->sortByColumn(3, Qt::AscendingOrder);
    ui->tableView->verticalHeader()->setDefaultSectionSize(fontMetrics().height()*1.5);

    connect(ui->btnNew, &QAbstractButton::clicked, this, std::bind(&DialogIncomeCategoriesPrivate::btnNew_onClicked, d));
    connect(ui->btnEdit, &QAbstractButton::clicked, this, std::bind(&DialogIncomeCategoriesPrivate::btnEdit_onClicked, d));
    connect(ui->btnDelete, &QAbstractButton::clicked, this, std::bind(&DialogIncomeCategoriesPrivate::btnDelete_onClicked, d));
    connect(ui->tableView, &QTableView::doubleClicked, this, std::bind(&DialogIncomeCategoriesPrivate::tableView_onDoubleClicked, d, std::placeholders::_1));
}

DialogIncomeCategories::~DialogIncomeCategories() {
    delete d;
    delete ui;
}
