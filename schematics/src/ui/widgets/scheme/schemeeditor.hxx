#ifndef SCHEMATICS_SCHEMEEDITOR_HXX
#define SCHEMATICS_SCHEMEEDITOR_HXX

#include <QGroupBox>

QT_BEGIN_NAMESPACE
class QPushButton;
class QDoubleSpinBox;
class QCheckBox;
QT_END_NAMESPACE

namespace Schematics::Ui::Widgets
{
    class SchemeEditor: public QGroupBox
    {
        Q_OBJECT
    public:
        explicit SchemeEditor(QWidget* parent = nullptr);
        ~SchemeEditor() override;

    private /* methods */:
        void buildView();

    private /* members */:
        QPushButton *btn_newScheme = nullptr;
        QPushButton *btn_loadScheme = nullptr;
        QPushButton *btn_saveScheme = nullptr;
        QPushButton *btn_applyParams = nullptr;
        QPushButton *btn_calcScheme = nullptr;
        QPushButton *btn_applyScheme = nullptr;
        QDoubleSpinBox *param_minDiam = nullptr;
        QDoubleSpinBox *param_maxDiam = nullptr;
        QDoubleSpinBox *param_dwsGap = nullptr;
        QDoubleSpinBox *param_pkaGap = nullptr;

        QDoubleSpinBox *scheme_dws350_width = nullptr;
        QDoubleSpinBox *scheme_dws350_height = nullptr;
        QPushButton *btn_add_dws350 = nullptr;

        QCheckBox* chk_pa300_enable = nullptr;
        QDoubleSpinBox *scheme_pa300_width = nullptr;
        QDoubleSpinBox *scheme_pa300_height = nullptr;

        QCheckBox* chk_pka350_enable = nullptr;
        QDoubleSpinBox *scheme_pka350_width = nullptr;
        QDoubleSpinBox *scheme_pka350_height = nullptr;

        QCheckBox* chk_pa350_enable = nullptr;
        QDoubleSpinBox *scheme_pa350_width = nullptr;
        QDoubleSpinBox *scheme_pa350_height = nullptr;
    };
} // namespace Schematics::Ui::Widgets

#endif //SCHEMATICS_SCHEMEEDITOR_HXX
