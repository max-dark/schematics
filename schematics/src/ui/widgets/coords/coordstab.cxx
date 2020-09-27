#include "coordstab.hxx"

#include <coords/coordinates.hxx>

#include <QtWidgets>

#include <ui/tools/tool.hxx>

namespace /* static */ {
/// Индификаторы позиций оборудования в системе координат линии
const char* PositionName[] =
{
    // Зона "ФБС1/

    "ФБС1/ левая фреза",
    //POS_ID_FBS1_LEFT_BLOCK,
    "ФБС1/ правая фреза",
    //POS_ID_FBS1_RIGHT_BLOCK,
    "ФБС1/ 1й прижимной ролик, высота предустановки",
    //POS_ID_FBS1_PRESS1_HEIGHT,
    "ФБС1/ 2й прижимной ролик, высота предустановки",
    //POS_ID_FBS1_PRESS2_HEIGHT,
    "ФБС1/ высота до центра поворотного устройства(ПУ)",
    //POS_ID_FBS1_ROTATOR_HEIGHT,
    "ФБС1/ предустановка ширины роликов ПУ на входе",
    //POS_ID_FBS1_ROTATOR_INP_WIDTH,
    "ФБС1/ предустановка ширины роликов ПУ на выходе",
    //POS_ID_FBS1_ROTATOR_OUT_WIDTH,

    // зона ФБС2

    "ФБС2/ левая фреза",
    //POS_ID_FBS2_LEFT_BLOCK,
    "ФБС2/ правая фреза",
    //POS_ID_FBS2_RIGHT_BLOCK,
    "ФБС2/ 1й ролик выравнивания",
    //POS_ID_FBS2_ALIGN1,
    "ФБС2/ 2й ролик выравнивания",
    //POS_ID_FBS2_ALIGN2,
    "ФБС2/ 3й ролик выравнивания",
    //POS_ID_FBS2_ALIGN3,
    // /// положение приемных роликов
    //TODO: POS_ID_FBS2_INP_ALIGN,
    "ФБС2/ 1й прижимной ролик, высота предустановки",
    //POS_ID_FBS2_PRESS1_HEIGHT,
    "ФБС2/ 2й прижимной ролик, высота предустановки",
    //POS_ID_FBS2_PRESS2_HEIGHT,
    "ФБС2/ 3й прижимной ролик, высота предустановки",
    //POS_ID_FBS2_PRESS3_HEIGHT,

    // зона 1го профилятора(PA350)

    "PA350/ положение левого блока",
    //POS_ID_P1_PA350_LEFT_BLOCK,
    "PA350/ положение правого блока",
    //POS_ID_P1_PA350_RIGHT_BLOCK,
    "PA350/ положение фрезы верхняя левая",
    //POS_ID_P1_PA350_TOP_LEFT,
    "PA350/ положение фрезы верхняя правая",
    //POS_ID_P1_PA350_TOP_RIGHT,
    "PA350/ положение фрезы нижняя левая",
    //POS_ID_P1_PA350_BOTTOM_LEFT,
    "PA350/ положение фрезы нижняя правая",
    //POS_ID_P1_PA350_BOTTOM_RIGHT,
    "PA350/ предустановка ширины центрирующих роликов на входе",
    //POS_ID_P1_PA350_ROLLERS_INP_WIDTH,

    // зона 2го профилятора(PKA300)

    "PKA300/ положение левого блока",
    //POS_ID_P2_PKA350_LEFT_BLOCK,
    "PKA300/ положение правого блока",
    //POS_ID_P2_PKA350_RIGHT_BLOCK,
    "PKA300/ положение фрезы верхняя левая",
    //POS_ID_P2_PKA350_TOP_LEFT,
    "PKA300/ положение фрезы верхняя правая",
    //POS_ID_P2_PKA350_TOP_RIGHT,
    "PKA300/ положение фрезы нижняя левая",
    //POS_ID_P2_PKA350_BOTTOM_LEFT,
    "PKA300/ положение фрезы нижняя правая",
    //POS_ID_P2_PKA350_BOTTOM_RIGHT,
    "PKA300/ положние левого внутреннего ролика",
    //POS_ID_P2_PKA350_LEFT_ROLLER,
    "/// положение правого внутреннего ролика",
    //POS_ID_P2_PKA350_RIGHT_ROLLER,
    "PKA300/ положение левой внешней пилы",
    //POS_ID_P2_PKA350_LEFT_SAW,
    "PKA300/ положение правой внешней пилы",
    //POS_ID_P2_PKA350_RIGHT_SAW,
    "PKA300/ предустановка ширины роликов на входе",
    //POS_ID_P2_PKA350_ROLLERS_INP_WIDTH,
    "PKA300/ предустановка ширины роликов на выходе",
    //POS_ID_P2_PKA350_ROLLERS_OUT_WIDTH,
    "PKA300/ высота центра поворотного устройства",
    //POS_ID_P2_PKA350_ROTATOR_HEIGHT,
    "PKA300/ высота центра левого блока пил",
    //POS_ID_P2_PKA350_LEFT_HEIGHT,
    "PKA300/ высота центра правого блока пил",
    //POS_ID_P2_PKA350_RIGHT_HEIGHT,
    "PKA300/ предустановка высоты прижима отделителя досок",
    //POS_ID_P2_PKA350_PRESS_HEIGHT,

    // зона 3го профилятора(PA300)

    "PA300/ положение левого блока",
    //POS_ID_P3_PA300_LEFT_BLOCK,
    "PA300/ положение правого блока",
    //POS_ID_P3_PA300_RIGHT_BLOCK,
    "PA300/ положение фрезы верхняя левая",
    //POS_ID_P3_PA300_TOP_LEFT,
    "PA300/ положение фрезы верхняя правая",
    //POS_ID_P3_PA300_TOP_RIGHT,
    "PA300/ положение фрезы нижняя левая",
    //POS_ID_P3_PA300_BOTTOM_LEFT,
    "PA300/ положение фрезы нижняя правая",
    //POS_ID_P3_PA300_BOTTOM_RIGHT,
    "PA300/ предустановка ширины центрирующих роликов на входе",
    //POS_ID_P3_PA300_ROLLERS_INP_WIDTH,

    // зона многопила(DWS350)

    "DWS350/ высота 1го прижимного ролика",
    //POS_ID_DWS350_PRESS1_HEIGHT,
    "DWS350/ высота 2го прижимного ролика",
    //POS_ID_DWS350_PRESS2_HEIGHT,
    "DWS350/ высота 3го прижимного ролика",
    //POS_ID_DWS350_PRESS3_HEIGHT,
    "DWS350/ предустановка ширины центрирующих роликов на входе",
    //POS_ID_DWS350_INP_ROLLERS_WIDTH,
    "DWS350/ предустановка ширины центрирующих роликов на выходе",
    //POS_ID_DWS350_OUT_ROLLERS_WIDTH,
    "DWS350/ позиция осей многопила",
    //POS_ID_DWS350_AXIS_HEIGHT,

    "/// количество позиций",
    //POS_ID_COUNT
};
} // namespace static

