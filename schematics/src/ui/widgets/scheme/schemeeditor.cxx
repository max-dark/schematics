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

    SchemeEditor::SchemeEditor(QWidget *parent) : QGroupBox(parent) {
        buildView();
    }

    void SchemeEditor::buildView() {
        using namespace Schematics::Ui;
        auto editBox = new QGridLayout;
        setLayout(editBox);

        // create top buttons
        {
            auto btn_newScheme = new QPushButton{"Новая"};
            auto btn_loadScheme = new QPushButton{"Загрузить"};
            auto btn_saveScheme = new QPushButton{"Сохранить"};

            editBox->addWidget(btn_newScheme, 0, 0);
            editBox->addWidget(btn_loadScheme, 0, 1);
            editBox->addWidget(btn_saveScheme, 0, 2);

            connect(btn_newScheme, &QPushButton::clicked, this, &SchemeEditor::newScheme);
            connect(btn_loadScheme, &QPushButton::clicked, this, &SchemeEditor::loadScheme);
            connect(btn_saveScheme, &QPushButton::clicked, this, &SchemeEditor::saveScheme);
        }
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

            btn_applyParams = new QPushButton{"Применить параметры"};

            tool::addGridRow(paramBox, btn_applyParams);
            tool::addGridRow(editBox, paramGroup);
        }

        auto dws_group = new QGroupBox{"DWS350/PA300"};
        {
            auto box = new QGridLayout;
            dws_group->setLayout(box);

            tool::addGridRow(box, new QLabel{"Центральные"});
            scheme_dws350_width = addSizeEditor(box, "Ширина");
            scheme_dws350_height = addSizeEditor(box, "Толщина");
            btn_add_dws350 = new QPushButton{"Добавить"};
            tool::addGridRow(box, btn_add_dws350);

            chk_pa300_enable = new QCheckBox{"Боковые"};
            tool::addGridRow(box, chk_pa300_enable);
            scheme_pa300_width = addSizeEditor(box, "Ширина");
            scheme_pa300_height = addSizeEditor(box, "Толщина");

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

            chk_pa350_enable = new QCheckBox{"Внешние"};
            tool::addGridRow(box, chk_pa350_enable);
            scheme_pa350_width = addSizeEditor(box, "Ширина");
            scheme_pa350_height = addSizeEditor(box, "Толщина");

            tool::addGridRow(editBox, pka_group);
        }

        // bottom buttons
        {
            auto btn_calcScheme = new QPushButton{"Рассчитать координаты"};
            auto btn_applyScheme = new QPushButton{"Выставить оборудование"};
            tool::addGridRow(editBox, btn_calcScheme);
            tool::addGridRow(editBox, btn_applyScheme);

            connect(btn_calcScheme, &QPushButton::clicked, this, &SchemeEditor::calculateSchemeCoords);
            connect(btn_applyScheme, &QPushButton::clicked, this, &SchemeEditor::applySchemeCoords);
        }
        tool::addGridRow(editBox, tool::createVSpace());
    }

    SchemeEditor::~SchemeEditor() = default;
} // namespace Schematics::Ui::Widgets