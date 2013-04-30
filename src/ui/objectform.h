#ifndef OBJECTFORM_H
#define OBJECTFORM_H

#include <QMainWindow>
#include <orm.h>

class QScrollArea;
class QGridLayout;

namespace Melampig
{

    class MELAMPIG_UI_EXPORT ObjectForm : public QMainWindow
    {
        Q_OBJECT
    public:
        ObjectForm(Object *obj, Keeper *keeper, QWidget *parent = 0);
        ~ObjectForm();

    signals:
        void objectSaved(int);

    public slots:
        void saveObject();

    private:
        Object *object;
        Keeper *keeper;

	private:
		QWidget *getAttrWidget( Attr *a, QWidget *parent );
		QString getWidgetData( Attr *a, QWidget *editor );
		void setWidgetData( Attr *a, QWidget *w );

		QHash<QString, QWidget*> widgetMap;

		QScrollArea *scrollArea;
		QWidget *innerWidget;
		QGridLayout *innerLayout;

		QAction *saveAction;
		QToolBar *actionBar;

		QWidget *parentWidget;
	};

}

#endif // OBJECTFORM_H
