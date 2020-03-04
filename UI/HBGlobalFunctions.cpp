#include "HBGlobalFunctions.h"
#include <QMessageBox>

void HBGlobalFunctions::message_not_implemented(QWidget * p) {
    QMessageBox msg(QMessageBox::Information, p->tr("Not Implemented"), p->tr("This resource is still not implemented!"), QMessageBox::Close, p);
    msg.exec();
}

void HBGlobalFunctions::message_file_exists(QWidget * p) {
    QMessageBox msg(QMessageBox::Warning, p->tr("File not Created"), p->tr("The selected file already exists!"), QMessageBox::Close, p);
    msg.exec();
}

void HBGlobalFunctions::message_error_inserting_data(QWidget * p) {
    QMessageBox msg(QMessageBox::Warning, p->tr("Data not Inserted"), p->tr("Error inserting data into the database!"), QMessageBox::Close, p);
    msg.exec();
}

void HBGlobalFunctions::message_error_updating_data(QWidget * p) {
    QMessageBox msg(QMessageBox::Warning, p->tr("Data not Updated"), p->tr("Error updating data into the database!"), QMessageBox::Close, p);
    msg.exec();
}

void HBGlobalFunctions::message_error_removing_data(QWidget * p) {
    QMessageBox msg(QMessageBox::Warning, p->tr("Data not Removed"), p->tr("Error removing data from the database!"), QMessageBox::Close, p);
    msg.exec();
}

bool HBGlobalFunctions::message_confirmation(QWidget * p) {
    QMessageBox msg(QMessageBox::Question, p->tr("Confirmation"), p->tr("This operation can't be undone. Are you sure?"), QMessageBox::Yes|QMessageBox::Cancel, p);
    return msg.exec()==QMessageBox::Yes;
}

QString HBGlobalFunctions::date_format() {
    return QObject::tr("yyyy/MM/dd");
}
