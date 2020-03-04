#include "HBDataManager.h"
#include "HBDataModel.h"
#include <QLocale>
#include <QTableView>
#include <QHeaderView>

#define DATE_FORMAT "dd/MM/yyyy"

class HBDataManagerPrivate {
public:
    HBDataManager * const parent;
    HBFileInterface * file_interface;
    QHash<HBCollection, HBDataModel*> data_models;
    QHash<HBCollection, QStringList> model_headers;

    HBDataManagerPrivate(HBDataManager * p) :
        parent(p),
        file_interface(new HBFileInterface)
    {
        initialize_models();
    }

    ~HBDataManagerPrivate() {
        delete file_interface;
        qDeleteAll(data_models);
    }

    void initialize_models() {
        HBDataManager *p = parent;
        data_models[HBCOLLECTION_ACCOUNTS] = new HBDataModel;
        data_models[HBCOLLECTION_EXPENSE_CATEGORIES] = new HBDataModel;
        data_models[HBCOLLECTION_EXPENSE_TRANSACTIONS] = new HBDataModel;
        data_models[HBCOLLECTION_INCOME_CATEGORIES] = new HBDataModel;
        data_models[HBCOLLECTION_INCOME_TRANSACTIONS] = new HBDataModel;
        data_models[HBCOLLECTION_TRANSFER_TRANSACTIONS] = new HBDataModel;
        model_headers[HBCOLLECTION_ACCOUNTS] << "id" << "timestamp" << "checksum" << p->tr("Account Type") << p->tr("Account Name") << p->tr("Initial Balance");
        model_headers[HBCOLLECTION_EXPENSE_CATEGORIES] << "id" << "timestamp" << "checksum" << p->tr("Group Name") << p->tr("Category Name");
        model_headers[HBCOLLECTION_EXPENSE_TRANSACTIONS] << "id" << "timestamp" << "checksum" << p->tr("Transaction Date") << p->tr("Account Name") << p->tr("Group Name") << p->tr("Category Name") << p->tr("Description") << p->tr("Value");
        model_headers[HBCOLLECTION_INCOME_CATEGORIES] << "id" << "timestamp" << "checksum" << p->tr("Category Name");
        model_headers[HBCOLLECTION_INCOME_TRANSACTIONS] << "id" << "timestamp" << "checksum" << p->tr("Transaction Date") << p->tr("Account Name") << p->tr("Category Name") << p->tr("Description") << p->tr("Value");
        model_headers[HBCOLLECTION_TRANSFER_TRANSACTIONS] << "id" << "timestamp" << "checksum" << p->tr("Transaction Date") << p->tr("Origin Account Name") << p->tr("Destination Account Name") << p->tr("Value");
        data_models[HBCOLLECTION_ACCOUNTS]->setHeaderLabels(model_headers[HBCOLLECTION_ACCOUNTS]);
        data_models[HBCOLLECTION_EXPENSE_CATEGORIES]->setHeaderLabels(model_headers[HBCOLLECTION_EXPENSE_CATEGORIES]);
        data_models[HBCOLLECTION_EXPENSE_TRANSACTIONS]->setHeaderLabels(model_headers[HBCOLLECTION_EXPENSE_TRANSACTIONS]);
        data_models[HBCOLLECTION_INCOME_CATEGORIES]->setHeaderLabels(model_headers[HBCOLLECTION_INCOME_CATEGORIES]);
        data_models[HBCOLLECTION_INCOME_TRANSACTIONS]->setHeaderLabels(model_headers[HBCOLLECTION_INCOME_TRANSACTIONS]);
        data_models[HBCOLLECTION_TRANSFER_TRANSACTIONS]->setHeaderLabels(model_headers[HBCOLLECTION_TRANSFER_TRANSACTIONS]);
    }

    void load_models() {
        load_model<Account>();
        load_model<ExpenseCategory>();
        load_model<ExpenseTransaction>();
        load_model<IncomeCategory>();
        load_model<IncomeTransaction>();
        load_model<TransferTransaction>();
    }

    template<typename T>
    void load_model() {
        HBObject obj = object_cast(T());
        data_models[obj.collection]->clear();
        QList<T> list = file_interface->getObjectList<T>();
        for (const T& item : list) {
            parent->insertRow(item);
        }
        data_models[obj.collection]->setHeaderLabels(model_headers[obj.collection]);
    }

