#include "HBUserInterface.h"
#include "ui_HBUserInterface.h"
#include "HBDataManager.h"
#include "HBGlobalFunctions.h"
#include <QSettings>
#include <QFileInfo>
#include <QFileDialog>
#include <QStandardPaths>
#include <QStringBuilder>
#include <DialogAccounts.h>
#include <DialogExpenseCategories.h>
#include <DialogIncomeCategories.h>
#include <InputIncomeTransaction.h>
#include <InputExpenseTransaction.h>
#include <InputTransferTransaction.h>

class HBUserInterfacePrivate {
public:
    HBUserInterface * const parent;
    Ui::HBUserInterface * const ui;
    HBDataManager * m_data;

    enum SelectedTabView {
        TABVIEW_EXPENSES,
        TABVIEW_INCOMES,
        TABVIEW_TRANSFERS
    };

    HBUserInterfacePrivate(HBUserInterface * p, Ui::HBUserInterface * ui) :
        parent(p),
        ui(ui),
        m_data(new HBDataManager)
    {

    }

    ~HBUserInterfacePrivate() {
        delete m_data;
    }

    void load_last_file() {
        QSettings s;
        QString last_file = s.value("LastFile", QString()).toString();
        if (last_file.size() && QFileInfo(last_file).exists()) {
            m_data->openFileInterface(last_file);
            m_data->setView(HBCOLLECTION_INCOME_TRANSACTIONS, ui->tblIncomes, 6);
            m_data->setView(HBCOLLECTION_EXPENSE_TRANSACTIONS, ui->tblExpenses, 7);
            m_data->setView(HBCOLLECTION_TRANSFER_TRANSACTIONS, ui->tblTransfers, {4, 5});
            parent->setWindowTitle(last_file % " - " % QApplication::desktopFileName() % " " % QApplication::applicationVersion());
        }
    }


    void actAbout_onTriggered() {
        HBGlobalFunctions::message_not_implemented(parent);
    }

    void actAccountsBalance_onTriggered() {
        HBGlobalFunctions::message_not_implemented(parent);
    }

    void actEditSelectedTransaction_onTriggered() {
        switch (ui->tabWidget->currentIndex()) {
        case TABVIEW_EXPENSES:
            tblExpenses_onDoubleClicked(ui->tblExpenses->currentIndex());
            break;

        case TABVIEW_INCOMES:
            tblIncomes_onDoubleClicked(ui->tblIncomes->currentIndex());
            break;

        case TABVIEW_TRANSFERS:
            tblTransfers_onDoubleClicked(ui->tblTransfers->currentIndex());
            break;
        }
    }

    void actExpenseAnalysis_onTriggered() {
        HBGlobalFunctions::message_not_implemented(parent);
    }

    void actExpensesPlanner_onTriggered() {
        HBGlobalFunctions::message_not_implemented(parent);
    }

    void actExport_onTriggered() {
        HBGlobalFunctions::message_not_implemented(parent);
    }

    void actFilterTransactions_onTriggered() {
        HBGlobalFunctions::message_not_implemented(parent);
    }

    void actFinancialPerformance_onTriggered() {
        HBGlobalFunctions::message_not_implemented(parent);
    }

    void actFindTransaction_onTriggered() {
        HBGlobalFunctions::message_not_implemented(parent);
    }

    void actHelpTopics_onTriggered() {
        HBGlobalFunctions::message_not_implemented(parent);
    }

    void actManageAccounts_onTriggered() {
        DialogAccounts(m_data, parent).exec();
    }

    void actManageExpenseCategories_onTriggered() {
        DialogExpenseCategories(m_data, parent).exec();
    }

    void actManageIncomeCategories_onTriggered() {
        DialogIncomeCategories(m_data, parent).exec();
    }

