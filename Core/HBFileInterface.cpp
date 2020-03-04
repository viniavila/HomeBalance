#include "HBFileInterface.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QStringBuilder>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include <QVariant>
#include <QCryptographicHash>
#include <QSet>
#include <QMutex>
#include <random>
#include <iostream>

const char *abz = "zxcvbnmasdfghjklqwertyuiop1234567890POIUYTREWQLKJHGFDSAMNBVCXZ";

class HBFileInterfacePrivate {
public:
    HBFileInterface * const parent;
    QString connection;
    QSqlDatabase database;

    QHash<HBCollection, QString> collection_titles;
    QHash<QString, HBObject> obj_list;

    std::default_random_engine random_engine;
    std::uniform_int_distribution<qint32> distribution;

    struct Task {
        enum Type {
            TASK_INSERT,
            TASK_UPDATE,
            TASK_DELETE
        } type;
        HBObject obj;
    };
    QList<Task> tasklist;
    QMutex mutex;

    HBFileInterfacePrivate(HBFileInterface * p) :
        parent(p),
        connection("HBFileInterface::connection: initialized at " % QDateTime::currentDateTimeUtc().toString(Qt::ISODateWithMs)),
        database(QSqlDatabase::addDatabase("QSQLITE", connection)),
        distribution(0, strlen(abz)-1)
    {
        initialize_structure();
    }

    ~HBFileInterfacePrivate() {
        if (parent->isRunning()) parent->wait();
        if (database.isOpen()) database.close();
    }

    void initialize_structure() {
        collection_titles[HBCOLLECTION_ACCOUNTS] = "Accounts";
        collection_titles[HBCOLLECTION_INCOME_CATEGORIES] = "Income Categories";
        collection_titles[HBCOLLECTION_EXPENSE_CATEGORIES] = "Expense Categories";
        collection_titles[HBCOLLECTION_INCOME_TRANSACTIONS] = "Income Transactions";
        collection_titles[HBCOLLECTION_EXPENSE_TRANSACTIONS] = "Expense Transactions";
        collection_titles[HBCOLLECTION_TRANSFER_TRANSACTIONS] = "Transfer Transactions";
    }

    void initialize_file() {
        if (database.isOpen()) {
            QSqlQuery q(database);
            for (const QString& title : collection_titles) {
                QString sql = "CREATE TABLE IF NOT EXISTS \"" % title % "\"(id TEXT, timestamp DATETIME, checksum TEXT, object TEXT);";
                q.exec(sql);
                std::cout << "Query Executed: " << sql.toStdString() << std::endl;
            }
            q.finish();
        }
    }

    void load_database() {
        if (database.isOpen()) {
            QSqlQuery q(database);
            for (HBCollection collection : collection_titles.keys()) {
                QString sql = "SELECT id, timestamp, checksum, object FROM \"" % collection_titles.value(collection) % "\";";
                q.exec(sql);
                std::cout << "Query Executed: " << sql.toStdString() << std::endl;
                while (q.next()) {
                    HBObject obj = extract_data_from_xml(q.value(3).toString().replace("''", "'"));
                    obj.id = q.value(0).toString();
                    obj.timestamp = QDateTime::fromString(q.value(1).toString(), Qt::ISODate);
                    obj.checksum = q.value(2).toString();
                    obj.collection = collection;
                    obj_list[obj.id] = obj;
                }
                q.finish();
            }
        }
    }

    QString create_xml_from_data(const HBObject& obj) {
        QString xml;
        QXmlStreamWriter stream(&xml);
        stream.writeStartDocument();
        stream.writeStartElement("object");
        for (const QString& key : obj.data.keys())
            stream.writeTextElement(key, obj.data.value(key));
        stream.writeEndElement();
        stream.writeEndDocument();
        return xml;
    }

    HBObject extract_data_from_xml(const QString& xml) {
        HBObject obj;
        QXmlStreamReader stream(xml);
        while (!stream.atEnd()) {
            stream.readNext();
            if (stream.tokenType() == QXmlStreamReader::StartElement && stream.name() != "object") {
                QString key = stream.name().toString();
                stream.readNext();
                QString value = stream.text().toString();
                obj.data[key] = value;
            }
        }
        return obj;
    }

    QString generate_id() {
        char id[13];
        id[12] = '\0';
        do {
            for (int i = 0; i < 12; ++i)
                id[i] = abz[distribution(random_engine)];
        } while (obj_list.keys().contains(id));
        return id;
    }

    inline QDateTime timestamp() {
        return QDateTime::currentDateTimeUtc();
    }

    QString do_checksum(const QStringList& values) {
        QByteArray arr_data;
        arr_data.append(values.join("|"));
        return (QCryptographicHash::hash(arr_data, QCryptographicHash::Md5).toHex());
    }

    void sql_insert_helper(const HBObject& obj) {
        if (!database.isOpen()) database.open();
        QSqlQuery q(database);
        QString title = collection_titles.value(obj.collection);
        QString sql = QString("INSERT INTO \"%1\" VALUES ('%2','%3','%4','%5');")
                .arg(title)
                .arg(obj.id)
                .arg(obj.timestamp.toString(Qt::ISODate))
                .arg(obj.checksum)
                .arg(create_xml_from_data(obj).replace("'", "''"));
        q.exec(sql);
        std::cout << "Query Executed: " << sql.toStdString() << std::endl;
        q.finish();
    }

