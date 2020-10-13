#include "tabledialog.hxx"

#include <QLineEdit>
#include <QTableView>
#include <QLabel>
#include <QLayout>
#include <QDialogButtonBox>
#include <QPushButton>

namespace Schematics::Ui::Dialogs
{

struct TableDialog::View
{
    QLineEdit * filter = nullptr;
    QTableView* table = nullptr;

    void buildUi(TableDialog* self)
    {
        auto box = new QVBoxLayout;
        {
            auto top = new QHBoxLayout;
            auto label = new QLabel{"Найти по описанию"};
            filter = new QLineEdit{};
            auto button = new QPushButton{"Очистить"};
            top->addWidget(label);
            top->addWidget(filter);
            top->addWidget(button);
            box->addLayout(top);
        }
        table = new QTableView{};
        box->addWidget(table);
        {
            auto buttons = new QDialogButtonBox{};

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
}

TableDialog::~TableDialog()
{
    delete ui;
}

void TableDialog::setData(Service::SettingsTable *model)
{
    data = model;
}

void TableDialog::doSave()
{
    //
}

void TableDialog::doSaveAndClose()
{
    //
}

void TableDialog::doCancel()
{
    //
}

void TableDialog::doUpdateFilter()
{
    //
}

} // namespace Schematics::Ui::Dialogs
