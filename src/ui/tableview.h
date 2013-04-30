#ifndef MELAMPIG_TABLEVIEW_H
#define MELAMPIG_TABLEVIEW_H

#include <QTableView>
#include <exports.h>

namespace Melampig
{
	class MELAMPIG_UI_EXPORT TableView : public QTableView
	{
		Q_OBJECT
		public:
			TableView(QWidget * parent = 0);
			~TableView();
	};
}

#endif // MELAMPIG_TABLEVIEW_H
