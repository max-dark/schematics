#include "editdelegate.hxx"

#include <ui/widgets/numbereditor.hxx>

#include <QSpinBox>

#include <QModelIndex>
#include <QVariant>
#include <QtDebug>

namespace Schematics::Ui::Dialogs
{

using Schematics::Ui::Widgets::NumberEditor;


static auto dataOf(const QModelIndex& index)
{
    return index.data(Qt::EditRole);
}

static auto typeOf(const QModelIndex& index)
{
    return dataOf(index).type();
}

static auto createInteger(QWidget* parent)
{
    auto edit = new QSpinBox{parent};
    constexpr auto limit = 200000;
    edit->setRange(-limit, +limit);
    edit->setAlignment(Qt::AlignRight);
    return edit;
}

static auto createDouble(QWidget* parent)
{
    auto edit = new NumberEditor{parent};
    constexpr auto limit = 200.0;
    edit->setRange(-limit, +limit);
    edit->setSuffix({});
    edit->setDecimals(2);
    return edit;
}

template<class EditorT, typename DataT>
static void setEditorValue(QWidget* editor, DataT value)
{
    static_cast<EditorT*>(editor)->setValue(value);
}

template<class EditorT>
static void setModelValue(QWidget* editor,
                          QAbstractItemModel *model,
                          const QModelIndex &index)
{
    auto edit = static_cast<EditorT*>(editor);
    edit->interpretText();

    auto value = edit->value();
    model->setData(index, value, Qt::EditRole);
}

EditDelegate::EditDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{
}

EditDelegate::~EditDelegate()
{}

QWidget *EditDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    qDebug() << typeOf(index);
    switch (typeOf(index)) {
    case QVariant::Int:
    case QVariant::LongLong:
        return createInteger(parent);
    case QVariant::Double:
        return createDouble(parent);
    default:
        break;
    }
    return QStyledItemDelegate::createEditor(parent, option, index);
}

void EditDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    switch (typeOf(index))
    {
    case QVariant::Int:
    case QVariant::LongLong:
        setEditorValue<QSpinBox, int>(editor, dataOf(index).toInt());
        break;
    case QVariant::Double:
        setEditorValue<NumberEditor, double>(editor, dataOf(index).toDouble());
        break;
    default:
        QStyledItemDelegate::setEditorData(editor, index);
        break;
    }
}

void EditDelegate::setModelData(QWidget *editor,
                                QAbstractItemModel *model,
                                const QModelIndex &index) const
{
    switch (typeOf(index))
    {
    case QVariant::Int:
    case QVariant::LongLong:
        setModelValue<QSpinBox>(editor, model, index);
        break;
    case QVariant::Double:
        setModelValue<NumberEditor>(editor, model, index);
        break;
    default:
        QStyledItemDelegate::setModelData(editor, model, index);
        break;
    }
}

} // namespace Schematics::Ui::Dialogs
