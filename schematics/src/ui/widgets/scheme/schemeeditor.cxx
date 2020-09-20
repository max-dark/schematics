#include "schemeeditor.hxx"

#include <QLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QVBoxLayout>

#include <QWidget>
#include <QPushButton>
#include <QCheckBox>
#include <QRadioButton>
#include <QDoubleSpinBox>
#include <QLabel>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGroupBox>
#include <QGraphicsEllipseItem>
#include <QSpacerItem>
#include <QSizePolicy>

#include <ui/tools/tool.hxx>

namespace Schematics::Ui::Widgets
{
    QDoubleSpinBox *addSizeEditor(QGridLayout *box, const QString &title) {
        auto lbl = new QLabel{title};
        auto edit = new QDoubleSpinBox{};
        edit->setSuffix(" mm");
        edit->setDecimals(1);
        edit->setAlignment(Qt::AlignRight);
        edit->setMinimum(0.0);
        edit->setMaximum(1000.0);

        auto last_row = box->rowCount();
        box->addWidget(lbl, last_row, 0);
        box->addItem(tool::createHSpace(), last_row, 1);
        box->addWidget(edit, last_row, 2);
        return edit;
    }

    void SchemeEditor::bindButton(QPushButton* btn, void (SchemeEditor::* method)()) const
    {
        QObject::connect(btn, &QPushButton::clicked,
                         this, method);
    }

    void SchemeEditor::bindEditor(QDoubleSpinBox* box, void (SchemeEditor::* method)()) const
    {
        QObject::connect(box, qOverload<double>(&QDoubleSpinBox::valueChanged),
                         this, method);
    }

    void SchemeEditor::bindEditor(QDoubleSpinBox* box, void (SchemeEditor::* method)(double)) const
    {
        QObject::connect(box, qOverload<double>(&QDoubleSpinBox::valueChanged),
                         this, method);
    }

    void SchemeEditor::bindChkBox(QCheckBox* box, void (SchemeEditor::* method)()) const
    {
        QObject::connect(box, &QCheckBox::stateChanged,
                         this, method);
    }

    void SchemeEditor::bindGroup(QCheckBox* enable,
                   QDoubleSpinBox* width,
                   QDoubleSpinBox* height,
                   void (SchemeEditor::* method)()) const
    {
        bindChkBox(enable, method);
        bindEditor(width, method);
        bindEditor(height, method);
    }

    SchemeEditor::SchemeEditor(QWidget *parent) : QGroupBox(parent) {
        buildView();
    }

    double SchemeEditor::minDiam() const
    {
        return param_minDiam->value();
    }

    double SchemeEditor::maxDiam() const
    {
        return param_maxDiam->value();
    }

    double SchemeEditor::dwsSaw() const
    {
        return param_dwsGap->value();
    }

    double SchemeEditor::pkaSaw() const
    {
        return param_pkaGap->value();
    }

    bool SchemeEditor::isVertical() const { return param_pu2->isChecked(); }

