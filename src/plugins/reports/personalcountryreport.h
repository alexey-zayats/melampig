#ifndef MELAMPIG_PERSONAL_COUNTRY_REPORT_H
#define MELAMPIG_PERSONAL_COUNTRY_REPORT_H

#include "reportbase.h"

namespace Melampig
{
	class PersonalCountryReport : public ReportBase
	{
		public:
			PersonalCountryReport(Object*, Keeper*);
			virtual QString generate(bool);
	};
}

#endif // MELAMPIG_PERSONAL_COUNTRY_REPORT_H
