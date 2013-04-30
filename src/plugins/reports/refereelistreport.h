#ifndef MELAMPIG_REFEREELISTREPORT_H
#define MELAMPIG_REFEREELISTREPORT_H

#include "reportbase.h"

namespace Melampig
{
	class RefereeListReport : public ReportBase
	{
		public:
			RefereeListReport(Object*, Keeper*);
			QString generate(bool);
	};
}

#endif // MELAMPIG_REFEREELISTREPORT_H
