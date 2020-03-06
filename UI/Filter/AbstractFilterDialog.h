#ifndef ABSTRACTFILTERDIALOG_H
#define ABSTRACTFILTERDIALOG_H

#include <QDialog>
#include <HBDataModel.h>

class AbstractFilterDialog : public QDialog {
    Q_OBJECT
public:
    inline AbstractFilterDialog(QWidget *parent = nullptr) : QDialog(parent) { }
    virtual void setFilterList(const HBDataModelFilterList& filter_list) = 0;
    virtual HBDataModelFilterList filterList() const = 0;
};

#endif // ABSTRACTFILTERDIALOG_H
