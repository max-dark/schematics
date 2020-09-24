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

#include <QMenu>
#include <QAction>

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

    void SchemeEditor::bindGroup(const SchemeEditor::BoardParams &group, void (SchemeEditor::*method)()) const
    {
        bindGroup(group.enable, group.width, group.height, method);
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

    void SchemeEditor::setBoardParams(SchemeEditor::BoardParams &group, bool enabled, double width, double height)
    {
        QSignalBlocker block_signals{this};
        group.enable->setChecked(enabled);
        group.width->setValue(width);
        group.height->setValue(height);
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

    void SchemeEditor::setParams(double min_diam, double max_diam, double dws_saw, double pka_saw, bool is_vertical)
    {
        {
            QSignalBlocker block_signals{this};

            param_minDiam->setValue(min_diam);
            param_maxDiam->setValue(max_diam);
            param_dwsGap->setValue(dws_saw);
            param_pkaGap->setValue(pka_saw);
            param_pu2->setChecked(is_vertical);
        }
        emit schemeParamChanged();
    }

    void SchemeEditor::setDWS350(double width, double height)
    {
        scheme_dws350_width->setValue(width);
        scheme_dws350_height->setValue(height);
    }

    void SchemeEditor::setPA300(bool enabled, double width, double height)
    {
        setBoardParams(pa300, enabled, width, height);
        on_pa300Changed();
    }

    void SchemeEditor::setPKA350(bool enabled, double width, double height)
    {
        setBoardParams(pka350, enabled, width, height);
        on_pka350Changed();
    }

    void SchemeEditor::setPA350(bool enabled, double width, double height)
    {
        setBoardParams(pa350, enabled, width, height);
        on_pa350Changed();
    }

    void SchemeEditor::clearAll()
    {
        setDWS350(0.0, 0.0);
        setPA300(false, 0.0, 0.0);
        setPKA350(false, 0.0, 0.0);
        setPA350(false, 0.0, 0.0);
    }

    void SchemeEditor::buildView() {
        using namespace Schematics::Ui;

        setTitle("Редактор схем");
        auto editBox = new QGridLayout;
        setLayout(editBox);

        setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Minimum);

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
            auto btn_del_dws350 = new QPushButton{"Удалить"};
            auto btn_set_dws350 = new QPushButton{"Заменить"};

            // меню для кнопки "Добавить"
            {
                auto add_menu = new QMenu{};
                for (size_t n = 1; n <= 5; ++n) {
                    auto add_n = add_menu->addAction(QString{"%1 шт"}.arg(n));
                    connect(add_n, &QAction::triggered, [this, n]{
                        on_addNBoardsClicked(n);
                    });
                }

                btn_add_dws350->setMenu(add_menu);
                // при установке меню на кнопку владелец меню не меняется
                // поэтому подцепим удаление меню на уничтожение кнопки
                connect(btn_add_dws350, &QPushButton::destroyed, add_menu, &QAction::deleteLater);
            }
            // меню кнопки "удалить"
            {
                auto del_menu = new QMenu{};
                auto del_all = del_menu->addAction("Все");
                auto del_pos = del_menu->addAction("По позиции");

                connect(del_all, &QAction::triggered, this, &SchemeEditor::deleteAllCentralBoards);
                connect(del_pos, &QAction::triggered, this, &SchemeEditor::deleteCentralBoardByPos);

                btn_del_dws350->setMenu(del_menu);
                connect(btn_del_dws350, &QPushButton::destroyed, del_menu, &QAction::deleteLater);
            }
            // Меню для кнопки "Заменить"
            {
                auto set_menu = new QMenu{};
                auto set_all = set_menu->addAction("Все");
                auto set_pos = set_menu->addAction("По позиции");

                connect(set_all, &QAction::triggered, this, &SchemeEditor::setAllCentralHeights);
                connect(set_pos, &QAction::triggered, this, &SchemeEditor::setCentralHeightByPos);

                btn_set_dws350->setMenu(set_menu);
                connect(btn_set_dws350, &QPushButton::destroyed, set_menu, &QAction::deleteLater);
            }
            auto next_row = box->rowCount();
            box->addWidget(btn_add_dws350, next_row, 0);
            box->addWidget(btn_del_dws350, next_row, 1);
            box->addWidget(btn_set_dws350, next_row, 2);

            pa300.enable = new QCheckBox{"Боковые"};
            tool::addGridRow(box, pa300.enable);
            pa300.width = addSizeEditor(box, "Ширина");
            pa300.height = addSizeEditor(box, "Толщина");

            bindGroup(pa300, &SchemeEditor::on_pa300Changed);

            tool::addGridRow(editBox, dws_group);
        }

        auto pka_group = new QGroupBox{"PKA350/PA350"};
        {
            auto box = new QGridLayout;
            pka_group->setLayout(box);

            pka350.enable = new QCheckBox{"Внутренние"};
            tool::addGridRow(box, pka350.enable);
            pka350.width = addSizeEditor(box, "Ширина");
            pka350.height = addSizeEditor(box, "Толщина");

            bindGroup(pka350, &SchemeEditor::on_pka350Changed);

            pa350.enable = new QCheckBox{"Внешние"};
            tool::addGridRow(box, pa350.enable);
            pa350.width = addSizeEditor(box, "Ширина");
            pa350.height = addSizeEditor(box, "Толщина");

            bindGroup(pa350, &SchemeEditor::on_pa350Changed);

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

    void SchemeEditor::on_addBoardClicked()
    {
        on_addNBoardsClicked(1);
    }

    void SchemeEditor::on_addNBoardsClicked(size_t count)
    {
        auto width = scheme_dws350_width->value();
        auto height = scheme_dws350_height->value();

        if (width > 0 && height > 0)
        {
            emit addCentralBoards(height, count);
        }
    }

    void SchemeEditor::on_pa300Changed() {
        auto width = pa300.width->value();
        auto height = pa300.height->value();
        auto is_enabled = pa300.enable->isChecked()
                          && (width > 0.0)
                          && (height > 0.0);

        emit pa300Changed(is_enabled, width, height);
    }

    void SchemeEditor::on_pka350Changed() {
        auto width = pka350.width->value();
        auto height = pka350.height->value();
        auto is_enabled = pka350.enable->isChecked()
                          && (width > 0.0)
                          && (height > 0.0);

        emit pka350Changed(is_enabled, width, height);
    }

    void SchemeEditor::on_pa350Changed() {
        auto width = pa350.width->value();
        auto height = pa350.height->value();
        auto is_enabled = pa350.enable->isChecked()
                          && (width > 0.0)
                          && (height > 0.0);

        emit pa350Changed(is_enabled, width, height);
    }

    SchemeEditor::~SchemeEditor() = default;
} // namespace Schematics::Ui::Widgets
