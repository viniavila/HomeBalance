#include "DialogExpenseCategories.h"
#include "ui_DialogExpenseCategories.h"
#include <InputExpenseCategory.h>
#include <HBGlobalFunctions.h>

class DialogExpenseCategoriesPrivate {
public:
    DialogExpenseCategories * const parent;
    Ui::DialogExpenseCategories * const ui;
    HBDataManager * m_data;

    DialogExpenseCategoriesPrivate(DialogExpenseCategories * p, Ui::DialogExpenseCategories * ui) :
        parent(p),
        ui(ui)
    {

    }

    void btnNew_onClicked() {
        InputExpenseCategory dlg(m_data, parent);
        if (dlg.exec()) {
            ExpenseCategory d = dlg.getData();
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
                m_data->deleteObject<ExpenseCategory>(index.row());
                emit parent->dataModified();
            }
        }
    }

    void tableView_onDoubleClicked(const QModelIndex& index) {
        if (index.isValid()) {
            ExpenseCategory d = m_data->getObject<ExpenseCategory>(index.row());
            InputExpenseCategory dlg(m_data, parent);
            dlg.setWindowTitle(parent->tr("Edit Expense Category"));
            dlg.setData(d);
            if (dlg.exec()) {
                dlg.getData(d);
                m_data->updateObject(index.row(), d);
                emit parent->dataModified();
            }
        }
    }

};

DialogExpenseCategories::DialogExpenseCategories(HBDataManager * m, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogExpenseCategories),
    d(new DialogExpenseCategoriesPrivate(this, ui))
{
    ui->setupUi(this);
    d->m_data = m;
    m->setView(HBCOLLECTION_EXPENSE_CATEGORIES, ui->tableView, {3,4});
    ui->tableView->sortByColumn(3, Qt::AscendingOrder);
    ui->tableView->sortByColumn(4, Qt::AscendingOrder);
    ui->tableView->verticalHeader()->setDefaultSectionSize(fontMetrics().height()*1.5);

    connect(ui->btnNew, &QAbstractButton::clicked, this, std::bind(&DialogExpenseCategoriesPrivate::btnNew_onClicked, d));
    connect(ui->btnEdit, &QAbstractButton::clicked, this, std::bind(&DialogExpenseCategoriesPrivate::btnEdit_onClicked, d));
    connect(ui->btnDelete, &QAbstractButton::clicked, this, std::bind(&DialogExpenseCategoriesPrivate::btnDelete_onClicked, d));
    connect(ui->tableView, &QTableView::doubleClicked, this, std::bind(&DialogExpenseCategoriesPrivate::tableView_onDoubleClicked, d, std::placeholders::_1));
}

DialogExpenseCategories::~DialogExpenseCategories() {
    delete d;
    delete ui;
}
