#ifndef AWARDREPORT_H
#define AWARDREPORT_H

#include <reportwidget.h>

namespace Melampig
{
    class Keeper;
    class Object;
    class AwardReport : public ReportWidget
    {
        Q_OBJECT
        public:
            AwardReport(Object *o, Keeper *keeper, QWidget *parent = 0);

        protected:
            virtual QString generateData();
    };

}

#endif // AWARDREPORT_H
