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

#include <ui/widgets/scheme/schemeview.hxx>
#include <ui/widgets/scheme/schemeeditor.hxx>

#include <ui/widgets/coords/coordstab.hxx>

namespace Schematics {

    namespace Ui {

        struct MainView {
            QLabel *lbl_status = nullptr;

            QTabWidget* tabList = nullptr;

            Widgets::SchemeView *schemeView = nullptr;
            Widgets::SchemeEditor* schemeEditor = nullptr;

            Widgets::CoordsTab* coordsTab = nullptr;

            void buildView(QMainWindow *self);

        private:
            QWidget *createEditorTab();

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

    }

    MainWindow::MainWindow(QWidget *parent)
            : QMainWindow(parent), ui{new Ui::MainView()} {
        ui->buildView(this);
        setWindowState(Qt::WindowMaximized);
        bindEvents();


        ui->schemeEditor->setParams(
            250, 250,
            4.8, 5.6,
            false);
    }

    MainWindow::~MainWindow() {
        delete ui;
    }

    void MainWindow::bindEvents() {
        bindSchemeEditor();
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
    }

    void MainWindow::on_loadScheme() {

    }

    void MainWindow::on_saveScheme() {

    }

    void MainWindow::schemeParamChanged()
    {
        ui->schemeView->setDiameter(ui->schemeEditor->minDiam());
        ui->schemeView->setSawSizes(
            ui->schemeEditor->dwsSaw(),
            ui->schemeEditor->pkaSaw()
            );
        ui->schemeView->setVertical(ui->schemeEditor->isVertical());
    }

    void MainWindow::centralWidthChanged(double width)
    {
        ui->schemeView->setCentralWidth(width);
    }

    void MainWindow::addCentralBoard(double width, double height)
    {
        ui->schemeView->addCentral(height);
    }

    void MainWindow::pa300Changed(bool enabled, double width, double height)
    {
        ui->schemeView->setPA300Enabled(enabled);
        ui->schemeView->setPA300Size(width, height);
    }

    void MainWindow::pka350Changed(bool enabled, double width, double height)
    {
        ui->schemeView->setPKA350Enabled(enabled);
        ui->schemeView->setPKA350Size(width, height);
    }

    void MainWindow::pa350Changed(bool enabled, double width, double height)
    {
        ui->schemeView->setPA350Enabled(enabled);
        ui->schemeView->setPA350Size(width, height);
    }

} // namespace Schematics
