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
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QFileDialog>

#include <QMessageBox>

#include <ui/widgets/scheme/schemeview.hxx>
#include <ui/widgets/scheme/schemeeditor.hxx>

#include <ui/widgets/coords/coordstab.hxx>

#include <schema/schema.hxx>
#include <schema/params.hxx>
#include <schema/units.hxx>

#include <schema/xmlwriter.hxx>
#include <schema/xmlreader.hxx>

using libschema::Unit;

namespace Schematics {

    static const QString scheme_filter{"Cutting scheme (*.cut)"};

    namespace Ui {

        struct MainView {
            QAction* scheme_new = nullptr;
            QAction* scheme_load = nullptr;
            QAction* scheme_save = nullptr;

            QLabel *lbl_status = nullptr;

            QTabWidget* tabList = nullptr;

            Widgets::SchemeView *schemeView = nullptr;
            Widgets::SchemeEditor* schemeEditor = nullptr;

            Widgets::CoordsTab* coordsTab = nullptr;

            void buildView(QMainWindow *self);

        private:
            QWidget *createEditorTab();
            QMenuBar *createMainMenu();

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
                tabList = new QTabWidget;
                {
                    auto schemeTab = createEditorTab();
                    coordsTab = new Widgets::CoordsTab{};

                    tabList->addTab(schemeTab, QString::fromUtf8("Схема раскроя"));
                    tabList->addTab(coordsTab, QString::fromUtf8("Координаты оборудования"));
                }

                mainBox->addWidget(tabList);
            }

            // add main menu
            {
                auto menuBar = createMainMenu();
                self->setMenuBar(menuBar);
            }

            self->centralWidget()->setLayout(mainBox);
        }

        QWidget *MainView::createEditorTab() {
            auto schemeTab = new QWidget;
            auto schemeBox = new QHBoxLayout;

            schemeView = new Widgets::SchemeView();

            schemeEditor = new Widgets::SchemeEditor{};


            schemeBox->addWidget(schemeView);
            schemeBox->addWidget(schemeEditor);
            schemeTab->setLayout(schemeBox);
            return schemeTab;
        }

