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
                updatePos();
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
            updatePos();
        }
        void setWidth(double width)
        {
            auto r = rect();

            switch (orientation()) {
            case Vertical:
                r.setHeight(width);
                break;
            case Horisontal:
                r.setWidth(width);
                break;
            }
            setRect(r);
            updatePos();
        }
        void setHeight(double height)
        {
            auto r = rect();

            switch (orientation()) {
            case Vertical:
                r.setWidth(height);
                break;
            case Horisontal:
                r.setHeight(height);
                break;
            }
            setRect(r);
            updatePos();
        }
        void updatePos()
        {
            auto r = rect();
            auto w = r.width();
            auto h = r.height();

            switch (orientation()) {
            case Vertical:
                r.moveTo(r.x(), - r.height() / 2);
                std::swap(h, w);
                break;
            case Horisontal:
                {
                    r.moveTo(- w / 2, r.y());
                }
                break;
            }
            setToolTip(QString{"%1 x %2"}
                           .arg(w, 0, 'f', 1)
                           .arg(h, 0, 'f', 1)
                       );
            setRect(r);
        }
        void setFillColor(const QColor& color)
        {
            auto bg = QBrush{color, Qt::Dense5Pattern};
            setBrush(bg);
        }
    private:
        Orientation m_orientation = Vertical;

        // QGraphicsItem interface
    public:
        void paint(QPainter *painter,
                   const QStyleOptionGraphicsItem *option,
                   QWidget *widget) override
        {
            QGraphicsRectItem::paint(painter, option, widget);
            customPaint(painter);
        }
    protected:
        virtual void customPaint(QPainter* painter)
        {
            painter->save();
            {
                auto b = boundingRect();
                auto center = b.center();
                auto zero = QPointF{};
                if (isVertical())
                {
                    painter->translate(center);
                    painter->rotate(90);
                    b.setSize({b.height(), b.width()});
                    b.moveCenter(zero);
                }
                auto font = painter->font();
                font.setPixelSize(9);
                painter->setFont(font);
                painter->setClipRect(b);
                auto flags = Qt::AlignCenter | Qt::TextSingleLine;
                auto rt = painter->fontMetrics()
                              .boundingRect(b.toRect(), flags, toolTip());
                auto bg = painter->background();
                bg.setStyle(Qt::SolidPattern);
                painter->fillRect(rt, bg);
                painter->drawText(b, flags, toolTip());
            }
            painter->restore();
        }
    };

    struct DWS350BoardItem: public BoardItem
    {
        DWS350BoardItem(): BoardItem(nullptr) {
            setFillColor(Qt::green);
            setVisible(true);
        }

        void setIndex(int value)
        {
            m_index = value;
        }

    private:
        int m_index = 0;
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
        enum Side
        {
            LeftSide, RightSide,
            TopSide = LeftSide,
            BottomSide = RightSide
        };
        using CentralBoards = QList<DWS350BoardItem*>;

        CentralBoards dws350;
        PA300BoardItem* pa300[2];
        PKA350BoardItem* pka350[2];
        PA350BoardItem* pa350[2];

        QGraphicsEllipseItem* diam = nullptr;
        QGraphicsLineItem* x_axis = nullptr;
        QGraphicsLineItem* y_axis = nullptr;

        double centralWidth = 200.0;
        double sawDws = 4.8;
        double sawPka = 5.6;
        bool pkaIsVertical = false;

        static SchemeItems* create(SchemeScene* gfx)
        {
            auto self = new SchemeItems{};

            return self->build(gfx);
        }

        SchemeItems* build(SchemeScene* gfx)
        {
            // diameter visualisation
            {
                diam = gfx->addEllipse(-1, -1, 1, 1);
                auto pen = QPen{Qt::DashLine};
                pen.setWidth(5);
                diam->setPen(pen);
                auto bg = QBrush{Qt::yellow, Qt::BDiagPattern};
                diam->setBrush(bg);
            }

            // coord axis visualisation
            {
                auto axis = QPen{};
                axis.setStyle(Qt::DashDotLine);
                axis.setColor(Qt::red);
                x_axis = gfx->addLine(-1, 0, 1, 0, axis);
                y_axis = gfx->addLine(0, -1, 0, 1, axis);
            }

            for(auto& b: pa300)
            {
                b = new PA300BoardItem{};
                gfx->addItem(b);
            }

            for(auto& b: pka350)
            {
                b = new PKA350BoardItem{};
                gfx->addItem(b);
            }

            for(auto& b: pa350)
            {
                b = new PA350BoardItem{};
                gfx->addItem(b);
            }

            return this;
        }


        void clear()
        {
            qDeleteAll(dws350.begin(), dws350.end());
            dws350.clear();

            setPA300Visible(false);
            setPKA350Visible(false);
            setPA350Visible(false);
        }

        double dwsSaw() const
        { return sawDws; }

        double pkaSaw() const
        { return sawPka; }

        void setSaws(double dws, double pka)
        {
            sawDws = dws;
            sawPka = pka;
            updateGeometry();
        }

        void setDiameter(double diameter)
        {
            auto radius = diameter / 2;
            auto axis = radius * 1.2;
            diam->setRect(-radius, -radius, diameter, diameter);
            x_axis->setLine(-axis, 0, axis, 0);
            y_axis->setLine(0, -axis, 0, axis);
        }

        void setCentralWidth(double width)
        {
            centralWidth = width;
            for(auto b: dws350)
            {
                b->setWidth(width);
            }
            updateGeometry();
        }

        bool setCentralHeight(int index, double height)
        {
            if (index >= 0 && index < dws350.size())
            {
                dws350.at(index)->setHeight(height);
                updateGeometry();
                return true;
            }
            return false;
        }

        auto addCentral(SchemeScene* gfx, double height)
        {
            auto b = new DWS350BoardItem{};
            b->setSize(centralWidth, height);
            gfx->addItem(b);
            dws350.push_back(b);

            return b;
        }

        bool removeCentral(int idx)
        {
            if (idx >= 0 && idx < dws350.size())
            {
                auto it = dws350.begin() + idx;

                // autoremove from scene
                // in QGraphicsItem destructor
                delete (*it);
                // remove from central list
                dws350.erase(it);

                updateGeometry();

                return true;
            }
            return false;
        }

        void setPA300Size(double width, double height)
        {
            setDevSize(pa300, width, height);
        }

        void setPA300Visible(bool visible)
        {
            setDevVisible(pa300, visible);
        }

        bool pa300Visible()
        { return devIsVisible(pa300); }

        void setPKA350Size(double width, double height)
        {
            setDevSize(pka350, width, height);
        }

        void setPKA350Visible(bool visible)
        {
            setDevVisible(pka350, visible);
        }

        bool pka350Visible()
        { return devIsVisible(pka350); }

        void setPA350Size(double width, double height)
        {
            setDevSize(pa350, width, height);
        }

        void setPA350Visible(bool visible)
        {
            setDevVisible(pa350, visible);
        }

        bool pa350Visible()
        { return devIsVisible(pa350); }

        void setVertical(bool isVertical)
        {
            pkaIsVertical = isVertical;
            auto o = isVertical ? BoardItem::Vertical : BoardItem::Horisontal;
            pka350[LeftSide]->setOrientation(o);
            pka350[RightSide]->setOrientation(o);
            pa350[LeftSide]->setOrientation(o);
            pa350[RightSide]->setOrientation(o);
            updateGeometry();
        }

        void updateGeometry()
        {
            // calc dws rect
            auto dw = 0.0;
            auto dh = dws350.count() == 0 ? 0.0 : centralWidth;

            for(auto b: dws350)
            {
                dw += b->rect().width() + dwsSaw();
            }
            if (dws350.count() > 0)
            {
                dw -= dwsSaw();
            }
            auto x_half = dw / 2;
            auto y_half = dh / 2;

            auto x = -x_half;
            for(auto b: dws350)
            {
                auto r = b->rect();

                b->setRect(x, -y_half, r.width(), r.height());

                x += r.width() + dwsSaw();
            }

            // update PA300
            {
                auto rr = pa300[RightSide]->rect();
                rr.moveTo(
                    x_half + dwsSaw(),
                    -rr.height() / 2.0);
                pa300[RightSide]->setRect(rr);

                auto rl = pa300[LeftSide]->rect();
                rl.moveTo(
                    -x_half - rl.width() - dwsSaw(),
                    -rl.height() / 2.0);
                pa300[LeftSide]->setRect(rl);
            }

            // update PKA350/PA350
            if (pkaIsVertical)
            {
                auto x_offset = x_half + pkaSaw();
                if (pa300Visible())
                {
                    x_offset += dwsSaw() + pa300[RightSide]->rect().width();
                }

                {
                    auto rr = pka350[RightSide]->rect();
                    rr.moveTo(x_offset, - rr.height() / 2.0);
                    pka350[RightSide]->setRect(rr);

                    auto rl = pka350[LeftSide]->rect();
                    rl.moveTo(-x_offset - rl.width()
                              ,- rl.height() / 2.0);
                    pka350[LeftSide]->setRect(rl);
                }

                {
                    auto l_offset = -x_offset;
                    auto r_offset = x_offset;
                    if (pka350Visible())
                    {
                        l_offset -= pkaSaw() + pka350[LeftSide]->rect().width();
                        r_offset += pkaSaw() + pka350[RightSide]->rect().width();
                    }

                    auto rr = pa350[RightSide]->rect();
                    rr.moveTo(r_offset, - rr.height() / 2.0);
                    pa350[RightSide]->setRect(rr);

                    auto rl = pa350[LeftSide]->rect();
                    rl.moveTo(l_offset - rl.width(), - rl.height() / 2.0);
                    pa350[LeftSide]->setRect(rl);
                }
            }
            else
            {
                auto y_offset = y_half + pkaSaw();
                // pka
                {
                    auto rt = pka350[TopSide]->rect();
                    rt.moveTo(-rt.width() / 2.0, -y_offset - rt.height());
                    pka350[TopSide]->setRect(rt);

                    auto rb = pka350[BottomSide]->rect();
                    rb.moveTo(-rb.width() / 2.0, y_offset);
                    pka350[BottomSide]->setRect(rb);
                }

                // pa
                {
                    auto y_t = - y_offset;
                    auto y_b =  y_offset;

                    if (pka350Visible())
                    {
                        y_t -= pkaSaw() + pka350[TopSide]->rect().height();
                        y_b += pkaSaw() + pka350[BottomSide]->rect().height();
                    }

                    auto rt = pa350[TopSide]->rect();
                    rt.moveTo(-rt.width()/2.0, y_t - rt.height());
                    pa350[TopSide]->setRect(rt);

                    auto rb = pa350[BottomSide]->rect();
                    rb.moveTo(-rb.width() / 2.0, y_b);
                    pa350[BottomSide]->setRect(rb);
                }
            }
        }
    private:
        template<class Name>
        void setDevSize(Name* dev[2], double width, double height)
        {
            static_assert(std::is_base_of_v<BoardItem, Name>, "Invalid dev parameter");
            dev[LeftSide]->setSize(width, height);
            dev[RightSide]->setSize(width, height);
            updateGeometry();
        }

        template<class Name>
        void setDevVisible(Name* dev[2], bool visible)
        {
            static_assert(std::is_base_of_v<BoardItem, Name>, "Invalid dev parameter");
            dev[LeftSide]->setVisible(visible);
            dev[RightSide]->setVisible(visible);
            updateGeometry();
        }

        template<class Name>
        bool devIsVisible(Name* dev[2])
        {
            static_assert(std::is_base_of_v<BoardItem, Name>, "Invalid dev parameter");

            return dev[LeftSide]->isVisible() && dev[RightSide]->isVisible();
        }
    };

    SchemeView::SchemeView(QWidget *parent) : QGraphicsView(parent) {
        gfx = new SchemeScene{this};
        setScene(gfx);
        scale(2.0, 2.0);
        gfx->setBackgroundBrush(Qt::white);
        scheme = SchemeItems::create(gfx);
        setDiameter(200.0);
    }

    void SchemeView::setDiameter(double diameter)
    {
        scheme->setDiameter(diameter);
    }

    void SchemeView::setVertical(bool isVertical)
    {
        scheme->setVertical(isVertical);
    }

    void SchemeView::setSawSizes(double dws, double pka)
    {
        scheme->setSaws(dws, pka);
    }

    void SchemeView::addCentral(double height)
    {
        scheme->addCentral(gfx, height);
        scheme->updateGeometry();
    }

    void SchemeView::setCentralWidth(double width)
    {
        scheme->setCentralWidth(width);
    }

    void SchemeView::setCentralHeight(int index, double height)
    {
        scheme->setCentralHeight(index, height);
    }

    void SchemeView::removeCentral(int index)
    {
        scheme->removeCentral(index);
    }

    void SchemeView::setPA300Size(double width, double height)
    {
        scheme->setPA300Size(width, height);
    }

    void SchemeView::setPA300Enabled(bool enabled)
    {
        scheme->setPA300Visible(enabled);
    }

    void SchemeView::setPKA350Size(double width, double height)
    {
        scheme->setPKA350Size(width, height);
    }

    void SchemeView::setPKA350Enabled(bool enabled)
    {
        scheme->setPKA350Visible(enabled);
    }

    void SchemeView::setPA350Size(double width, double height)
    {
        scheme->setPA350Size(width, height);
    }

    void SchemeView::setPA350Enabled(bool enabled)
    {
        scheme->setPA350Visible(enabled);
    }

    SchemeView::~SchemeView()
    {
        delete scheme;
    }

    void SchemeView::clear()
    {
        scheme->clear();
    }
} // namespace Schematics::Ui::Widgets
