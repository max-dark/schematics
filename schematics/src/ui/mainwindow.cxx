#include "mainwindow.hxx"

#include <QtWidgets>
#include <QtGui>
#include <QtCore>

#include <ui/widgets/scheme/schemeview.hxx>
#include <ui/widgets/scheme/schemeeditor.hxx>

#include <ui/widgets/coords/coordstab.hxx>
#include <ui/widgets/leds.hxx>
#include <ui/widgets/ledlist.hxx>
#include <ui/tools/tool.hxx>

#include <schema/schema.hxx>
#include <schema/params.hxx>
#include <schema/units.hxx>

#include <schema/xmlwriter.hxx>
#include <schema/xmlreader.hxx>

#include <services/application.hxx>

#include <QVector>

using libschema::Unit;

namespace Schematics {

    static const QString scheme_filter{"Cutting scheme (*.cut)"};

    namespace Ui {

        using LedVector = QVector<Widgets::Led*>;

        struct MainView {
            QAction* scheme_new = nullptr;
            QAction* scheme_load = nullptr;
            QAction* scheme_save = nullptr;

            QAction* cfg_coords = nullptr;
            QAction* cfg_speeds = nullptr;
            QAction* cfg_delays = nullptr;

            QAction *a_connect = nullptr;
            QAction *a_init = nullptr;

            QAction* help_about = nullptr;
            QAction* help_qt = nullptr;

            Widgets::Led* sab_plc = nullptr;
            Widgets::Led* kdo_plc = nullptr;
            LedVector sab_leds;
            LedVector kdo_leds;

            QTabWidget* tabList = nullptr;

            Widgets::SchemeView *schemeView = nullptr;
            Widgets::SchemeEditor* schemeEditor = nullptr;

            Widgets::CoordsTab* coordsTab = nullptr;

            Widgets::LedList* motorsTab = nullptr;
            Widgets::LedList* alarmsTab = nullptr;
            Widgets::LedList* sensorsTab = nullptr;

            void buildView(QMainWindow *self);

        private:
            QLabel* createLabel(const QString& title);
            QWidget *createEditorTab();
            QMenuBar *createMainMenu();

        };
        QLabel* MainView::createLabel(const QString& title)
        {
            auto label = new QLabel{title};
            label->setAlignment(Qt::AlignVCenter);
            return label;
        }

