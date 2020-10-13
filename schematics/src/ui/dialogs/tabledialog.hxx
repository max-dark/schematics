#pragma once

#include <QDialog>


#include <services/settingstable.hxx>

namespace Schematics::Ui::Dialogs
{

class TableDialog: public QDialog
{
    Q_OBJECT
public:
    explicit TableDialog(QWidget* parent = nullptr);
    ~TableDialog() override;
    void setData(Schematics::Service::SettingsTable* model);
private slots:
    void doSave();
    void doSaveAndClose();
    void doCancel();
    void doUpdateFilter();
private:
    struct View;
    View * ui = nullptr;

    Schematics::Service::SettingsTable* data = nullptr;
};

}
