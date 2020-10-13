#include "tabledialog.hxx"

#include <QLineEdit>
#include <QTableView>
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
        box->addWidget(table);
        {
            buttons = new QDialogButtonBox{
                QDialogButtonBox::Ok |
                QDialogButtonBox::Save |
                QDialogButtonBox::Cancel
            };

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

    connect(ui->filter, &QLineEdit::textChanged,
            this, &TableDialog::doUpdateFilter);
    connect(this, &TableDialog::rejected,
            this, &TableDialog::doCancel);
    connect(this, &TableDialog::accepted,
            this, &TableDialog::doSaveAndClose);

    connect(ui->buttons, &QDialogButtonBox::accepted,
            this, &TableDialog::accept);
//    connect(ui->buttons, &QDialogButtonBox::clicked,
//            this, &TableDialog::);
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
    qDebug() << "save";
}

void TableDialog::doSaveAndClose()
{
    qDebug() << "save && close";
}

void TableDialog::doCancel()
{
    qDebug() << "cancel";
}

void TableDialog::doUpdateFilter(const QString &text)
{
    qDebug() << text;
}

} // namespace Schematics::Ui::Dialogs
