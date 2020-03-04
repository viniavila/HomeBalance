#ifndef HBGLOBALFUNCTIONS_H
#define HBGLOBALFUNCTIONS_H

#include <QString>
class QWidget;

class HBGlobalFunctions {
public:
    static void message_not_implemented(QWidget * parent);
    static void message_file_exists(QWidget * parent);
    static void message_error_inserting_data(QWidget * parent);
    static void message_error_updating_data(QWidget * parent);
    static void message_error_removing_data(QWidget * parent);
    static bool message_confirmation(QWidget * parent);
    static QString date_format();
};

#endif // HBGLOBALFUNCTIONS_H
