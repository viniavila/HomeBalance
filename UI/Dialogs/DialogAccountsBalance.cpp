#include "DialogAccountsBalance.h"
#include "ui_DialogAccountsBalance.h"
#include <QSortFilterProxyModel>
#include <QStandardItemModel>
#include <QLocale>
#include <QSettings>

#define SORT_ROLE Qt::UserRole+2

class DialogAccountsBalancePrivate {
public:
    DialogAccountsBalance * const parent;
    Ui::DialogAccountsBalance * const ui;
    HBDataManager * m_data;
    QDate last_date;
    QSortFilterProxyModel * model;
    QStandardItemModel * src_model;

    QHash<QString, Account> accounts;
    QHash<QString, float> current_balance;

    DialogAccountsBalancePrivate(DialogAccountsBalance * p, Ui::DialogAccountsBalance * ui) :
        parent(p),
        ui(ui),
        model(new QSortFilterProxyModel(parent)),
        src_model(new QStandardItemModel(parent))
    {
        model->setSourceModel(src_model);
        model->setSortRole(SORT_ROLE);
    }

    ~DialogAccountsBalancePrivate() {

    }

    QStandardItem * create_item(const QString& text, const QVariant& value) {
        QStandardItem * item  = new QStandardItem(text);
        item->setData(value, SORT_ROLE);
        item->setTextAlignment(Qt::AlignCenter);
        item->setFlags(item->flags() ^ Qt::ItemIsEditable);
        return item;
    }

    void update_item(QStandardItem* item, const QString& text, const QVariant& value) {
        item->setText(text);
        item->setData(value, SORT_ROLE);
    }

    void load_accounts() {
        QLocale l;
        for (const Account& c : m_data->getObjectList<Account>()) {
            accounts[c.id] = c;
            current_balance[c.id] = c.initial_balance;
            QList<QStandardItem*> row;
            row << create_item(c.id, c.id);
            row << create_item(m_data->accountTypes().at(c.type), m_data->accountTypes().at(c.type));
            row << create_item(c.name, c.name);
            row << create_item(l.toString(c.initial_balance, 'f', 2), c.initial_balance);
            row << create_item(l.toString(c.initial_balance, 'f', 2), c.initial_balance);
            src_model->appendRow(row);
        }
        src_model->setHorizontalHeaderLabels({ "id", parent->tr("Account Type"), parent->tr("Account Name"), parent->tr("Initial Balance"), parent->tr("Current Balance")});
    }

    void set_last_date(bool today) {
        int m, y;
        if (today) {
            last_date = QDate::currentDate();
            last_date.getDate(&y, &m, nullptr);
            ui->cboMonth->setCurrentIndex(m-1);
            ui->txtYear->setValue(y);
        }
        else {
            y = ui->txtYear->value();
            m = ui->cboMonth->currentIndex()+1;
            last_date.setDate(y, m, 1);
        }
        last_date.setDate(y, m, last_date.daysInMonth());
    }

    void refresh() {
        QLocale l;
        for (const QString& id : current_balance.keys()) {
            current_balance[id] = accounts[id].initial_balance;
        }
        for (const ExpenseTransaction & t : m_data->getObjectList<ExpenseTransaction>()) {
            if (t.transaction_date <= last_date)
                current_balance[t.account_id] -= t.value;
        }
        for (const IncomeTransaction & t : m_data->getObjectList<IncomeTransaction>()) {
            if (t.transaction_date <= last_date)
                current_balance[t.account_id] += t.value;
        }
        for (const TransferTransaction & t : m_data->getObjectList<TransferTransaction>()) {
            if (t.transaction_date <= last_date) {
                current_balance[t.origin_account_id] -= t.value;
                current_balance[t.destination_account_id] += t.value;
            }
        }
        for (int i=0; i < src_model->rowCount(); ++i) {
            QString id = src_model->item(i, 0)->text();
            update_item(src_model->item(i, 4), l.toString(current_balance[id], 'f', 2), current_balance[id]);
        }
    }

    void txtYear_onValueChanged(int) {
        set_last_date(false);
        refresh();
    }

    void cboMonth_onCurrentIndexChanged(int) {
        set_last_date(false);
        refresh();
    }

    void btnToday_onClicked() {
        set_last_date(true);
        refresh();
    }

};

DialogAccountsBalance::DialogAccountsBalance(HBDataManager * m, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogAccountsBalance),
    d(new DialogAccountsBalancePrivate(this, ui))
{
    ui->setupUi(this);
    d->m_data = m;
    d->load_accounts();
    d->set_last_date(true);
    d->refresh();

    ui->view->setModel(d->model);
    ui->view->setColumnHidden(0, true);
    ui->view->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->view->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    ui->view->verticalHeader()->setDefaultSectionSize(fontMetrics().height()*1.5);
    ui->view->sortByColumn(2, Qt::AscendingOrder);
    ui->view->sortByColumn(1, Qt::AscendingOrder);

    QSettings st;
    restoreGeometry(st.value("WindowGeometry/DialogAccountsBalance").toByteArray());

    connect(ui->txtYear, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, std::bind(&DialogAccountsBalancePrivate::txtYear_onValueChanged, d, std::placeholders::_1));
    connect(ui->cboMonth, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, std::bind(&DialogAccountsBalancePrivate::cboMonth_onCurrentIndexChanged, d, std::placeholders::_1));
    connect(ui->btnToday, &QAbstractButton::clicked, this, std::bind(&DialogAccountsBalancePrivate::btnToday_onClicked, d));
}

DialogAccountsBalance::~DialogAccountsBalance() {
    delete d;
    delete ui;
}

void DialogAccountsBalance::done(int r) {
    QSettings st;
    st.setValue("WindowGeometry/DialogAccountsBalance", saveGeometry());
    QDialog::done(r);
}
