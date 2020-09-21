#ifndef SCHEMATICS_UI_TOOL_HXX
#define SCHEMATICS_UI_TOOL_HXX

#include <QSpacerItem>
#include <QSizePolicy>

#include <QGridLayout>
#include <QLayoutItem>
#include <QSpacerItem>

#include <QWidget>

namespace Schematics::Ui::tool
{
    QSpacerItem* createVSpace();

    QSpacerItem* createHSpace();

    void addGridRow(QGridLayout* grid, QWidget* widget);

    void addGridRow(QGridLayout* grid, QLayoutItem* item);
} // namespace Schematics::Ui::tool

#endif //SCHEMATICS_UI_TOOL_HXX
