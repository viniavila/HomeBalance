#ifndef HBDATAMODEL_H
#define HBDATAMODEL_H

#include <QSortFilterProxyModel>

class HBDataModelPrivate;

struct HBDataModelItem {
    QString text;
    QVariant data;
    QVariant sort_data;
    inline HBDataModelItem() { }
    inline HBDataModelItem(const QString& text) : text(text), data(text), sort_data(text) { }
    inline HBDataModelItem(const QString& text, const QVariant& data, const QVariant& sort_data) :
        text(text), data(data), sort_data(sort_data) { }
};
typedef QList<HBDataModelItem> HBDataModelItemList;

struct HBDataModelFilter {
    enum Type {
        FILTER_TEXT,
        FILTER_GE_DATE,
        FILTER_LE_DATE,
        FILTER_GE_FLOAT,
        FILTER_LE_FLOAT,
        FILTER_GE_INT,
        FILTER_LE_INT
    } type;
    int column;
    QVariant value;
    inline HBDataModelFilter() { }
    inline HBDataModelFilter(int column, Type t, const QVariant& value) : type(t), column(column), value(value) { }
};
typedef QList<HBDataModelFilter> HBDataModelFilterList;

class HBDataModel : public QSortFilterProxyModel {
    Q_OBJECT

public:
    explicit HBDataModel(QObject *parent = nullptr);
    virtual ~HBDataModel();

    void appendRow(const HBDataModelItemList& row);
    void updateRow(int row, const HBDataModelItemList& row_new);
    void removeRow(int row);
    void clear();

    void updateItem(int column, const HBDataModelItem& item);

    void addFilter(const HBDataModelFilter& filter);
    void setFilters(const HBDataModelFilterList& filters);
    void clearFilters();

    void setHeaderLabels(const QStringList& labels);

signals:
    void modelUpdated() const;

protected:
    virtual bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const;

private:
    HBDataModelPrivate * const d;
    void setSortRole(int) = delete;
    void setFilterCaseSensitivity(Qt::CaseSensitivity) = delete;
    void setFilterKeyColumn(int) = delete;
    void setFilterFixedString(const QString&) = delete;
    void setFilterRegExp(const QRegExp&) = delete;
    void setFilterRole(int) = delete;
    void setFilterWildcard(const QString&) = delete;
};

#endif // HBDATAMODEL_H