    void update_value(HBCollection collection, int column, const QString& value_id, const QString& value_new) {
        HBDataModelItem item;
        item.text = value_new;
        item.sort_data = value_new;
        item.data = value_id;
        data_models[collection]->updateItem(column, item);
    }

};

HBDataManager::HBDataManager(QObject *parent) :
    QObject(parent),
    d(new HBDataManagerPrivate(this))
{

}

HBDataManager::~HBDataManager() {
    delete d;
}

void HBDataManager::openFileInterface(const QString& filename) {
    d->file_interface->setFilename(filename);
    d->load_models();
}

QStringList HBDataManager::accountTypes() const {
    QStringList list;
    list << tr("Checking Account") << tr("Saving Account") << tr("Credit Card");
    return list;
}

HBFileInterface * HBDataManager::fileInterface() const {
    return d->file_interface;
}

void HBDataManager::setView(HBCollection collection, QTableView* view, int stretch_column) {
    if (d->data_models.contains(collection)) {
        HBDataModel *m = d->data_models[collection];
        view->setModel(m);
        view->setColumnHidden(0, true);
        view->setColumnHidden(1, true);
        view->setColumnHidden(2, true);
        view->verticalHeader()->setVisible(false);
        view->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
        if (stretch_column >= 0)
            view->horizontalHeader()->setSectionResizeMode(stretch_column, QHeaderView::Stretch);
    }
}

void HBDataManager::setView(HBCollection collection, QTableView* view, const std::initializer_list<int>& stretch_columns) {
    if (d->data_models.contains(collection)) {
        HBDataModel *m = d->data_models[collection];
        view->setModel(m);
        view->setColumnHidden(0, true);
        view->setColumnHidden(1, true);
        view->setColumnHidden(2, true);
        view->verticalHeader()->setVisible(false);
        view->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
        for (int i : stretch_columns)
            view->horizontalHeader()->setSectionResizeMode(i, QHeaderView::Stretch);
    }
}

void HBDataManager::insertRow(const Account& data) {
    QStringList account_types = accountTypes();
    HBDataModelItemList row;
    row << HBDataModelItem(data.id)
        << HBDataModelItem(data.timestamp.toString(Qt::ISODate), data.timestamp, data.timestamp)
        << HBDataModelItem(data.checksum)
        << HBDataModelItem(account_types.at(data.type), data.type, account_types.at(data.type))
        << HBDataModelItem(data.name)
        << HBDataModelItem(QLocale().toString(data.initial_balance, 'f', 2), data.initial_balance, data.initial_balance);
    d->data_models[HBCOLLECTION_ACCOUNTS]->appendRow(row);
}

void HBDataManager::insertRow(const ExpenseCategory& data) {
    HBDataModelItemList row;
    row << HBDataModelItem(data.id)
        << HBDataModelItem(data.timestamp.toString(Qt::ISODate), data.timestamp, data.timestamp)
        << HBDataModelItem(data.checksum)
        << HBDataModelItem(data.group)
        << HBDataModelItem(data.name);
    d->data_models[HBCOLLECTION_EXPENSE_CATEGORIES]->appendRow(row);
}

void HBDataManager::insertRow(const IncomeCategory& data) {
    HBDataModelItemList row;
    row << HBDataModelItem(data.id)
        << HBDataModelItem(data.timestamp.toString(Qt::ISODate), data.timestamp, data.timestamp)
        << HBDataModelItem(data.checksum)
        << HBDataModelItem(data.name);
    d->data_models[HBCOLLECTION_INCOME_CATEGORIES]->appendRow(row);
}

void HBDataManager::insertRow(const ExpenseTransaction& data) {
    HBDataModelItemList row;
    Account account = d->file_interface->getObject<Account>(data.account_id);
    ExpenseCategory category = d->file_interface->getObject<ExpenseCategory>(data.category_id);
    row << HBDataModelItem(data.id)
        << HBDataModelItem(data.timestamp.toString(Qt::ISODate), data.timestamp, data.timestamp)
        << HBDataModelItem(data.checksum)
        << HBDataModelItem(data.transaction_date.toString(DATE_FORMAT), data.transaction_date, data.transaction_date)
        << HBDataModelItem(account.name, data.account_id, account.name)
        << HBDataModelItem(category.group, data.category_id, category.group)
        << HBDataModelItem(category.name, data.category_id, category.name)
        << HBDataModelItem(data.description)
        << HBDataModelItem(QLocale().toString(data.value, 'f', 2), data.value, data.value);
    d->data_models[HBCOLLECTION_EXPENSE_TRANSACTIONS]->appendRow(row);
}

