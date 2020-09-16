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
    public /* methods */:
        explicit SchemeEditor(QWidget* parent = nullptr);
        ~SchemeEditor() override;

    signals:
        void newScheme();
        void loadScheme();
        void saveScheme();

        void schemeParamChanged();
        void applySchemeParams();

        void centralWidthChanged(double width);
        void addCentralBoard(double width, double height);

        void pa300Changed(bool enabled, double width, double height);
        void pka350Changed(bool enabled, double width, double height);
        void pa350Changed(bool enabled, double width, double height);

        void calculateSchemeCoords();
        void applySchemeCoords();

    private slots:
        void on_addBoardClicked();
        void on_pa300Changed();
        void on_pka350Changed();
        void on_pa350Changed();
    private /* methods */:
        void buildView();

        void bindButton(QPushButton* btn, void (SchemeEditor::* method)()) const;
        void bindEditor(QDoubleSpinBox* box, void (SchemeEditor::* method)()) const;
        void bindEditor(QDoubleSpinBox* box, void (SchemeEditor::* method)(double)) const;
        void bindChkBox(QCheckBox* box, void (SchemeEditor::* method)()) const;

        void bindGroup(QCheckBox* enable,
                       QDoubleSpinBox* width,
                       QDoubleSpinBox* height,
                       void (SchemeEditor::* method)()) const;

    private /* members */:
        QDoubleSpinBox *param_minDiam = nullptr;
        QDoubleSpinBox *param_maxDiam = nullptr;
        QDoubleSpinBox *param_dwsGap = nullptr;
        QDoubleSpinBox *param_pkaGap = nullptr;
        QCheckBox* param_pu2 = nullptr;

        QDoubleSpinBox *scheme_dws350_width = nullptr;
        QDoubleSpinBox *scheme_dws350_height = nullptr;

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