        void MainView::buildView(QMainWindow *self) {
            self->setCentralWidget(new QWidget);
            self->setWindowTitle("Schematics PoC Demo");
            auto mainBox = new QVBoxLayout;

            // add top panel
            {
                using namespace Ui::Widgets;
                auto topBox = new QHBoxLayout;

                {
                    topBox->addWidget(createLabel("Лесопиление: "));
                    sab_plc = new Led{};
                    sab_plc->setColor(Led::RED);
                    topBox->addWidget(sab_plc);
                    topBox->addWidget(createLabel("PLC"));

                    auto led_names = QStringList{}
                                     << "Подача" << "ФБС1" << "ФБС2"
                                     << "PA350 / PKA350" << "PA300 / DWS350"
                                     << "Боковой конвейер";
                    sab_leds.reserve(led_names.size());
                    for (auto& name : led_names)
                    {
                        auto led = new Led{};
                        topBox->addWidget(led);
                        sab_leds.push_back(led);
                        auto lbl = createLabel(name);
                        lbl->setWordWrap(true);
                        topBox->addWidget(lbl);

                    }
                }
                auto vline = new QFrame{};
                vline->setFrameShape(QFrame::VLine);
                vline->setFrameShadow(QFrame::Sunken);
                topBox->addWidget(vline);

                {
                    topBox->addWidget(createLabel("КДО: "));
                    kdo_plc = new Led{};
                    kdo_plc->setColor(Led::RED);
                    topBox->addWidget(kdo_plc);
                    topBox->addWidget(createLabel("PLC"));

                    auto led_names = QStringList{}
                                     << "Улица / щепа" << "Улица / опилки"
                                     << "ФБС"
                                     << "Профиляторы" << "Многопил";
                    kdo_leds.reserve(led_names.size());
                    for (auto& name : led_names)
                    {
                        auto led = new Led{};
                        topBox->addWidget(led);
                        kdo_leds.push_back(led);
                        auto lbl = createLabel(name);
                        lbl->setWordWrap(true);
                        topBox->addWidget(lbl);

                    }
                }
                mainBox->addLayout(topBox);
            }
            // add tabs
            {
                tabList = new QTabWidget;
                {
                    auto schemeTab = createEditorTab();
                    coordsTab = new Widgets::CoordsTab{};
                    coordsTab->setPreview(schemeView->scene());

                    motorsTab = new Widgets::LedList;
                    alarmsTab = new Widgets::LedList;
                    sensorsTab = new Widgets::LedList;

                    tabList->addTab(schemeTab, QString::fromUtf8("Схема раскроя"));
                    tabList->addTab(coordsTab, QString::fromUtf8("Координаты оборудования"));
                    tabList->addTab(motorsTab, "Механизмы");
                    tabList->addTab(alarmsTab, "Индикаторы аварий");
                    tabList->addTab(sensorsTab, "Датчики");
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
            // schema
            {
                auto menu = mainMenu->addMenu("Схема");

                scheme_new =  menu->addAction("Новая");
                scheme_load =  menu->addAction("Загрузить");
                scheme_save = menu->addAction("Сохранить");
            }

            // tools
            {
                auto menu = mainMenu->addMenu("Инструменты");

                menu->addSection("Калибровка");
                cfg_coords = menu->addAction("Координаты");
                cfg_speeds = menu->addAction("Скорости");
                cfg_delays = menu->addAction("Интервалы");

                menu->addSection("Линия");
                a_connect = menu->addAction("Подключение");
                a_init = menu->addAction("Инициализация");
            }

            // about
            {
                auto menu = mainMenu->addMenu("Справка");

                help_about = menu->addAction("О программе");
                help_qt = menu->addAction("О Qt");
            }

            return mainMenu;
        }

    }

    template<class ObjectT, class MethodT>
    auto bindAction(QAction* action, ObjectT* object, MethodT method)
    {
        return QObject::connect(
                action, &QAction::triggered,
                object, method
        );
    }

    MainWindow::MainWindow(Service::Application *app, QWidget *parent)
        : QMainWindow(parent),
        ui{new Ui::MainView()}
    {
        this->app = app;
        ui->buildView(this);
        //setWindowState(Qt::WindowMaximized);
        bindEvents();

        auto params = new libschema::Params{};
        scheme = new libschema::Schema{this};
        scheme->set_params(params);

        ui->schemeEditor->setParams(
            250, 250,
            4.8, 5.6,
            false);

        updateViews();
    }

    MainWindow::~MainWindow() {
        delete ui;
    }

    void MainWindow::bindEvents() {
        bindMenus();
        bindSchemeEditor();
        bindCoordsTab();
    }

    void MainWindow::bindMenus()
    {
        bindAction(ui->scheme_new, this, &MainWindow::on_newScheme);
        bindAction(ui->scheme_load, this, &MainWindow::on_loadScheme);
        bindAction(ui->scheme_save, this, &MainWindow::on_saveScheme);

        bindAction(ui->cfg_coords, this, &MainWindow::on_cfgCoords);
        bindAction(ui->cfg_speeds, this, &MainWindow::on_cfgSpeeds);
        bindAction(ui->cfg_delays, this, &MainWindow::on_cfgDelays);
        bindAction(ui->a_connect, this, &MainWindow::on_initConnection);
        bindAction(ui->a_init, this, &MainWindow::on_initAxis);

        bindAction(ui->help_about, this, &MainWindow::on_about);
        bindAction(ui->help_qt, this, &MainWindow::on_aboutQt);
    }

    void MainWindow::bindSchemeEditor() {
        using Schematics::Ui::Widgets::SchemeEditor;

        connect(ui->schemeEditor, &SchemeEditor::schemeParamChanged,
                this, &MainWindow::schemeParamChanged);
        connect(ui->schemeEditor, &SchemeEditor::centralWidthChanged,
                this, &MainWindow::centralWidthChanged);
        connect(ui->schemeEditor, &SchemeEditor::addCentralBoards,
                this, &MainWindow::addCentralBoards);
        connect(ui->schemeEditor, &SchemeEditor::deleteAllCentralBoards,
                this, &MainWindow::deleteAllCentralBoards);
        connect(ui->schemeEditor, &SchemeEditor::deleteCentralBoardByPos,
                this, &MainWindow::deleteCentralBoardByPos);

        connect(ui->schemeEditor, &SchemeEditor::setAllCentralHeights,
                this, &MainWindow::setAllCentralHeights);
        connect(ui->schemeEditor, &SchemeEditor::setCentralHeightByPos,
                this, &MainWindow::setCentralHeightByPos);

        connect(ui->schemeEditor, &SchemeEditor::pa300Changed,
                this, &MainWindow::pa300Changed);
        connect(ui->schemeEditor, &SchemeEditor::pka350Changed,
                this, &MainWindow::pka350Changed);
        connect(ui->schemeEditor, &SchemeEditor::pa350Changed,
                this, &MainWindow::pa350Changed);

        connect(ui->schemeEditor, &SchemeEditor::calculateSchemeCoords,
                this, &MainWindow::calculateScheme);
        connect(ui->schemeEditor, &SchemeEditor::applySchemeCoords,
                this, &MainWindow::applyCoords);
    }

    void MainWindow::bindCoordsTab()
    {
        using Schematics::Ui::Widgets::CoordsTab;
        connect(ui->coordsTab, &CoordsTab::needCaclculate,
                this, &MainWindow::calculateScheme);
        connect(ui->coordsTab, &CoordsTab::applyCoord,
                this, &MainWindow::applyCoordById);
    }

    void MainWindow::updateViews()
    {
        bool sab, kdo;
        app->getConnectionState(sab, kdo);
        updateSab(sab);
        updateKdo(kdo);
    }

    void MainWindow::updateSab(bool is_connected)
    {
        using namespace Ui::Widgets;
        ui->sab_plc->setColor(is_connected ? Led::GREEN : Led::RED);
    }

    void MainWindow::updateKdo(bool is_connected)
    {
        using namespace Ui::Widgets;
        ui->kdo_plc->setColor(is_connected ? Led::GREEN : Led::RED);
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
                auto tmp_schema = new libschema::Schema{this};
                tmp_schema->set_params(new libschema::Params{});
                libschema::XmlReader reader;
                auto read_ok = reader.read(tmp_schema, input);
                if (read_ok)
                {
                    applyScheme(tmp_schema);

                    // set current scheme
                    std::swap(scheme, tmp_schema);
                }
                else
                {
                    QMessageBox::critical(
                        this,
                        "Ошибка при загрузке файла",
                        reader.errorMessage());
                }

                // cleanup
                tmp_schema->deleteLater();
            }
            else
            {
                QMessageBox::critical(
                    this,
                    "Ошибка при открытии файла",
                    input.errorString());
            }
        }
    }

