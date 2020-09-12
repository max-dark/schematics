#include "schemeview.hxx"

#include "schemescene.hxx"

namespace Schematics::Ui::Widgets
{

    SchemeView::SchemeView(QWidget *parent) : QGraphicsView(parent) {
        gfx = new SchemeScene{this};
        setScene(gfx);
    }

    SchemeView::~SchemeView() = default;
} // namespace Schematics::Ui::Widgets