void HBDataManager::insertRow(const IncomeTransaction& data) {
    HBDataModelItemList row;
    Account account = d->file_interface->getObject<Account>(data.account_id);
    IncomeCategory category = d->file_interface->getObject<IncomeCategory>(data.category_id);
    row << HBDataModelItem(data.id)
        << HBDataModelItem(data.timestamp.toString(Qt::ISODate), data.timestamp, data.timestamp)
        << HBDataModelItem(data.checksum)
        << HBDataModelItem(data.transaction_date.toString(DATE_FORMAT), data.transaction_date, data.transaction_date)
        << HBDataModelItem(account.name, data.account_id, account.name)
        << HBDataModelItem(category.name, data.category_id, category.name)
        << HBDataModelItem(data.description)
        << HBDataModelItem(QLocale().toString(data.value, 'f', 2), data.value, data.value);
    d->data_models[HBCOLLECTION_INCOME_TRANSACTIONS]->appendRow(row);
}

void HBDataManager::insertRow(const TransferTransaction& data) {
    HBDataModelItemList row;
    Account o_account = d->file_interface->getObject<Account>(data.origin_account_id);
    Account d_account = d->file_interface->getObject<Account>(data.destination_account_id);
    row << HBDataModelItem(data.id)
        << HBDataModelItem(data.timestamp.toString(Qt::ISODate), data.timestamp, data.timestamp)
        << HBDataModelItem(data.checksum)
        << HBDataModelItem(data.transaction_date.toString(DATE_FORMAT), data.transaction_date, data.transaction_date)
        << HBDataModelItem(o_account.name, data.origin_account_id, o_account.name)
        << HBDataModelItem(d_account.name, data.destination_account_id, d_account.name)
        << HBDataModelItem(QLocale().toString(data.value, 'f', 2), data.value, data.value);
    d->data_models[HBCOLLECTION_TRANSFER_TRANSACTIONS]->appendRow(row);
}

void HBDataManager::updateRow(int r, const Account& data) {
    QStringList account_types = accountTypes();
    HBDataModelItemList row;
    row << HBDataModelItem(data.id)
        << HBDataModelItem(data.timestamp.toString(Qt::ISODate), data.timestamp, data.timestamp)
        << HBDataModelItem(data.checksum)
        << HBDataModelItem(account_types.at(data.type), data.type, account_types.at(data.type))
        << HBDataModelItem(data.name)
        << HBDataModelItem(QLocale().toString(data.initial_balance, 'f', 2), data.initial_balance, data.initial_balance);
    d->data_models[HBCOLLECTION_ACCOUNTS]->updateRow(r, row);
    d->update_value(HBCOLLECTION_EXPENSE_TRANSACTIONS, 4, data.id, data.name);
    d->update_value(HBCOLLECTION_INCOME_TRANSACTIONS, 4, data.id, data.name);
    d->update_value(HBCOLLECTION_TRANSFER_TRANSACTIONS, 4, data.id, data.name);
    d->update_value(HBCOLLECTION_TRANSFER_TRANSACTIONS, 5, data.id, data.name);
}

void HBDataManager::updateRow(int r, const ExpenseCategory& data) {
    HBDataModelItemList row;
    row << HBDataModelItem(data.id)
        << HBDataModelItem(data.timestamp.toString(Qt::ISODate), data.timestamp, data.timestamp)
        << HBDataModelItem(data.checksum)
        << HBDataModelItem(data.group)
        << HBDataModelItem(data.name);
    d->data_models[HBCOLLECTION_EXPENSE_CATEGORIES]->updateRow(r, row);
    d->update_value(HBCOLLECTION_EXPENSE_TRANSACTIONS, 5, data.id, data.group);
    d->update_value(HBCOLLECTION_EXPENSE_TRANSACTIONS, 6, data.id, data.name);
}

void HBDataManager::updateRow(int r, const IncomeCategory& data) {
    HBDataModelItemList row;
    row << HBDataModelItem(data.id)
        << HBDataModelItem(data.timestamp.toString(Qt::ISODate), data.timestamp, data.timestamp)
        << HBDataModelItem(data.checksum)
        << HBDataModelItem(data.name);
    d->data_models[HBCOLLECTION_INCOME_CATEGORIES]->updateRow(r, row);
    d->update_value(HBCOLLECTION_INCOME_TRANSACTIONS, 5, data.id, data.name);
}

