#ifndef MELAMPIG_TEAMSCOREREPORT_H
#define MELAMPIG_TEAMSCOREREPORT_H

#include "reportbase.h"

#include <QStringList>
#include <QMap>

namespace Melampig
{
    class TeamScoreReport : public ReportBase
    {
        public:
            TeamScoreReport(Object*, Keeper*);
            virtual QString generate(bool);
        private:
            QString place(int num);
            void reportByRound(bool is_final);
            void groupRanking();
            void finalRanking();


            int style_id;
            QString style_title;

            int competition_id;
            QString competition_title;
            QMap<QString,QString> tmpls;

            QStringList pages;
            QMap<QString,QString> styleVars;
    };
}

#endif // MELAMPIG_TEAMSCOREREPORT_H
