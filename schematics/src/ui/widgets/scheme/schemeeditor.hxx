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

        double minDiam() const;
        double maxDiam() const;
        double dwsSaw() const;
        double pkaSaw() const;
        bool isVertical() const;

        void setParams(double min_diam, double max_diam,
                       double dws_saw, double pka_saw,
                       bool is_vertical);

    signals:
        void schemeParamChanged();

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
    private /* types */:
        struct BoardParams
        {
            QCheckBox* enable = nullptr;
            QDoubleSpinBox* width = nullptr;
            QDoubleSpinBox* height = nullptr;
        };
    private /* methods */:
        void buildView();

        void bindButton(QPushButton* btn, void (SchemeEditor::* method)()) const;
        void bindEditor(QDoubleSpinBox* box, void (SchemeEditor::* method)()) const;
        void bindEditor(QDoubleSpinBox* box, void (SchemeEditor::* method)(double)) const;
        void bindChkBox(QCheckBox* box, void (SchemeEditor::* method)()) const;

        void bindGroup(const BoardParams& group,
                       void (SchemeEditor::* method)()) const;
        void bindGroup(QCheckBox* enable,
                       QDoubleSpinBox* width,
                       QDoubleSpinBox* height,
                       void (SchemeEditor::* method)()) const;

        void setBoardParams(BoardParams& group, bool enabled, double width, double height);

    private /* members */:
        QDoubleSpinBox *param_minDiam = nullptr;
        QDoubleSpinBox *param_maxDiam = nullptr;
        QDoubleSpinBox *param_dwsGap = nullptr;
        QDoubleSpinBox *param_pkaGap = nullptr;
        QCheckBox* param_pu2 = nullptr;

        QDoubleSpinBox *scheme_dws350_width = nullptr;
        QDoubleSpinBox *scheme_dws350_height = nullptr;

        BoardParams pa300;

        BoardParams pka350;

        BoardParams pa350;
    };
} // namespace Schematics::Ui::Widgets

#endif //SCHEMATICS_SCHEMEEDITOR_HXX
