#pragma once

#include <QStyledItemDelegate>

namespace Schematics::Ui::Dialogs
{


class EditDelegate: public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit EditDelegate(QObject* parent = nullptr);
    ~EditDelegate() override;

    // QAbstractItemDelegate interface
public:
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;
};

}
