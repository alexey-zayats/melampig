#ifndef TEAM_WEIGHING_H
#define TEAM_WEIGHING_H

#include <reportwidget.h>

namespace Melampig
{
    class Keeper;
    class Object;
    class TeamWeighing : public ReportWidget
    {
        Q_OBJECT
        public:
            TeamWeighing(Object *o, Keeper *keeper, QWidget *parent = 0);

        protected:
            virtual QString generateData();
    };

}

#endif // TEAM_WEIGHING_H
