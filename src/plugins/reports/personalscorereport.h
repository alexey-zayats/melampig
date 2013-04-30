#ifndef MELAMPIG_PERSONALSCOREREPORT_H
#define MELAMPIG_PERSONALSCOREREPORT_H

#include "reportbase.h"

namespace Melampig
{
	class PersonalScoreReport : public ReportBase
	{
		public:
			PersonalScoreReport(Object*, Keeper*);
			virtual QString generate(bool);

		private:
			void genCompetition(int, int, int);
			void genRepechage(int, int, int);
			void genClassement(bool, int, int, int);

            QMap<QString,QString> pVars;
            QString page_tmpl;
	};
}

#endif // MELAMPIG_PERSONALSCOREREPORT_H
