#include <ui/tools/tool.hxx>

namespace Schematics::Ui::tool
{
    QSpacerItem* createVSpace() {
        return new QSpacerItem{10, 10, QSizePolicy::Minimum, QSizePolicy::Expanding};
    }

    QSpacerItem* createHSpace() {
        return new QSpacerItem{10, 10, QSizePolicy::Expanding, QSizePolicy::Minimum};
    }

    void addGridRow(QGridLayout* grid, QWidget* widget)
    {
        grid->addWidget(widget, grid->rowCount(), 0, 1, -1);;
    }

    void addGridRow(QGridLayout* grid, QLayoutItem* item)
    {
        grid->addItem(item, grid->rowCount(), 0, 1, -1);;
    }
} // namespace Schematics::Ui::tool