    void actNewDatabase_onTriggered() {
        QSettings s;
        QString filter = parent->tr("Home Balance Database Files") % " *.hbdb";
        QString last_dir = s.value("LastDir", QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation)).toString();
        QString chosen_file;
        QFileInfo f_info;
        do {
            chosen_file = QFileDialog::getSaveFileName(parent, parent->tr("Create a New Database File"), last_dir, filter, &filter);
            if (chosen_file.size()) {
                if (chosen_file.size() < 5 || chosen_file.right(5) != ".hbdb")
                    chosen_file.append(".hbdb");
            }
            else
                break;
            f_info = QFileInfo(chosen_file);
        } while (f_info.exists());
        if (chosen_file.size()) {
            s.setValue("LastFile", f_info.filePath());
            s.setValue("LastDir", f_info.dir().absolutePath());
            m_data->openFileInterface(chosen_file);
            m_data->setView(HBCOLLECTION_INCOME_TRANSACTIONS, ui->tblIncomes, 6);
            m_data->setView(HBCOLLECTION_EXPENSE_TRANSACTIONS, ui->tblExpenses, 7);
            m_data->setView(HBCOLLECTION_TRANSFER_TRANSACTIONS, ui->tblTransfers, {4, 5});
            parent->setWindowTitle(chosen_file % " - " % QApplication::desktopFileName() % " " % QApplication::applicationVersion());
        }
    }

    void actNewTransaction_onTriggered() {
        switch (ui->tabWidget->currentIndex()) {
        case TABVIEW_EXPENSES: {
            InputExpenseTransaction input(m_data, parent);
            if (input.exec()) {
                ExpenseTransaction d = input.getData();
                m_data->insertObject(d);
                ui->tblExpenses->scrollToBottom();
            }
            break;
        }

        case TABVIEW_INCOMES: {
            InputIncomeTransaction input(m_data, parent);
            if (input.exec()) {
                IncomeTransaction d = input.getData();
                m_data->insertObject(d);
                ui->tblIncomes->scrollToBottom();
            }
            break;
        }

        case TABVIEW_TRANSFERS: {
            InputTransferTransaction input(m_data, parent);
            if (input.exec()) {
                TransferTransaction d = input.getData();
                m_data->insertObject(d);
                ui->tblTransfers->scrollToBottom();
            }
            break;
        }
        }
    }

    void actOpenDatabase_onTriggered() {
        QSettings s;
        QString filter = parent->tr("Home Balance Database Files") % " *.hbdb";
        QString last_dir = s.value("LastDir", QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation)).toString();
        QString chosen_file = QFileDialog::getOpenFileName(parent, parent->tr("Select an Existing Database File"), last_dir, filter, &filter);
        if (chosen_file.size()) {
            if (chosen_file.size() < 5 || chosen_file.right(5) != ".hbdb")
                chosen_file.append(".hbdb");
            QFileInfo f_info = QFileInfo(chosen_file);
            if (f_info.exists()) {
                s.setValue("LastFile", f_info.filePath());
                s.setValue("LastDir", f_info.dir().absolutePath());
                m_data->openFileInterface(chosen_file);
                m_data->setView(HBCOLLECTION_INCOME_TRANSACTIONS, ui->tblIncomes, 6);
                m_data->setView(HBCOLLECTION_EXPENSE_TRANSACTIONS, ui->tblExpenses, 7);
                m_data->setView(HBCOLLECTION_TRANSFER_TRANSACTIONS, ui->tblTransfers, {4, 5});
                parent->setWindowTitle(chosen_file % " - " % QApplication::desktopFileName() % " " % QApplication::applicationVersion());
            }
        }
    }

    void actRemoveSelectedTransaction_onTriggered() {
        switch (ui->tabWidget->currentIndex()) {
        case TABVIEW_EXPENSES: {
            QModelIndex index = ui->tblExpenses->currentIndex();
            if (index.isValid() && HBGlobalFunctions::message_confirmation(parent))
                m_data->deleteObject<ExpenseTransaction>(index.row());
            break;
        }

        case TABVIEW_INCOMES: {
            QModelIndex index = ui->tblIncomes->currentIndex();
            if (index.isValid() && HBGlobalFunctions::message_confirmation(parent))
                m_data->deleteObject<IncomeTransaction>(index.row());
            break;
        }

        case TABVIEW_TRANSFERS: {
            QModelIndex index = ui->tblTransfers->currentIndex();
            if (index.isValid() && HBGlobalFunctions::message_confirmation(parent))
                m_data->deleteObject<TransferTransaction>(index.row());
            break;
        }
        }
    }

    void actSettings_onTriggered() {
        HBGlobalFunctions::message_not_implemented(parent);
    }

    void tblExpenses_onDoubleClicked(const QModelIndex& index) {
        if (index.isValid()) {
            ExpenseTransaction d = m_data->getObject<ExpenseTransaction>(index.row());
            InputExpenseTransaction dlg(m_data, parent);
            dlg.setWindowTitle(parent->tr("Edit Expense Transaction"));
            dlg.setData(d);
            if (dlg.exec()) {
                dlg.getData(d);
                m_data->updateObject(index.row(), d);
            }
        }
    }

    void tblIncomes_onDoubleClicked(const QModelIndex& index) {
        if (index.isValid()) {
            IncomeTransaction d = m_data->getObject<IncomeTransaction>(index.row());
            InputIncomeTransaction dlg(m_data, parent);
            dlg.setWindowTitle(parent->tr("Edit Income Transaction"));
            dlg.setData(d);
            if (dlg.exec()) {
                dlg.getData(d);
                m_data->updateObject(index.row(), d);
            }
        }
    }

    void tblTransfers_onDoubleClicked(const QModelIndex& index) {
        if (index.isValid()) {
            TransferTransaction d = m_data->getObject<TransferTransaction>(index.row());
            InputTransferTransaction dlg(m_data, parent);
            dlg.setWindowTitle(parent->tr("Edit Transfer Transaction"));
            dlg.setData(d);
            if (dlg.exec()) {
                dlg.getData(d);
                m_data->updateObject(index.row(), d);
            }
        }
    }

};

