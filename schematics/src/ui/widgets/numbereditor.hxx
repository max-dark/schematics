#ifndef SCHEMATICS_NUMBEREDITOR_HXX
#define SCHEMATICS_NUMBEREDITOR_HXX

#include <QDoubleSpinBox>

namespace Schematics::Ui::Widgets
{
    class NumberEditor: public QDoubleSpinBox
    {
    public:
        explicit NumberEditor(QWidget *parent = nullptr);
        ~NumberEditor() override;
        QValidator::State validate(QString &input, int &pos) const override;
    };
} // namespace Schematics::Ui::Widgets

#endif //SCHEMATICS_NUMBEREDITOR_HXX