    void sql_update_helper(const HBObject& obj) {
        if (!database.isOpen()) database.open();
        QSqlQuery q(database);
        QString title = collection_titles.value(obj.collection);
        QString sql = QString("UPDATE \"%1\" SET timestamp='%2', checksum='%3', object='%4' WHERE id='%5';")
                .arg(title)
                .arg(obj.timestamp.toString(Qt::ISODate))
                .arg(obj.checksum)
                .arg(create_xml_from_data(obj).replace("'", "''"))
                .arg(obj.id);
        q.exec(sql);
        std::cout << "Query Executed: " << sql.toStdString() << std::endl;
        q.finish();
    }

    void sql_delete_helper(const HBObject& obj) {
        if (!database.isOpen()) database.open();
        QSqlQuery q(database);
        QString title = collection_titles.value(obj.collection);
        QString sql = QString("DELETE FROM \"%1\" WHERE id='%2';")
                .arg(title)
                .arg(obj.id);
        q.exec(sql);
        std::cout << "Query Executed: " << sql.toStdString() << std::endl;
        q.finish();
    }
};

HBFileInterface::HBFileInterface(QObject *parent) :
    QThread(parent),
    d(new HBFileInterfacePrivate(this))
{

}

HBFileInterface::~HBFileInterface() {
    QString c = d->connection;
    delete d;
    QSqlDatabase::removeDatabase(c);
}

bool HBFileInterface::isOpen() const {
    return (d->database.isValid() && d->database.isOpen());
}

void HBFileInterface::setFilename(const QString& filename) {
    if (d->database.isOpen())
        d->database.close();
    d->obj_list.clear();
    d->database.setDatabaseName(filename);
    if (d->database.open()) {
        d->initialize_file();
        d->load_database();
    }
}

QString HBFileInterface::filename() const {
    return d->database.databaseName();
}

void HBFileInterface::run() {
    while (!d->tasklist.isEmpty()) {
        d->mutex.lock();
        HBFileInterfacePrivate::Task task = d->tasklist.takeFirst();
        d->mutex.unlock();
        switch (task.type) {
        case HBFileInterfacePrivate::Task::TASK_INSERT:
            d->sql_insert_helper(task.obj);
            break;

        case HBFileInterfacePrivate::Task::TASK_UPDATE:
            d->sql_update_helper(task.obj);
            break;

        case HBFileInterfacePrivate::Task::TASK_DELETE:
            d->sql_delete_helper(task.obj);
            break;
        }
    }
}

void HBFileInterface::insertObject(HBObject& obj) {
    QStringList keys = obj.data.keys();
    std::sort(keys.begin(), keys.end());
    QStringList values;
    for (const QString& key : keys)
        values << obj.data.value(key);
    obj.id = d->generate_id();
    obj.timestamp = d->timestamp();
    obj.checksum = d->do_checksum(values);

    HBFileInterfacePrivate::Task t;
    t.type = HBFileInterfacePrivate::Task::TASK_INSERT;
    t.obj = obj;
    d->mutex.lock();
    d->tasklist.append(t);
    d->mutex.unlock();
    if (!isRunning()) start();

    d->obj_list[obj.id] = obj;
}

void HBFileInterface::updateObject(HBObject& obj) {
    QStringList keys = obj.data.keys();
    std::sort(keys.begin(), keys.end());
    QStringList values;
    for (const QString& key : keys)
        values << obj.data.value(key);
    obj.timestamp = d->timestamp();
    obj.checksum = d->do_checksum(values);

    HBFileInterfacePrivate::Task t;
    t.type = HBFileInterfacePrivate::Task::TASK_UPDATE;
    t.obj = obj;
    d->mutex.lock();
    d->tasklist.append(t);
    d->mutex.unlock();
    if (!isRunning()) start();

    d->obj_list[obj.id] = obj;
}

void HBFileInterface::deleteObject(const QString& id) {
    HBObject obj = d->obj_list[id];

    HBFileInterfacePrivate::Task t;
    t.type = HBFileInterfacePrivate::Task::TASK_DELETE;
    t.obj = obj;
    d->mutex.lock();
    d->tasklist.append(t);
    d->mutex.unlock();
    if (!isRunning()) start();

    d->obj_list.remove(id);
}

QStringList HBFileInterface::getValueList(HBCollection collection, const QString& key, bool sorted) const {
    QSet<QString> set;
    for (const HBObject& obj : d->obj_list) {
        if (obj.collection == collection && obj.data.contains(key))
            set << obj.data.value(key);
    }
    QStringList list = set.toList();
    if (sorted)
        std::sort(list.begin(), list.end(), [](const QString& s1, const QString& s2){ return QString::localeAwareCompare(s1, s2) < 0; });
    return list;
}

HBObject HBFileInterface::getObject(const QString& id) const {
    return d->obj_list.value(id);
}

HBObject HBFileInterface::getObject(HBCollection collection, const QString& key, const QString& value) const {
    for (const HBObject& obj : d->obj_list) {
        if (obj.collection == collection && obj.data.keys().contains(key) && obj.data.value(key) == value)
            return obj;
    }
    return HBObject();
}

HBObjectList HBFileInterface::getObjectList(HBCollection collection) const {
    HBObjectList list;
    for (const HBObject& obj : d->obj_list) {
        if (obj.collection == collection)
            list << obj;
    }
    return list;
}

HBObjectList HBFileInterface::getObjectList(HBCollection collection, const QString& key, const QString& value) const {
    HBObjectList list;
    for (const HBObject& obj : d->obj_list) {
        if (obj.collection == collection && obj.data.keys().contains(key) && obj.data.value(key) == value)
            list << obj;
    }
    return list;
}
