#include "InputIncomeCategory.h"
#include "ui_InputIncomeCategory.h"

InputIncomeCategory::InputIncomeCategory(HBDataManager *, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::InputIncomeCategory)
{
    ui->setupUi(this);
}

InputIncomeCategory::~InputIncomeCategory() {
    delete ui;
}

void InputIncomeCategory::setData(const IncomeCategory& d) {
    ui->txtName->setText(d.name);
}

void InputIncomeCategory::getData(IncomeCategory& d) const {
    d.name = ui->txtName->text();
}

IncomeCategory InputIncomeCategory::getData() const {
    IncomeCategory d;
    getData(d);
    return d;
}