        QMenuBar *MainView::createMainMenu()
        {
            auto mainMenu = new QMenuBar{};
            {
                auto schemeMenu = mainMenu->addMenu("Схема");

                scheme_new = new QAction{"Новая"};
                scheme_load = new QAction{"Загрузить"};
                scheme_save = new QAction{"Сохранить"};

                schemeMenu->addAction(scheme_new);
                schemeMenu->addAction(scheme_load);
                schemeMenu->addAction(scheme_save);
            }

            return mainMenu;
        }

    }

    MainWindow::MainWindow(QWidget *parent)
            : QMainWindow(parent), ui{new Ui::MainView()} {
        ui->buildView(this);
        setWindowState(Qt::WindowMaximized);
        bindEvents();

        auto params = new libschema::Params{};
        scheme = new libschema::Schema{this};
        scheme->set_params(params);

        ui->schemeEditor->setParams(
            250, 250,
            4.8, 5.6,
            false);
    }

    MainWindow::~MainWindow() {
        delete ui;
    }

    void MainWindow::bindEvents() {
        bindMenus();
        bindSchemeEditor();
    }

    void MainWindow::bindMenus()
    {
        connect(ui->scheme_new, &QAction::triggered,
                this, &MainWindow::on_newScheme);
        connect(ui->scheme_load, &QAction::triggered,
                this, &MainWindow::on_loadScheme);
        connect(ui->scheme_save, &QAction::triggered,
                this, &MainWindow::on_saveScheme);
    }

    void MainWindow::bindSchemeEditor() {
        using Schematics::Ui::Widgets::SchemeEditor;

        connect(ui->schemeEditor, &SchemeEditor::schemeParamChanged,
                this, &MainWindow::schemeParamChanged);
        connect(ui->schemeEditor, &SchemeEditor::centralWidthChanged,
                this, &MainWindow::centralWidthChanged);
        connect(ui->schemeEditor, &SchemeEditor::addCentralBoard,
                this, &MainWindow::addCentralBoard);
        connect(ui->schemeEditor, &SchemeEditor::pa300Changed,
                this, &MainWindow::pa300Changed);
        connect(ui->schemeEditor, &SchemeEditor::pka350Changed,
                this, &MainWindow::pka350Changed);
        connect(ui->schemeEditor, &SchemeEditor::pa350Changed,
                this, &MainWindow::pa350Changed);
    }

    void MainWindow::on_newScheme() {
        ui->schemeView->clear();
        ui->schemeEditor->clearAll();
        scheme->remove_all();
    }

    void MainWindow::on_loadScheme()
    {
        auto fname = QFileDialog::getOpenFileName(
            this, "Загрузка: Выберите файл схемы раскроя", "",
            scheme_filter);
        if (!fname.isEmpty())
        {
            QFile input{fname};
            auto ok = input.open(QIODevice::ReadOnly);

            if(ok)
            {
                libschema::XmlReader reader;
                reader.read(nullptr, input);
            }
        }
    }

    void MainWindow::on_saveScheme()
    {
        auto fname = QFileDialog::getSaveFileName(
            this, "Сохранение: Выберите файл схемы раскроя", "",
            scheme_filter);
        if (!fname.isEmpty())
        {
            QFile output{fname};
            auto ok = output.open(QIODevice::WriteOnly);
            if (ok)
            {
                libschema::XmlWriter writer;
                writer.write(scheme, output);
                if (writer.hasError())
                {
                    QMessageBox::critical(
                        this,
                        "Ошибка при записи файла",
                        writer.errorMessage());
                }
            }
            else
            {
                QMessageBox::critical(
                    this,
                    "Ошибка при открытии файла",
                    output.errorString());
            }
        }
    }

    void MainWindow::schemeParamChanged()
    {
        const auto editor = ui->schemeEditor;
        const auto view = ui->schemeView;
        view->setDiameter(editor->minDiam());
        view->setSawSizes(
            editor->dwsSaw(),
            editor->pkaSaw()
            );
        view->setVertical(editor->isVertical());

        auto params = scheme->params();

        params->set_diameter(Unit::from_mm(editor->minDiam()));
        params->set_dws_gap(Unit::from_mm(editor->dwsSaw()));
        params->set_pka_gap(Unit::from_mm(editor->pkaSaw()));
        params->set_rot2_mode(editor->isVertical());
    }

    void MainWindow::centralWidthChanged(double width)
    {
        ui->schemeView->setCentralWidth(width);
        scheme->set_dws_board_width(Unit::from_mm(width));
    }

    void MainWindow::addCentralBoard(double /*width*/, double height)
    {
        ui->schemeView->addCentral(height);
        scheme->add_dws_board(Unit::from_mm(height));
    }

    void MainWindow::pa300Changed(bool enabled, double width, double height)
    {
        ui->schemeView->setPA300Enabled(enabled);
        ui->schemeView->setPA300Size(width, height);

        if (enabled)
        {
            scheme->set_pa300_board(Unit::from_mm(width),
                                    Unit::from_mm(height));
        }
        else
        {
            scheme->remove_pa300_poard();
        }
    }

    void MainWindow::pka350Changed(bool enabled, double width, double height)
    {
        ui->schemeView->setPKA350Enabled(enabled);
        ui->schemeView->setPKA350Size(width, height);

        if (enabled)
        {
            scheme->set_pka350_board(Unit::from_mm(width),
                                    Unit::from_mm(height));
        }
        else
        {
            scheme->remove_pka350_poard();
        }
    }

    void MainWindow::pa350Changed(bool enabled, double width, double height)
    {
        ui->schemeView->setPA350Enabled(enabled);
        ui->schemeView->setPA350Size(width, height);

        if (enabled)
        {
            scheme->set_pa350_board(Unit::from_mm(width),
                                    Unit::from_mm(height));
        }
        else
        {
            scheme->remove_pa350_poard();
        }
    }

} // namespace Schematics
