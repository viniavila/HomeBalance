#ifndef HBDATA_H
#define HBDATA_H

#include "HBFileInterface.h"

struct Account {
    QString id;
    QDateTime timestamp;
    QString checksum;
    enum Type {
        ACCOUNT_CHECKING,
        ACCOUNT_SAVING,
        ACCOUNT_CREDIT_CARD
    } type;
    QString name;
    float initial_balance;
};

struct IncomeCategory {
    QString id;
    QDateTime timestamp;
    QString checksum;
    QString name;
};

struct ExpenseCategory {
    QString id;
    QDateTime timestamp;
    QString checksum;
    QString group;
    QString name;
};

struct IncomeTransaction {
    QString id;
    QDateTime timestamp;
    QString checksum;
    QDate transaction_date;
    QString account_id;
    QString category_id;
    QString description;
    float value;
};

struct ExpenseTransaction {
    QString id;
    QDateTime timestamp;
    QString checksum;
    QDate transaction_date;
    QString account_id;
    QString category_id;
    QString description;
    float value;
};

struct TransferTransaction {
    QString id;
    QDateTime timestamp;
    QString checksum;
    QDate transaction_date;
    QString origin_account_id;
    QString destination_account_id;
    float value;
};

typedef QList<Account> AccountList;
typedef QList<IncomeCategory> IncomeCategoryList;
typedef QList<ExpenseCategory> ExpenseCategoryList;
typedef QList<IncomeTransaction> IncomeTransactionList;
typedef QList<ExpenseTransaction> ExpenseTransactionList;
typedef QList<TransferTransaction> TransferTransactionList;

template<> Account object_cast(const HBObject& o);
template<> IncomeCategory object_cast(const HBObject& o);
template<> ExpenseCategory object_cast(const HBObject& o);
template<> IncomeTransaction object_cast(const HBObject& o);
template<> ExpenseTransaction object_cast(const HBObject& o);
template<> TransferTransaction object_cast(const HBObject& o);

template<> HBObject object_cast(const Account& d);
template<> HBObject object_cast(const IncomeCategory& d);
template<> HBObject object_cast(const ExpenseCategory& d);
template<> HBObject object_cast(const IncomeTransaction& d);
template<> HBObject object_cast(const ExpenseTransaction& d);
template<> HBObject object_cast(const TransferTransaction& d);

#endif // HBDATA_H
