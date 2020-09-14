#include "schemeview.hxx"

#include "schemescene.hxx"

#include <QGraphicsEllipseItem>
#include <QGraphicsLineItem>
#include <QPen>
#include <QBrush>
#include <QColor>

namespace Schematics::Ui::Widgets
{
    struct SchemeItems
    {
        QGraphicsEllipseItem* diam = nullptr;
        QGraphicsLineItem* x_axis = nullptr;
        QGraphicsLineItem* y_axis = nullptr;

        static SchemeItems* create(SchemeScene* gfx)
        {
            auto self = new SchemeItems{};

            return self->build(gfx);
        }

        SchemeItems* build(SchemeScene* gfx)
        {
            diam = gfx->addEllipse(-1, -1, 1, 1);
            {
                auto pen = QPen{Qt::DashLine};
                pen.setWidth(5);
                diam->setPen(pen);
                auto bg = QBrush{Qt::yellow, Qt::BDiagPattern};
                diam->setBrush(bg);
            }
            auto axis = QPen{};
            axis.setStyle(Qt::DashDotLine);
            axis.setColor(Qt::red);
            x_axis = gfx->addLine(-1, 0, 1, 0, axis);
            y_axis = gfx->addLine(0, -1, 0, 1, axis);

            return this;
        }

        void setDiameter(double diameter)
        {
            auto x2 = diameter * 2;
            auto axis = diameter * 1.2;
            diam->setRect(-diameter, -diameter, x2, x2);
            x_axis->setLine(-axis, 0, axis, 0);
            y_axis->setLine(0, -axis, 0, axis);
        }
    };

    SchemeView::SchemeView(QWidget *parent) : QGraphicsView(parent) {
        gfx = new SchemeScene{this};
        setScene(gfx);
        gfx->setBackgroundBrush(Qt::white);
        scheme = SchemeItems::create(gfx);
        setDiameter(100.0);
    }

    void SchemeView::setDiameter(double diameter)
    {
        scheme->setDiameter(diameter);
    }

    void SchemeView::setCentralWidth(double width)
    {
        //
    }

    SchemeView::~SchemeView()
    {
        delete scheme;
    }
} // namespace Schematics::Ui::Widgets
