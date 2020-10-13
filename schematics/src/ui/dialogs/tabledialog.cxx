#include "tabledialog.hxx"

#include <QLineEdit>
#include <QTableView>
#include <QHeaderView>
#include <QLabel>
#include <QLayout>
#include <QDialogButtonBox>
#include <QPushButton>

#include <QDebug>

namespace Schematics::Ui::Dialogs
{

struct TableDialog::View
{
    QLineEdit * filter = nullptr;
    QTableView* table = nullptr;
    QDialogButtonBox* buttons = nullptr;

    void buildUi(TableDialog* self)
    {
        auto box = new QVBoxLayout;
        {
            auto top = new QHBoxLayout;
            auto label = new QLabel{"Найти по описанию"};
            filter = new QLineEdit{};
            auto button = new QPushButton{"Очистить"};
            QObject::connect(button, &QPushButton::clicked,
                             filter, &QLineEdit::clear);
            top->addWidget(label);
            top->addWidget(filter);
            top->addWidget(button);
            box->addLayout(top);
        }
        table = new QTableView{};
        table->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
        box->addWidget(table);
        {
            buttons = new QDialogButtonBox{};
            auto ok_btn = buttons->addButton("Ok", QDialogButtonBox::ActionRole);
            auto save = buttons->addButton("Сохранить", QDialogButtonBox::ActionRole);
            buttons->addButton(QDialogButtonBox::Cancel)->setText("Отмена");

            QObject::connect(ok_btn, &QAbstractButton::clicked,
                             buttons, &QDialogButtonBox::accepted);
            QObject::connect(save, &QAbstractButton::clicked,
                             self, &TableDialog::doSave);

            QObject::connect(buttons, &QDialogButtonBox::accepted,
                    self, &TableDialog::accept);
            QObject::connect(buttons, &QDialogButtonBox::rejected,
                    self, &TableDialog::reject);

            box->addWidget(buttons);
        }
        self->setLayout(box);
    }
};

TableDialog::TableDialog(QWidget *parent)
    : QDialog(parent)
    , ui{new View}
{
    ui->buildUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    setMinimumSize(600, 400);
    resize(700, 800);

    connect(ui->filter, &QLineEdit::textChanged,
            this, &TableDialog::doUpdateFilter);
    connect(this, &TableDialog::rejected,
            this, &TableDialog::doCancel);
    connect(this, &TableDialog::accepted,
            this, &TableDialog::doSave);
}

TableDialog::~TableDialog()
{
    delete ui;
}

void TableDialog::setData(Service::SettingsTable *model)
{
    data = model;
    if (data)
    {
        ui->table->setModel(data->table());
        data->setFilter({});
        // показать только последние 3 столбца
        int first_visible = data->table()->columnCount() - 3;
        int column = 0;
        while (column < first_visible) {
            ui->table->hideColumn(column);
            ++column;
        }
    }
    else {
        ui->table->setModel(nullptr);
    }
}

void TableDialog::doSave()
{
    if (data) data->saveChanges();
}

void TableDialog::doCancel()
{
    if (data) data->cancelChanges();
}

void TableDialog::doUpdateFilter(const QString &text)
{
    if (data) data->setFilter(text);
}

} // namespace Schematics::Ui::Dialogs
