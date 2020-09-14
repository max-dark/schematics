#ifndef SCHEMATICS_SCHEMEVIEW_HXX
#define SCHEMATICS_SCHEMEVIEW_HXX

#include <QGraphicsView>

namespace Schematics::Ui::Widgets
{
    class SchemeScene;
    struct SchemeItems;
    class SchemeView: public QGraphicsView
    {
        Q_OBJECT
    public:
        explicit SchemeView(QWidget *parent = nullptr);
        ~SchemeView() override;

        void setDiameter(double diameter);
        void setCentralWidth(double width);
    private:
        SchemeScene* gfx = nullptr;
        SchemeItems* scheme = nullptr;
    };
} // namespace Schematics::Ui::Widgets

#endif //SCHEMATICS_SCHEMEVIEW_HXX
