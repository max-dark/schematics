#include "schemeview.hxx"

#include "schemescene.hxx"

#include <QGraphicsEllipseItem>
#include <QGraphicsLineItem>
#include <QGraphicsRectItem>
#include <QPen>
#include <QBrush>
#include <QColor>
#include <QList>

namespace Schematics::Ui::Widgets
{
    class BoardItem: public QGraphicsRectItem
    {
    public:
        enum Orientation
        {
            Vertical, Horisontal
        };
        explicit BoardItem(QGraphicsItem* parent = nullptr)
            :QGraphicsRectItem(parent)
        {
            setVisible(false);
        }
        ~BoardItem() override {}
        Orientation orientation() const { return m_orientation; }
        void setOrientation(Orientation o)
        {
            if (m_orientation != o)
            {
                m_orientation = o;
                // swap sizes
                auto r = rect();
                setRect(r.x(), r.y(), r.height(), r.width());
            }
        }
        bool isVertical() const { return m_orientation == Vertical; }
        bool isHorisontal() const { return m_orientation == Horisontal; }
        void setSize(double w, double h)
        {
            if (isVertical())
            {
                std::swap(w, h);
            }
            auto r = rect();
            setRect(r.x(), r.y(), w, h);
        }
        void setFillColor(const QColor& color)
        {
            auto bg = QBrush{color, Qt::Dense5Pattern};
            setBrush(bg);
        }
    private:
        Orientation m_orientation = Vertical;
    };

    struct DWS350BoardItem: public BoardItem
    {
        DWS350BoardItem(): BoardItem(nullptr) {
            setFillColor(Qt::green);
            setVisible(true);
        }
    };

    struct PA300BoardItem: public BoardItem
    {
        PA300BoardItem(): BoardItem(nullptr) {
            setFillColor(Qt::red);
        }
    };

    struct PKA350BoardItem: public BoardItem
    {
        PKA350BoardItem(): BoardItem(nullptr) {
            setOrientation(Horisontal);
            setFillColor(Qt::blue);
        }
    };

    struct PA350BoardItem: public BoardItem
    {
        PA350BoardItem(): BoardItem(nullptr)
        {
            setOrientation(Horisontal);
            setFillColor(Qt::darkCyan);
        }
    };

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
        setDiameter(200.0);
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