    void MainWindow::applyScheme(const libschema::Schema *new_schema)
    {
        // clear views
        ui->schemeView->clear();
        ui->schemeEditor->clearAll();

        // load params
        auto params = new_schema->params();
        ui->schemeEditor->setParams(
            params->diameter().to_mm(),
            params->diameter().to_mm(),
            params->dws_gap().to_mm(),
            params->pka_gap().to_mm(),
            params->is_rot2_disabled()
            );

        // load dws350
        ui->schemeEditor->setDWS350(
            new_schema->dws_board_width().to_mm(),
            0.0);
        for(const auto h: new_schema->dws350().boards)
        {
            ui->schemeView->addCentral(h.to_mm());
        }

        // load pa300
        ui->schemeEditor->setPA300(
            new_schema->pa300().is_valid(),
            new_schema->pa300().board_width.to_mm(),
            new_schema->pa300().board_height.to_mm());

        // load pka350
        ui->schemeEditor->setPKA350(
            new_schema->pka350().is_valid(),
            new_schema->pka350().board_width.to_mm(),
            new_schema->pka350().board_height.to_mm());

        // load pa350
        ui->schemeEditor->setPA350(
            new_schema->pa350().is_valid(),
            new_schema->pa350().board_width.to_mm(),
            new_schema->pa350().board_height.to_mm());
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

    void MainWindow::calculateScheme()
    {
        ui->coordsTab->fillCoords(scheme);
    }

    void MainWindow::applyCoordById(Coords::PositionId id, libschema::Unit value)
    {
        app->applyCoordById(id, value);
    }

    void MainWindow::applyCoords()
    {
        app->applyCoordinates(ui->coordsTab->coordinates());
    }

    void MainWindow::centralWidthChanged(double width)
    {
        ui->schemeView->setCentralWidth(width);
        scheme->set_dws_board_width(Unit::from_mm(width));
    }

    void MainWindow::addCentralBoards(double height, size_t count)
    {
        for(size_t i = 0; i < count; ++i)
        {
            ui->schemeView->addCentral(height);
            scheme->add_dws_board(Unit::from_mm(height));
        }
    }

    void MainWindow::deleteAllCentralBoards()
    {
        auto count = scheme->dws350().count().units();
        while (count > 0) {
            count -= 1;
            scheme->remove_dws_board(count);
            ui->schemeView->removeCentral(count);
        }
    }

    void MainWindow::deleteCentralBoardByPos()
    {
        bool ok;
        auto count = scheme->dws350().count().units();

        if (count > 0)
        {
            auto idx = QInputDialog::getInt(
                        this,
                        "Введите номер доски",
                        "Введите номер доски для удаления (счет идет слева направо)",
                        1, 1, count, 1, &ok);
            if (ok)
            {
                idx -= 1;
                scheme->remove_dws_board(idx);
                ui->schemeView->removeCentral(idx);
            }
        }
    }

    void MainWindow::setAllCentralHeights(double height)
    {
        if (height > 0.0)
        {
            auto count = scheme->dws350().count().units();
            while (count > 0) {
                count -= 1;
                scheme->set_dws_board_height(count, Unit::from_mm(height));
                ui->schemeView->setCentralHeight(count, height);
            }
        }
    }

    void MainWindow::setCentralHeightByPos(double height)
    {
        if (height > 0.0)
        {
            bool ok;
            auto count = scheme->dws350().count().units();

            if (count > 0)
            {
                auto idx = QInputDialog::getInt(
                            this,
                            "Введите номер доски",
                            "Введите номер доски для замены (счет идет слева направо)",
                            1, 1, count, 1, &ok);
                if (ok)
                {
                    idx -= 1;
                    scheme->set_dws_board_height(idx, Unit::from_mm(height));
                    ui->schemeView->setCentralHeight(idx, height);
                }
            }
        }
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

void MainWindow::on_cfgCoords()
{

}

void MainWindow::on_cfgSpeeds()
{

}

void MainWindow::on_cfgDelays()
{

}

void MainWindow::on_initConnection()
{

}

void MainWindow::on_initAxis()
{

}

void MainWindow::on_about()
{
    QMessageBox::about(this, windowTitle(),
                       "Разработано:\n"
                       "Max Dark <maxim.dark@gmail.com>\n"
                       "Репозиторий проекта:\n"
                       "https://gitlab.com/andreevo-ldk/alpha/schematics");
}

void MainWindow::on_aboutQt()
{
    QMessageBox::aboutQt(this);
}

} // namespace Schematics
