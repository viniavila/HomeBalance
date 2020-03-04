#include "HBData.h"

template<> Account object_cast(const HBObject& o) {
    Q_ASSERT(o.collection == HBCOLLECTION_ACCOUNTS);
    Account d;
    d.id = o.id;
    d.timestamp = o.timestamp;
    d.checksum = o.checksum;
    d.type = static_cast<Account::Type>(o.data["type"].toInt());
    d.name = o.data["name"];
    d.initial_balance = o.data["initial_balance"].toFloat();
    return d;
}

template<> HBObject object_cast(const Account& d) {
    HBObject o;
    o.collection = HBCOLLECTION_ACCOUNTS;
    o.id = d.id;
    o.timestamp = d.timestamp;
    o.checksum = d.checksum;
    o.data["type"] = QString::number(d.type);
    o.data["name"] = d.name;
    o.data["initial_balance"] = QString::number(d.initial_balance, 'f', 2);
    return o;
}

template<> IncomeCategory object_cast(const HBObject& o) {
    Q_ASSERT(o.collection == HBCOLLECTION_INCOME_CATEGORIES);
    IncomeCategory d;
    d.id = o.id;
    d.timestamp = o.timestamp;
    d.checksum = o.checksum;
    d.name = o.data["name"];
    return d;
}

template<> HBObject object_cast(const IncomeCategory& d) {
    HBObject o;
    o.collection = HBCOLLECTION_INCOME_CATEGORIES;
    o.id = d.id;
    o.timestamp = d.timestamp;
    o.checksum = d.checksum;
    o.data["name"] = d.name;
    return o;
}

template<> ExpenseCategory object_cast(const HBObject& o) {
    Q_ASSERT(o.collection == HBCOLLECTION_EXPENSE_CATEGORIES);
    ExpenseCategory d;
    d.id = o.id;
    d.timestamp = o.timestamp;
    d.checksum = o.checksum;
    d.group = o.data["group"];
    d.name = o.data["name"];
    return d;
}

template<> HBObject object_cast(const ExpenseCategory& d) {
    HBObject o;
    o.collection = HBCOLLECTION_EXPENSE_CATEGORIES;
    o.id = d.id;
    o.timestamp = d.timestamp;
    o.checksum = d.checksum;
    o.data["group"] = d.group;
    o.data["name"] = d.name;
    return o;
}

template<> IncomeTransaction object_cast(const HBObject& o) {
    Q_ASSERT(o.collection == HBCOLLECTION_INCOME_TRANSACTIONS);
    IncomeTransaction d;
    d.id = o.id;
    d.timestamp = o.timestamp;
    d.checksum = o.checksum;
    d.transaction_date = QDate::fromString(o.data["transaction_date"], Qt::ISODate);
    d.account_id = o.data["account_id"];
    d.category_id = o.data["category_id"];
    d.description = o.data["description"];
    d.value = o.data["value"].toFloat();
    return d;
}

template<> HBObject object_cast(const IncomeTransaction& d) {
    HBObject o;
    o.collection = HBCOLLECTION_INCOME_TRANSACTIONS;
    o.id = d.id;
    o.timestamp = d.timestamp;
    o.checksum = d.checksum;
    o.data["transaction_date"] = d.transaction_date.toString(Qt::ISODate);
    o.data["account_id"] = d.account_id;
    o.data["category_id"] = d.category_id;
    o.data["description"] = d.description;
    o.data["value"] = QString::number(d.value, 'f', 2);
    return o;
}

template<> ExpenseTransaction object_cast(const HBObject& o) {
    Q_ASSERT(o.collection == HBCOLLECTION_EXPENSE_TRANSACTIONS);
    ExpenseTransaction d;
    d.id = o.id;
    d.timestamp = o.timestamp;
    d.checksum = o.checksum;
    d.transaction_date = QDate::fromString(o.data["transaction_date"], Qt::ISODate);
    d.account_id = o.data["account_id"];
    d.category_id = o.data["category_id"];
    d.description = o.data["description"];
    d.value = o.data["value"].toFloat();
    return d;
}

template<> HBObject object_cast(const ExpenseTransaction& d) {
    HBObject o;
    o.collection = HBCOLLECTION_EXPENSE_TRANSACTIONS;
    o.id = d.id;
    o.timestamp = d.timestamp;
    o.checksum = d.checksum;
    o.data["transaction_date"] = d.transaction_date.toString(Qt::ISODate);
    o.data["account_id"] = d.account_id;
    o.data["category_id"] = d.category_id;
    o.data["description"] = d.description;
    o.data["value"] = QString::number(d.value, 'f', 2);
    return o;
}

template<> TransferTransaction object_cast(const HBObject& o) {
    Q_ASSERT(o.collection == HBCOLLECTION_TRANSFER_TRANSACTIONS);
    TransferTransaction d;
    d.id = o.id;
    d.timestamp = o.timestamp;
    d.checksum = o.checksum;
    d.transaction_date = QDate::fromString(o.data["transaction_date"], Qt::ISODate);
    d.origin_account_id = o.data["origin_account_id"];
    d.destination_account_id = o.data["destination_account_id"];
    d.value = o.data["value"].toFloat();
    return d;
}

template<> HBObject object_cast(const TransferTransaction& d) {
    HBObject o;
    o.collection = HBCOLLECTION_TRANSFER_TRANSACTIONS;
    o.id = d.id;
    o.timestamp = d.timestamp;
    o.checksum = d.checksum;
    o.data["transaction_date"] = d.transaction_date.toString(Qt::ISODate);
    o.data["origin_account_id"] = d.origin_account_id;
    o.data["destination_account_id"] = d.destination_account_id;
    o.data["value"] = QString::number(d.value, 'f', 2);
    return o;
}
