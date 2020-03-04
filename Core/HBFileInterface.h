#ifndef HBFILEINTERFACE_H
#define HBFILEINTERFACE_H

#include <QThread>
#include <QDateTime>

class HBFileInterfacePrivate;

enum HBCollection {
    HBCOLLECTION_ACCOUNTS,
    HBCOLLECTION_INCOME_CATEGORIES,
    HBCOLLECTION_EXPENSE_CATEGORIES,
    HBCOLLECTION_INCOME_TRANSACTIONS,
    HBCOLLECTION_EXPENSE_TRANSACTIONS,
    HBCOLLECTION_TRANSFER_TRANSACTIONS
};

struct HBObject {
    HBCollection collection;
    QString id;
    QDateTime timestamp;
    QString checksum;
    QHash<QString, QString> data;
};
typedef QList<HBObject> HBObjectList;

template<typename T> HBObject object_cast(const T&);
template<typename T> T object_cast(const HBObject&);

class HBFileInterface : public QThread {
    Q_OBJECT
public:
    HBFileInterface(QObject *parent = nullptr);
    virtual ~HBFileInterface();

    bool isOpen() const;
    void setFilename(const QString& filename);
    QString filename() const;

    template<typename T>
    void insertObject(T& t) {
        HBObject obj = object_cast(t);
        insertObject(obj);
        t.id = obj.id;
        t.timestamp = obj.timestamp;
        t.checksum = obj.checksum;
    }

    template<typename T>
    void updateObject(T& t) {
        HBObject obj = object_cast(t);
        updateObject(obj);
        t.timestamp = obj.timestamp;
        t.checksum = obj.checksum;
    }

    template<typename T>
    T getObject(const QString& id) const {
        return object_cast<T>(getObject(id));
    }

    template<typename T>
    T getObject(const QString& key, const QString& value) const {
        HBObject obj = object_cast(T());
        return object_cast<T>(getObject(obj.collection, key, value));
    }

    template<typename T>
    QList<T> getObjectList() const {
        HBObject obj = object_cast(T());
        HBObjectList obj_list = getObjectList(obj.collection);
        QList<T> t_list;
        for (const HBObject& obj : obj_list)
            t_list << object_cast<T>(obj);
        return t_list;
    }

    template<typename T>
    QList<T> getObjectList(const QString& key, const QString& value) const {
        HBObject obj = object_cast(T());
        HBObjectList obj_list = getObjectList(obj.collection, key, value);
        QList<T> t_list;
        for (const HBObject& obj : obj_list)
            t_list << object_cast<T>(obj);
        return t_list;
    }

    void deleteObject(const QString& id);
    QStringList getValueList(HBCollection collection, const QString& key, bool sorted = false) const;

protected:
    void run();

private:
    HBFileInterfacePrivate * const d;
    void insertObject(HBObject& obj);
    void updateObject(HBObject& obj);
    HBObject getObject(const QString& id) const;
    HBObject getObject(HBCollection collection, const QString& key, const QString& value) const;
    HBObjectList getObjectList(HBCollection collection) const;
    HBObjectList getObjectList(HBCollection collection, const QString& key, const QString& value) const;
};

#endif // HBFILEINTERFACE_H
