#include "HBDataModel.h"
#include <QStandardItemModel>
#include <QTableView>
#include <QDate>

#define VALUE_ROLE Qt::UserRole+1
#define SORT_ROLE Qt::UserRole+2

class HBDataModelPrivate {
public:
    HBDataModel * const parent;
    QStandardItemModel * model;
    HBDataModelFilterList filters;

    HBDataModelPrivate(HBDataModel * p) :
        parent(p),
        model(new QStandardItemModel)
    {

    }

    ~HBDataModelPrivate() {
        delete model;
    }


};

HBDataModel::HBDataModel(QObject *parent) :
    QSortFilterProxyModel(parent),
    d(new HBDataModelPrivate(this))
{
    setSourceModel(d->model);
    setSortCaseSensitivity(Qt::CaseInsensitive);
    setSortLocaleAware(true);
    QSortFilterProxyModel::setSortRole(SORT_ROLE);
}

HBDataModel::~HBDataModel() {
    delete d;
}

void HBDataModel::appendRow(const HBDataModelItemList& row) {
    QList<QStandardItem*> items;
    for (const HBDataModelItem& r : row) {
        QStandardItem * item = new QStandardItem(r.text);
        item->setData(r.sort_data, SORT_ROLE);
        item->setData(r.data, VALUE_ROLE);
        item->setTextAlignment(Qt::AlignCenter);
        item->setFlags(item->flags() ^ Qt::ItemIsEditable);
        items << item;
    }
    d->model->appendRow(items);
    emit modelUpdated();
}

void HBDataModel::updateRow(int row, const HBDataModelItemList& row_new) {
    QModelIndex my_index = index(row, 0);
    if (my_index.isValid()) {
        int model_row = mapToSource(my_index).row();
        int col = 0;
        for (const HBDataModelItem& r : row_new) {
            QStandardItem * item = d->model->item(model_row, col);
            item->setText(r.text);
            item->setData(r.sort_data, SORT_ROLE);
            item->setData(r.data, VALUE_ROLE);
            col++;
        }
        emit modelUpdated();
    }
}

void HBDataModel::removeRow(int row) {
    QModelIndex my_index = index(row, 0);
    if (my_index.isValid()) {
        int model_row = mapToSource(my_index).row();
        d->model->removeRow(model_row);
        emit modelUpdated();
    }
}

void HBDataModel::clear() {
    d->model->clear();
}

void HBDataModel::updateItem(int column, const HBDataModelItem& item_new) {
    for (int row = 0; row < d->model->rowCount(); ++row) {
        QStandardItem * item = d->model->item(row, column);
        if (item->data(VALUE_ROLE).toString() == item_new.data) {
            item->setText(item_new.text);
            item->setData(item_new.sort_data, SORT_ROLE);
            item->setData(item_new.data, VALUE_ROLE);
        }
    }
    emit modelUpdated();
    invalidateFilter();
}

void HBDataModel::addFilter(const HBDataModelFilter& filter) {
    d->filters << filter;
    invalidateFilter();
}

void HBDataModel::setFilters(const HBDataModelFilterList& filters) {
    d->filters = filters;
    invalidateFilter();
}

void HBDataModel::clearFilters() {
    d->filters.clear();
    invalidateFilter();
}

HBDataModelFilterList HBDataModel::filters() const {
    return d->filters;
}

void HBDataModel::setHeaderLabels(const QStringList& labels) {
    d->model->setHorizontalHeaderLabels(labels);
}

bool HBDataModel::filterAcceptsRow(int source_row, const QModelIndex &) const {
    bool result = true;
    int i_value;
    float f_value;
    QDate date;
    QString text;
    QRegExp pattern;
    pattern.setPatternSyntax(QRegExp::WildcardUnix);

    for (const HBDataModelFilter& filter : d->filters) {
        switch (filter.type) {
        case HBDataModelFilter::FILTER_TEXT:
            pattern.setPattern(filter.value.toString());
            text = d->model->item(source_row, filter.column)->text();
            result &= pattern.exactMatch(text);
            break;

        case HBDataModelFilter::FILTER_GE_DATE:
            date = d->model->item(source_row, filter.column)->data(VALUE_ROLE).toDate();
            result &= (date >= filter.value.toDate());
            break;

        case HBDataModelFilter::FILTER_LE_DATE:
            date = d->model->item(source_row, filter.column)->data(VALUE_ROLE).toDate();
            result &= (date <= filter.value.toDate());
            break;

        case HBDataModelFilter::FILTER_GE_FLOAT:
            f_value = d->model->item(source_row, filter.column)->data(VALUE_ROLE).toFloat();
            result &= (f_value >= filter.value.toFloat());
            break;

        case HBDataModelFilter::FILTER_LE_FLOAT:
            f_value = d->model->item(source_row, filter.column)->data(VALUE_ROLE).toFloat();
            result &= (f_value <= filter.value.toFloat());
            break;

        case HBDataModelFilter::FILTER_GE_INT:
            i_value = d->model->item(source_row, filter.column)->data(VALUE_ROLE).toInt();
            result &= (i_value >= filter.value.toInt());
            break;

        case HBDataModelFilter::FILTER_LE_INT:
            i_value = d->model->item(source_row, filter.column)->data(VALUE_ROLE).toInt();
            result &= (i_value <= filter.value.toInt());
            break;
        }
        if (!result) break;
    }
    return result;
}
