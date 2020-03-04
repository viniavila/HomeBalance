#include "DialogAccounts.h"
#include "ui_DialogAccounts.h"
#include <InputAccount.h>
#include <HBGlobalFunctions.h>

class DialogAccountsPrivate {
public:
    DialogAccounts * const parent;
    Ui::DialogAccounts * const ui;
    HBDataManager * m_data;

    DialogAccountsPrivate(DialogAccounts * p, Ui::DialogAccounts * ui) :
        parent(p),
        ui(ui)
    {

    }

    void btnNew_onClicked() {
        InputAccount dlg(m_data, parent);
        if (dlg.exec()) {
            Account d = dlg.getData();
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
                m_data->deleteObject<Account>(index.row());
                emit parent->dataModified();
            }
        }
    }

    void tableView_onDoubleClicked(const QModelIndex& index) {
        if (index.isValid()) {
            Account d = m_data->getObject<Account>(index.row());
            InputAccount dlg(m_data, parent);
            dlg.setWindowTitle(parent->tr("Edit Account"));
            dlg.setData(d);
            if (dlg.exec()) {
                dlg.getData(d);
                m_data->updateObject(index.row(), d);
                emit parent->dataModified();
            }
        }
    }
};

DialogAccounts::DialogAccounts(HBDataManager * m, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogAccounts),
    d(new DialogAccountsPrivate(this, ui))
{
    ui->setupUi(this);
    d->m_data = m;
    m->setView(HBCOLLECTION_ACCOUNTS, ui->tableView, 4);
    ui->tableView->sortByColumn(4, Qt::AscendingOrder);
    ui->tableView->verticalHeader()->setDefaultSectionSize(fontMetrics().height()*1.5);

    connect(ui->btnNew, &QAbstractButton::clicked, this, std::bind(&DialogAccountsPrivate::btnNew_onClicked, d));
    connect(ui->btnEdit, &QAbstractButton::clicked, this, std::bind(&DialogAccountsPrivate::btnEdit_onClicked, d));
    connect(ui->btnDelete, &QAbstractButton::clicked, this, std::bind(&DialogAccountsPrivate::btnDelete_onClicked, d));
    connect(ui->tableView, &QTableView::doubleClicked, this, std::bind(&DialogAccountsPrivate::tableView_onDoubleClicked, d, std::placeholders::_1));
}

DialogAccounts::~DialogAccounts() {
    delete d;
    delete ui;
}
