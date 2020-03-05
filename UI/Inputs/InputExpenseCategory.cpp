#include "InputExpenseCategory.h"
#include "ui_InputExpenseCategory.h"
#include <QCompleter>

InputExpenseCategory::InputExpenseCategory(HBDataManager *m, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::InputExpenseCategory)
{
    ui->setupUi(this);
    QCompleter *completer = new QCompleter(m->getValueList<ExpenseCategory>("group", true), this);
    completer->setCompletionMode(QCompleter::InlineCompletion);
    ui->txtGroup->setCompleter(completer);
}

InputExpenseCategory::~InputExpenseCategory() {
    delete ui;
}

void InputExpenseCategory::setData(const ExpenseCategory& d) {
    ui->txtGroup->setText(d.group);
    ui->txtName->setText(d.name);
}

void InputExpenseCategory::getData(ExpenseCategory& d) const {
    d.group = ui->txtGroup->text();
    d.name = ui->txtName->text();
}

ExpenseCategory InputExpenseCategory::getData() const {
    ExpenseCategory d;
    getData(d);
    return d;
}

