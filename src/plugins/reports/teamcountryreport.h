#ifndef MELAMPIG_TEAMCOUNTRYREPORT_H
#define MELAMPIG_TEAMCOUNTRYREPORT_H

#include "reportbase.h"

namespace Melampig
{
	class TeamCountryReport : public ReportBase
	{
		public:
			TeamCountryReport(Object*, Keeper*);
			virtual QString generate(bool);
	};
}

#endif // MELAMPIG_TEAMCOUNTRYREPORT_H
