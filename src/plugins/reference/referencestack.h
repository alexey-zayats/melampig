#ifndef REFERENCESTACK_H
#define REFERENCESTACK_H

#include <QMainWindow>
#include <QModelIndex>

#include <orm.h>

namespace Ui {
    class ReferenceStack;
}

class QStringListModel;

namespace Melampig
{
	class Keeper;
	class ReferenceStack : public QMainWindow
	{
		Q_OBJECT

	public:
		explicit ReferenceStack(Keeper *keeper, QWidget *parent = 0);
		~ReferenceStack();

	public slots:
		void listView_activated(QModelIndex);

	private:
		Keeper *keeper;
		Ui::ReferenceStack *ui;
		QStringListModel *model;
        QWidget *parentWidget;

        QHash<QString, ObjectType> refs;
        QStringList itemList;
	};
}

#endif // REFERENCESTACK_H
