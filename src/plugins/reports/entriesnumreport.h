#ifndef MELAMPIG_ENTRIES_NUM_REPORT_H
#define MELAMPIG_ENTRIES_NUM_REPORT_H

#include "reportbase.h"

namespace Melampig
{
	class EntriesNumReport : public ReportBase
	{
		public:
			EntriesNumReport(Object*, Keeper*);
			virtual QString generate(bool);
	};
}

#endif // MELAMPIG_ENTRIES_NUM_REPORT_H