namespace Schematics::Ui::Widgets
{

    CoordsTab::CoordsTab(QWidget *parent)
        :QWidget(parent)
    {
        buildUi();
    }

    void CoordsTab::setPreview(QGraphicsScene *scene)
    {
        auto oldScene = preview->scene();
        if (scene == oldScene) return;

        if (nullptr != oldScene)
        {
            oldScene->disconnect(preview);
        }
        preview->setScene(scene);
        preview->fitInView(scene->itemsBoundingRect(), Qt::KeepAspectRatio);
        connect(scene, &QGraphicsScene::sceneRectChanged, [this](const QRectF &rect){
            preview->fitInView(rect, Qt::KeepAspectRatio);
        });
    }

    const Coords::Coordinates &CoordsTab::coordinates() const
    {
        return coords;
    }

    void CoordsTab::fillCoords(const libschema::Schema *schema)
    {
        QSignalBlocker block{this};

        coords.fill_from(schema);

        auto p = pos_list.begin();
        for(const auto coord: coords)
        {
            (*p)->setValue(coord.to_mm());
            ++p;
        }
    }

    void CoordsTab::coordChanged(Coords::PositionId id, libschema::Unit value)
    {
        coords.setById(id, value);
    }

    void CoordsTab::applyBtnClicked(Coords::PositionId id)
    {
        emit applyCoord(id, coords.byId(id));
    }

    void CoordsTab::buildUi()
    {
        auto main = new QHBoxLayout;
        auto coordsView = new QScrollArea{};
        auto coordsBox = new QGridLayout;
        {
            int col = 0;
            int row = 0;
            int id = 0;
            for (auto& p : pos_list) {
                Coords::PositionId pos_id = Coords::PositionId(id);

                p = new NumberEditor{};
                p->setDecimals(2);
                p->setRange(-1500, 1500);

                connect(p, qOverload<double>(&NumberEditor::valueChanged), [this, pos_id](double value)
                {
                    coordChanged(pos_id, libschema::Unit::from_mm(value));
                });

                auto lbl = new QLabel{QString{"№%1 %2"}.arg(id + 1).arg(PositionName[id])};
                lbl->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
                lbl->setWordWrap(true);

                auto btn = new QPushButton{QString{"Применить"}};
                connect(btn, &QPushButton::clicked, [this, pos_id]{
                    applyBtnClicked(pos_id);
                });

                coordsBox->addWidget(lbl, row, col, 2, 1, Qt::AlignTop);
                coordsBox->addWidget(p, row + 0, col + 1);
                coordsBox->addWidget(btn, row + 1, col + 1);
                row += 2;
                if (row % 34 == 0)
                {
                    row = 0; col += 2;
                }
                ++id;
            }
        }

        auto sideBox = new QVBoxLayout;
        {
            preview = new QGraphicsView;
            preview->setFixedSize(400, 400);
            auto calcBtn = new QPushButton{"Рассчитать по схеме"};
            calcBtn->setFixedHeight(50);
            connect(calcBtn, &QPushButton::clicked, this, &CoordsTab::needCaclculate);

            sideBox->addWidget(preview);
            sideBox->addWidget(calcBtn);
            sideBox->addItem(tool::createVSpace());
        }

        auto lst = new QWidget{};
        lst->setLayout(coordsBox);
        coordsView->setWidget(lst);
        main->addWidget(coordsView);
        main->addLayout(sideBox);
        setLayout(main);
    }

    CoordsTab::~CoordsTab() = default;
} // namespace Schematics::Ui::Widgets
