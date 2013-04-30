#ifndef MELAMPIG_ENTRIES_LIST_REPORT_H
#define MELAMPIG_ENTRIES_LIST_REPORT_H

#include "reportbase.h"

namespace Melampig
{
	class EntriesListReport : public ReportBase
	{
		public:
			EntriesListReport(Object*, Keeper*);
			virtual QString generate(bool);
	};
}

#endif // MELAMPIG_ENTRIES_LIST_REPORT_H
