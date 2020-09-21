#ifndef SCHEMATICS_SCHEMESCENE_HXX
#define SCHEMATICS_SCHEMESCENE_HXX

#include <QGraphicsScene>

namespace Schematics::Ui::Widgets
{
    class SchemeScene: public QGraphicsScene
    {
        Q_OBJECT
    public:
        explicit SchemeScene(QObject* parent = nullptr);
        ~SchemeScene() override;
    };
} // namespace Schematics::Ui::Widgets

#endif //SCHEMATICS_SCHEMESCENE_HXX
