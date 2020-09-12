#ifndef SCHEMATICS_SCHEMEVIEW_HXX
#define SCHEMATICS_SCHEMEVIEW_HXX

#include <QGraphicsView>

namespace Schematics::Ui::Widgets
{
    class SchemeScene;
    class SchemeView: public QGraphicsView
    {
        Q_OBJECT
    public:
        explicit SchemeView(QWidget *parent = nullptr);
        ~SchemeView() override;
    private:
        SchemeScene* gfx = nullptr;
    };
} // namespace Schematics::Ui::Widgets

#endif //SCHEMATICS_SCHEMEVIEW_HXX
