#ifndef FIGHTSHEET_H
#define FIGHTSHEET_H

#include <reportwidget.h>

namespace Melampig
{
    class Keeper;
    class Object;
    class FightSheet : public ReportWidget
    {
        Q_OBJECT
        public:
            FightSheet(Object *o, Keeper *keeper, QWidget *parent = 0);

        protected:
            QString generateData();
    };

}

#endif // FIGHTSHEET_H
