#ifndef HBDATAMANAGER_H
#define HBDATAMANAGER_H

#include <QObject>
#include <HBData.h>

class HBDataManagerPrivate;
class QTableView;

class HBDataManager : public QObject {
    Q_OBJECT
public:
    explicit HBDataManager(QObject *parent = nullptr);
    virtual ~HBDataManager();

    void openFileInterface(const QString& filename);
    QStringList accountTypes() const;

    void setView(HBCollection collection, QTableView* t, int stretch_column);
    void setView(HBCollection collection, QTableView* t, const std::initializer_list<int>& stretch_columns);

    template<typename T>
    T getObject(int row) const {
        HBObject obj = object_cast(T());
        QString id = objectId(obj.collection, row);
        return fileInterface()->getObject<T>(id);
    }

    template<typename T>
    T getObject(const QString& id) const {
        return fileInterface()->getObject<T>(id);
    }

    template<typename T>
    T getObject(const QString& key, const QString& value) const {
        return fileInterface()->getObject<T>(key, value);
    }

    template<typename T>
    QList<T> getObjectList() const {
        return fileInterface()->getObjectList<T>();
    }

    template<typename T>
    QList<T> getObjectList(const QString& key, const QString& value) const {
        return fileInterface()->getObjectList<T>(key, value);
    }

    template<typename T>
    QStringList getValueList(const QString& key, bool sorted = false) const {
        return fileInterface()->getValueList<T>(key, sorted);
    }

    template<typename T>
    void insertObject(T& obj) {
        fileInterface()->insertObject(obj);
        insertRow(obj);
    }

    template<typename T>
    void updateObject(int row, T& obj) {
        fileInterface()->updateObject(obj);
        updateRow(row, obj);
    }

    template<typename T>
    void deleteObject(int row) {
        HBObject obj = object_cast(T());
        deleteObject(obj.collection, row);
    }

private:
    HBDataManagerPrivate * const d;
    HBFileInterface * fileInterface();
    HBFileInterface const * fileInterface() const;
    void deleteObject(HBCollection collection, int row);
    QString objectId(HBCollection collection, int row) const;

    void insertRow(const Account& d);
    void insertRow(const ExpenseCategory& d);
    void insertRow(const IncomeCategory& d);
    void insertRow(const ExpenseTransaction& d);
    void insertRow(const IncomeTransaction& d);
    void insertRow(const TransferTransaction& d);
    void updateRow(int row, const Account& d);
    void updateRow(int row, const ExpenseCategory& d);
    void updateRow(int row, const IncomeCategory& d);
    void updateRow(int row, const ExpenseTransaction& d);
    void updateRow(int row, const IncomeTransaction& d);
    void updateRow(int row, const TransferTransaction& d);
    friend class HBDataManagerPrivate;
};

#endif // HBDATAMANAGER_H
