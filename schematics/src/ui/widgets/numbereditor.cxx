#include "numbereditor.hxx"

namespace Schematics::Ui::Widgets
{
    NumberEditor::NumberEditor(QWidget *parent) : QDoubleSpinBox(parent) {

    }

    QValidator::State NumberEditor::validate(QString &input, int &pos) const {
        auto point = locale().decimalPoint();
        auto dot = (point == ',') ? '.' : ',';
        input.replace(dot, point);
        return QDoubleSpinBox::validate(input, pos);
    }

    NumberEditor::~NumberEditor() = default;
} // namespace Schematics::Ui::Widgets