HBUserInterface::HBUserInterface(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::HBUserInterface),
    d(new HBUserInterfacePrivate(this, ui))
{
    ui->setupUi(this);
    setWindowTitle(QApplication::desktopFileName() % " " % QApplication::applicationVersion());
    d->load_last_file();
    ui->tblExpenses->verticalHeader()->setDefaultSectionSize(fontMetrics().height()*1.5);
    ui->tblIncomes->verticalHeader()->setDefaultSectionSize(fontMetrics().height()*1.5);
    ui->tblTransfers->verticalHeader()->setDefaultSectionSize(fontMetrics().height()*1.5);

    connect(ui->actAbout, &QAction::triggered, this, std::bind(&HBUserInterfacePrivate::actAbout_onTriggered, d));
    connect(ui->actAboutQt, &QAction::triggered, qApp, &QApplication::aboutQt);
    connect(ui->actAccountsBalance, &QAction::triggered, this, std::bind(&HBUserInterfacePrivate::actAccountsBalance_onTriggered, d));
    connect(ui->actEditSelectedTransaction, &QAction::triggered, this, std::bind(&HBUserInterfacePrivate::actEditSelectedTransaction_onTriggered, d));
    connect(ui->actExpenseAnalysis, &QAction::triggered, this, std::bind(&HBUserInterfacePrivate::actExpenseAnalysis_onTriggered, d));
    connect(ui->actExpensesPlanner, &QAction::triggered, this, std::bind(&HBUserInterfacePrivate::actExpensesPlanner_onTriggered, d));
    connect(ui->actExport, &QAction::triggered, this, std::bind(&HBUserInterfacePrivate::actExport_onTriggered, d));
    connect(ui->actFilterTransactions, &QAction::triggered, this, std::bind(&HBUserInterfacePrivate::actFilterTransactions_onTriggered, d));
    connect(ui->actFinancialPerformance, &QAction::triggered, this, std::bind(&HBUserInterfacePrivate::actFinancialPerformance_onTriggered, d));
    connect(ui->actFindTransaction, &QAction::triggered, this, std::bind(&HBUserInterfacePrivate::actFindTransaction_onTriggered, d));
    connect(ui->actHelpTopics, &QAction::triggered, this, std::bind(&HBUserInterfacePrivate::actHelpTopics_onTriggered, d));
    connect(ui->actManageAccounts, &QAction::triggered, this, std::bind(&HBUserInterfacePrivate::actManageAccounts_onTriggered, d));
    connect(ui->actManageExpenseCategories, &QAction::triggered, this, std::bind(&HBUserInterfacePrivate::actManageExpenseCategories_onTriggered, d));
    connect(ui->actManageIncomeCategories, &QAction::triggered, this, std::bind(&HBUserInterfacePrivate::actManageIncomeCategories_onTriggered, d));
    connect(ui->actNewDatabase, &QAction::triggered, this, std::bind(&HBUserInterfacePrivate::actNewDatabase_onTriggered, d));
    connect(ui->actNewTransaction, &QAction::triggered, this, std::bind(&HBUserInterfacePrivate::actNewTransaction_onTriggered, d));
    connect(ui->actOpenDatabase, &QAction::triggered, this, std::bind(&HBUserInterfacePrivate::actOpenDatabase_onTriggered, d));
    connect(ui->actQuit, &QAction::triggered, this, &HBUserInterface::close);
    connect(ui->actRemoveSelectedTransaction, &QAction::triggered, this, std::bind(&HBUserInterfacePrivate::actRemoveSelectedTransaction_onTriggered, d));
    connect(ui->actSettings, &QAction::triggered, this, std::bind(&HBUserInterfacePrivate::actSettings_onTriggered, d));
    connect(ui->tblExpenses, &QTableView::doubleClicked, this, std::bind(&HBUserInterfacePrivate::tblExpenses_onDoubleClicked, d, std::placeholders::_1));
    connect(ui->tblIncomes, &QTableView::doubleClicked, this, std::bind(&HBUserInterfacePrivate::tblIncomes_onDoubleClicked, d, std::placeholders::_1));
    connect(ui->tblTransfers, &QTableView::doubleClicked, this, std::bind(&HBUserInterfacePrivate::tblTransfers_onDoubleClicked, d, std::placeholders::_1));
}

HBUserInterface::~HBUserInterface() {
    delete d;
    delete ui;
}