void HBDataManager::updateRow(int r, const ExpenseTransaction& data) {
    HBDataModelItemList row;
    Account account = d->file_interface->getObject<Account>(data.account_id);
    ExpenseCategory category = d->file_interface->getObject<ExpenseCategory>(data.category_id);
    row << HBDataModelItem(data.id)
        << HBDataModelItem(data.timestamp.toString(Qt::ISODate), data.timestamp, data.timestamp)
        << HBDataModelItem(data.checksum)
        << HBDataModelItem(data.transaction_date.toString(DATE_FORMAT), data.transaction_date, data.transaction_date)
        << HBDataModelItem(account.name, data.account_id, account.name)
        << HBDataModelItem(category.group, data.category_id, category.group)
        << HBDataModelItem(category.name, data.category_id, category.name)
        << HBDataModelItem(data.description)
        << HBDataModelItem(QLocale().toString(data.value, 'f', 2), data.value, data.value);
    d->data_models[HBCOLLECTION_EXPENSE_TRANSACTIONS]->updateRow(r, row);
}

void HBDataManager::updateRow(int r, const IncomeTransaction& data) {
    HBDataModelItemList row;
    Account account = d->file_interface->getObject<Account>(data.account_id);
    IncomeCategory category = d->file_interface->getObject<IncomeCategory>(data.category_id);
    row << HBDataModelItem(data.id)
        << HBDataModelItem(data.timestamp.toString(Qt::ISODate), data.timestamp, data.timestamp)
        << HBDataModelItem(data.checksum)
        << HBDataModelItem(data.transaction_date.toString(DATE_FORMAT), data.transaction_date, data.transaction_date)
        << HBDataModelItem(account.name, data.account_id, account.name)
        << HBDataModelItem(category.name, data.category_id, category.name)
        << HBDataModelItem(data.description)
        << HBDataModelItem(QLocale().toString(data.value, 'f', 2), data.value, data.value);
    d->data_models[HBCOLLECTION_INCOME_TRANSACTIONS]->updateRow(r, row);
}

void HBDataManager::updateRow(int r, const TransferTransaction& data) {
    HBDataModelItemList row;
    Account o_account = d->file_interface->getObject<Account>(data.origin_account_id);
    Account d_account = d->file_interface->getObject<Account>(data.destination_account_id);
    row << HBDataModelItem(data.id)
        << HBDataModelItem(data.timestamp.toString(Qt::ISODate), data.timestamp, data.timestamp)
        << HBDataModelItem(data.checksum)
        << HBDataModelItem(data.transaction_date.toString(DATE_FORMAT), data.transaction_date, data.transaction_date)
        << HBDataModelItem(o_account.name, data.origin_account_id, o_account.name)
        << HBDataModelItem(d_account.name, data.destination_account_id, d_account.name)
        << HBDataModelItem(QLocale().toString(data.value, 'f', 2), data.value, data.value);
    d->data_models[HBCOLLECTION_TRANSFER_TRANSACTIONS]->updateRow(r, row);
}

void HBDataManager::deleteObject(HBCollection collection, int row) {
    QString id = objectId(collection, row);
    if (!id.isEmpty()) {
        d->file_interface->deleteObject(id);
        d->data_models[collection]->removeRow(row);
        switch (collection) {
        case HBCOLLECTION_ACCOUNTS:
            d->update_value(HBCOLLECTION_EXPENSE_TRANSACTIONS, 4, id, QString());
            d->update_value(HBCOLLECTION_INCOME_TRANSACTIONS, 4, id, QString());
            d->update_value(HBCOLLECTION_TRANSFER_TRANSACTIONS, 4, id, QString());
            d->update_value(HBCOLLECTION_TRANSFER_TRANSACTIONS, 5, id, QString());
            break;

        case HBCOLLECTION_EXPENSE_CATEGORIES:
            d->update_value(HBCOLLECTION_EXPENSE_TRANSACTIONS, 5, id, QString());
            d->update_value(HBCOLLECTION_EXPENSE_TRANSACTIONS, 6, id, QString());
            break;

        case HBCOLLECTION_INCOME_CATEGORIES:
            d->update_value(HBCOLLECTION_INCOME_TRANSACTIONS, 5, id, QString());
            break;

        default:
            break;
        }
    }
}

QString HBDataManager::objectId(HBCollection collection, int row) {
    QModelIndex index = d->data_models[collection]->index(row, 0);
    if (index.isValid())
        return index.data().toString();
    return QString();
}
