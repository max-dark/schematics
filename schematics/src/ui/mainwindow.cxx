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

namespace Schematics {

    namespace Ui {
        namespace tool {
            auto createVSpace() {
                return new QSpacerItem{10, 10, QSizePolicy::Minimum, QSizePolicy::Expanding};
            }

            auto createHSpace() {
                return new QSpacerItem{10, 10, QSizePolicy::Expanding, QSizePolicy::Minimum};
            }
        }

        struct MainView {
            QLabel *lbl_status = nullptr;
            QGraphicsView *schemeView = nullptr;
            QGraphicsScene *scheme = nullptr;
            QGraphicsEllipseItem *circle = nullptr;
            QPushButton *btn_newScheme = nullptr;
            QPushButton *btn_loadScheme = nullptr;
            QPushButton *btn_saveScheme = nullptr;
            QPushButton *btn_applyParams = nullptr;
            QDoubleSpinBox *scheme_minDiam = nullptr;
            QDoubleSpinBox *scheme_maxDiam = nullptr;
            QDoubleSpinBox *scheme_dwsGap = nullptr;
            QDoubleSpinBox *scheme_pkaGap = nullptr;

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

                    tabList->addTab(schemeTab, QString::fromUtf8(u8"Scheme"));
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
                //editGroup->se
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
                    scheme_minDiam = addSizeEditor(paramBox, "Минимальный");
                    scheme_maxDiam = addSizeEditor(paramBox, "Максимальный");
                    paramBox->addWidget(new QLabel{"Ширина пропила"});
                    scheme_dwsGap = addSizeEditor(paramBox, "DWS");
                    scheme_pkaGap = addSizeEditor(paramBox, "PKA");

                    btn_applyParams = new QPushButton{"Применить"};

                    paramBox->addWidget(btn_applyParams, paramBox->rowCount(), 0, 1, -1);
                    editBox->addWidget(paramGroup, editBox->rowCount(), 0, 1, -1);
                }

                auto dws_group = new QGroupBox{"DWS350/PA300"};
                {
                    auto box = new QGridLayout;
                    dws_group->setLayout(box);

                    editBox->addWidget(dws_group, editBox->rowCount(), 0, 1, -1);
                }
                editBox->addItem(tool::createVSpace(), editBox->rowCount(), 0, -1, -1);
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
