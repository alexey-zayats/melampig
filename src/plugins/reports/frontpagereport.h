#ifndef MELAMPIG_FRONTPAGEREPORT_H
#define MELAMPIG_FRONTPAGEREPORT_H

#include "reportbase.h"

namespace Melampig
{
	class FrontPageReport : public ReportBase
	{
		public:
			FrontPageReport(Object*, Keeper*);
			QString generate(bool);
	};
}

#endif // MELAMPIG_FRONTPAGEREPORT_H
