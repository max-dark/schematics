#include "mainwindow.hxx"

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
#include <QDebug>

#include <ui/tools/tool.hxx>

namespace Schematics {

    namespace Ui {

        struct MainView {
            QLabel *lbl_status = nullptr;
            QGraphicsView *schemeView = nullptr;
            QGraphicsScene *scheme = nullptr;
            QGraphicsEllipseItem *circle = nullptr;
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

            void buildView(QMainWindow *self);

        private:
            QWidget *createEditorTab();

            static QDoubleSpinBox *addSizeEditor(QGridLayout *box, const QString &title);
        };

        void MainView::buildView(QMainWindow *self) {
            self->setCentralWidget(new QWidget);
            self->setWindowTitle("Schematics PoC Demo");
            auto mainBox = new QVBoxLayout;

            // add top panel
            {
                auto topBox = new QHBoxLayout;

                lbl_status = new QLabel{QString::fromUtf8(u8"test")};
                topBox->addWidget(lbl_status);
                mainBox->addLayout(topBox);
            }
            // add tabs
            {
                auto tabList = new QTabWidget;
                {
                    QWidget *schemeTab = createEditorTab();

                    tabList->addTab(schemeTab, QString::fromUtf8(u8"Схема раскроя"));
                }

                mainBox->addWidget(tabList);
            }
            self->centralWidget()->setLayout(mainBox);
        }

        QWidget *MainView::createEditorTab() {
            auto schemeTab = new QWidget;
            auto schemeBox = new QHBoxLayout;

            scheme = new QGraphicsScene{};
            scheme->setBackgroundBrush(Qt::white);

            circle = scheme->addEllipse(0, 0, 200, 200);
            circle->moveBy(-100, -100);
            QPen c_pen{};
            c_pen.setColor(Qt::black);
            c_pen.setStyle(Qt::DashLine);
            c_pen.setWidth(5);
            circle->setPen(c_pen);

            schemeView = new QGraphicsView();
            schemeView->setScene(scheme);

            auto editGroup = new QGroupBox{QString::fromUtf8(u8"Редактор схем")};
            {
                auto editBox = new QGridLayout;
                editGroup->setLayout(editBox);

                btn_newScheme = new QPushButton{"Новая"};
                btn_loadScheme = new QPushButton{"Загрузить"};
                btn_saveScheme = new QPushButton{"Сохранить"};

                editBox->addWidget(btn_newScheme, 0, 0);
                editBox->addWidget(btn_loadScheme, 0, 1);
                editBox->addWidget(btn_saveScheme, 0, 2);

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

                btn_calcScheme = new QPushButton{"Рассчитать координаты"};
                tool::addGridRow(editBox, btn_calcScheme);
                btn_applyScheme = new QPushButton{"Выставить оборудование"};
                tool::addGridRow(editBox, btn_applyScheme);
                tool::addGridRow(editBox, tool::createVSpace());
            }

            schemeBox->addWidget(schemeView);
            schemeBox->addWidget(editGroup);
            schemeTab->setLayout(schemeBox);
            return schemeTab;
        }

        QDoubleSpinBox *MainView::addSizeEditor(QGridLayout *box, const QString &title) {
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
    }

    MainWindow::MainWindow(QWidget *parent)
            : QMainWindow(parent), ui{new Ui::MainView()} {
        ui->buildView(this);
        setWindowState(Qt::WindowMaximized);
    }

    MainWindow::~MainWindow() {
        delete ui;
    }

} // namespace Schematics
