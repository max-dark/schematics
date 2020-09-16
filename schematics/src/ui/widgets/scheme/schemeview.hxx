#ifndef SCHEMATICS_SCHEMEVIEW_HXX
#define SCHEMATICS_SCHEMEVIEW_HXX

#include <QGraphicsView>

namespace Schematics::Ui::Widgets
{
    class SchemeScene;
    struct SchemeItems;

    /**
     *  \brief Cutting scheme visualisation
     *  \example Usage example
     *  \code
     *  for(auto sz: {35., 60., 35.})
     *  {
     *      addCentral(sz);
     *  }
     *  setCentralWidth(300.0);
     *  setPA300Size(250., 35.);
     *  setPA300Enabled(true);
     *  setPKA350Size(200, 25);
     *  setPKA350Enabled(true);
     *  setPA350Size(100, 19);
     *  setPA350Enabled(true);
     *  setVertical(true);
     *  setDiameter(350.0);
     *
     *  setCentralHeight(1, 40);
     *  \endcode
     *
     **/

    class SchemeView: public QGraphicsView
    {
        Q_OBJECT
    public /* methods */:
        explicit SchemeView(QWidget *parent = nullptr);
        ~SchemeView() override;

        void clear();
    public /* param ctrl */:
        void setDiameter(double diameter);
        void setVertical(bool isVertical);
        void setSawSizes(double dws, double pka);
    public /* dws350 */:
        void addCentral(double height);
        void setCentralWidth(double width);
        void setCentralHeight(int index, double height);
        void removeCentral(int index);
    public /* pa300 */:
        void setPA300Size(double width, double height);
        void setPA300Enabled(bool enabled);
    public /* pka350 */:
        void setPKA350Size(double width, double height);
        void setPKA350Enabled(bool enabled);
    public /* pa350 */:
        void setPA350Size(double width, double height);
        void setPA350Enabled(bool enabled);
    private /* members */:
        /// graphics
        SchemeScene* gfx = nullptr;
        /// internal engine
        SchemeItems* scheme = nullptr;
    };
} // namespace Schematics::Ui::Widgets

#endif //SCHEMATICS_SCHEMEVIEW_HXX
