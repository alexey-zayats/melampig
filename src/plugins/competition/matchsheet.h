#ifndef MATCHSHEET_H
#define MATCHSHEET_H

#include <reportwidget.h>

namespace Melampig
{
    class Keeper;
    class Object;
    class MatchSheet : public ReportWidget
    {
        Q_OBJECT
        public:
            MatchSheet(Object *o, Keeper *keeper, QWidget *parent = 0);

        protected:
            QString generateData();
    };

}

#endif // MATCHSHEET_H
