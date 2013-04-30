#ifndef WRESTLER_WEIGHING_H
#define WRESTLER_WEIGHING_H

#include <reportwidget.h>

namespace Melampig
{
    class Keeper;
    class Object;
    class WrestlerWeighing : public ReportWidget
    {
        Q_OBJECT
        public:
            WrestlerWeighing(Object *o, Keeper *keeper, QWidget *parent = 0);

        protected:
            QString generateData();
    };
}

#endif // WRESTLER_WEIGHING_H