    void SchemeEditor::buildView() {
        using namespace Schematics::Ui;

        setTitle("Редактор схем");
        auto editBox = new QGridLayout;
        setLayout(editBox);

        QSizePolicy def_policy{QSizePolicy::Preferred, QSizePolicy::Minimum};

        auto paramGroup = new QGroupBox{"Параметры схемы"};
        {
            auto paramBox = new QGridLayout;
            paramGroup->setLayout(paramBox);
            paramGroup->setSizePolicy(def_policy);

            paramBox->addWidget(new QLabel{"Диаметр"});
            param_minDiam = addSizeEditor(paramBox, "Минимальный");
            param_maxDiam = addSizeEditor(paramBox, "Максимальный");
            paramBox->addWidget(new QLabel{"Ширина пропила"});
            param_dwsGap = addSizeEditor(paramBox, "DWS");
            param_pkaGap = addSizeEditor(paramBox, "PKA");

            for (auto editor: {param_minDiam, param_maxDiam, param_dwsGap, param_pkaGap})
            {
                bindEditor(editor, &SchemeEditor::schemeParamChanged);
            }

            param_pu2 = new QCheckBox{"Отключить ПУ №2"};
            paramBox->addWidget(param_pu2);
            bindChkBox(param_pu2, &SchemeEditor::schemeParamChanged);

            tool::addGridRow(editBox, paramGroup);
        }

        auto dws_group = new QGroupBox{"DWS350/PA300"};
        {
            auto box = new QGridLayout;
            dws_group->setLayout(box);

            tool::addGridRow(box, new QLabel{"Центральные"});
            scheme_dws350_width = addSizeEditor(box, "Ширина");
            scheme_dws350_height = addSizeEditor(box, "Толщина");

            bindEditor(scheme_dws350_width, &SchemeEditor::centralWidthChanged);

            auto btn_add_dws350 = new QPushButton{"Добавить"};
            bindButton(btn_add_dws350, &SchemeEditor::on_addBoardClicked);
            tool::addGridRow(box, btn_add_dws350);

            chk_pa300_enable = new QCheckBox{"Боковые"};
            tool::addGridRow(box, chk_pa300_enable);
            scheme_pa300_width = addSizeEditor(box, "Ширина");
            scheme_pa300_height = addSizeEditor(box, "Толщина");

            bindGroup(chk_pa300_enable,
                      scheme_pa300_width,
                      scheme_pa300_height,
                      &SchemeEditor::on_pa300Changed);

            tool::addGridRow(editBox, dws_group);
        }

        auto pka_group = new QGroupBox{"PKA350/PA350"};
        {
            auto box = new QGridLayout;
            pka_group->setLayout(box);

            chk_pka350_enable = new QCheckBox{"Внутренние"};
            tool::addGridRow(box, chk_pka350_enable);
            scheme_pka350_width = addSizeEditor(box, "Ширина");
            scheme_pka350_height = addSizeEditor(box, "Толщина");

            bindGroup(chk_pka350_enable,
                      scheme_pka350_width,
                      scheme_pka350_height,
                      &SchemeEditor::on_pka350Changed);

            chk_pa350_enable = new QCheckBox{"Внешние"};
            tool::addGridRow(box, chk_pa350_enable);
            scheme_pa350_width = addSizeEditor(box, "Ширина");
            scheme_pa350_height = addSizeEditor(box, "Толщина");

            bindGroup(chk_pa350_enable,
                      scheme_pa350_width,
                      scheme_pa350_height,
                      &SchemeEditor::on_pa350Changed);

            tool::addGridRow(editBox, pka_group);
        }

        // bottom buttons
        {
            auto btn_calcScheme = new QPushButton{"Рассчитать координаты"};
            auto btn_applyScheme = new QPushButton{"Выставить оборудование"};
            tool::addGridRow(editBox, btn_calcScheme);
            tool::addGridRow(editBox, btn_applyScheme);

            bindButton(btn_calcScheme, &SchemeEditor::calculateSchemeCoords);
            bindButton(btn_applyScheme, &SchemeEditor::applySchemeCoords);
        }
        tool::addGridRow(editBox, tool::createVSpace());
    }

    void SchemeEditor::on_addBoardClicked() {
        auto width = scheme_dws350_width->value();
        auto height = scheme_dws350_height->value();

        emit addCentralBoard(width, height);
    }

    void SchemeEditor::on_pa300Changed() {
        auto width = scheme_pa300_width->value();
        auto height = scheme_pa300_height->value();
        auto is_enabled = chk_pa300_enable->isChecked()
                          && (width > 0.0)
                          && (height > 0.0);

        emit pa300Changed(is_enabled, width, height);
    }

    void SchemeEditor::on_pka350Changed() {
        auto width = scheme_pka350_width->value();
        auto height = scheme_pka350_height->value();
        auto is_enabled = chk_pka350_enable->isChecked()
                          && (width > 0.0)
                          && (height > 0.0);

        emit pka350Changed(is_enabled, width, height);
    }

    void SchemeEditor::on_pa350Changed() {
        auto width = scheme_pa350_width->value();
        auto height = scheme_pa350_height->value();
        auto is_enabled = chk_pa350_enable->isChecked()
                          && (width > 0.0)
                          && (height > 0.0);

        emit pa350Changed(is_enabled, width, height);
    }

    SchemeEditor::~SchemeEditor() = default;
} // namespace Schematics::Ui::Widgets
