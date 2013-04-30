#ifndef MELAMPIG_TREEVIEW_H
#define MELAMPIG_TREEVIEW_H

#include <QTreeView>
#include <exports.h>

namespace Melampig
{
	class MELAMPIG_UI_EXPORT TreeView : public QTreeView
	{
		Q_OBJECT
		public:
			TreeView(QWidget * parent = 0);
			~TreeView();
	};
}

#endif // MELAMPIG_TREE_VIEW_H
