#include "objectselector.h"
#include "referenceslector.h"
#include "mainwindow.h"

#include <QtGui>

namespace Melampig
{
    ObjectSelector::ObjectSelector(Attr *a, Keeper *keeper, QWidget *parent) :
        QWidget(parent), attr(a), keeper(keeper)
    {
        findButton = new QToolButton(this);
        findButton->setIcon( QIcon( ":/icons/magnifier.png" ));

        clearButton = new QToolButton(this);
        clearButton->setIcon( QIcon(":/icons/delete.png") );

        if ( a->isArray() ) {
            listWidget = new QListWidget(this);
            deleteButton = new QToolButton(this);
            deleteButton->setIcon( QIcon( ":/icons/cancel.png" ));

            QGridLayout *layout = new QGridLayout(this);
            layout->addWidget(listWidget, 0, 0, 2, 1);
            layout->addWidget(findButton, 0, 1);
            layout->addWidget(deleteButton, 1, 1);

            setLayout(layout);

            layout->setSpacing(2);
            layout->setMargin(2);

            setMinimumHeight(100);
            listWidget->setFocus();

            connect(deleteButton, SIGNAL(clicked()), this, SLOT(removeReference()));

        } else {
            lineEdit = new QLineEdit(this);
            lineEdit->setReadOnly(true);

            connect(lineEdit, SIGNAL(textChanged(QString)), this, SLOT(lineEdit_textChanged(QString)));

            QBoxLayout *layout = new QBoxLayout(QBoxLayout::LeftToRight, this);
            layout->addWidget(lineEdit);
            layout->addWidget(findButton);
            layout->addWidget(clearButton);

            setLayout(layout);

            layout->setSpacing(2);
            layout->setMargin(2);
            lineEdit->setFocus();
        }

        setAutoFillBackground(true);
        connect(findButton, SIGNAL(clicked()), this, SLOT(findReference()));
        connect(clearButton, SIGNAL(clicked()), this, SLOT(clearReferences()));
    }

    ObjectSelector::~ObjectSelector()
    {
        while( ref.size() )
            delete ref.takeFirst();
    }

    void ObjectSelector::lineEdit_textChanged(QString)
    {

    }

    void ObjectSelector::clearReferences ()
    {
        while( ref.size() )
            delete ref.takeFirst();

        if ( attr->isArray() ) {
            listWidget->clear();
        } else {
            lineEdit->clear();
        }
    }

    void ObjectSelector::removeReference()
    {
        int row = listWidget->currentRow();
        if ( row >= 0 ) {
            delete listWidget->takeItem( listWidget->currentRow() );
            delete ref.takeAt(row);
        }
    }

    void ObjectSelector::addReference(const QString &id)
    {
        QStringList idList;
        if ( attr->isArray() ) {
            idList = Object::toList(id);
        } else {
            idList << id;
        }

        for (int i = 0; i < idList.size(); i++)
        {
            if ( idList.at(i).toInt() == 0 ) return;

            ref.insert( i, keeper->objectByType(attr->getRef()) );
            ref.at(i)->restore(idList.at(i).toInt());
            if ( attr->isArray() ) {
                listWidget->addItem( ref.at(i)->get("title") );
            } else {
                lineEdit->setText( ref.at(i)->get("title") );
            }
;        }
    }

    void ObjectSelector::setReference(const QString &id)
    {
        if ( attr->isArray() )
            listWidget->clear();

        while( ref.size() )
            delete ref.takeFirst();

        addReference(id);
    }

    QString ObjectSelector::value()
    {
        QStringList idList;
        for (int i = 0; i < ref.size(); i++) {
            idList << ref.at(i)->get("id");
        }
        if ( attr->isArray() ) {
            return Object::toArray(idList);
        } else {
            return idList.size() ? idList.at(0) : QString();
        }
    }

    void ObjectSelector::setParentWidget(QWidget*w) {
        parentWidget = w;
        mainWindow = ((MainWindow*)((QWorkspace*)parentWidget)->parentWidget());
        workspace = (QWorkspace*)parentWidget;
    }


    void ObjectSelector::findReference()
    {
        QString title( tr("Choose %1 from list") );
        title = title.arg( attr->getTitle() );

		QWidgetList windows = workspace->windowList();
		for ( int j = 0; j < windows.size(); ++j )
			if ( windows.at(j)->windowTitle().compare( title ) == 0 ) {
				workspace->setActiveWindow(windows.at(j));
				return;
			}

        ReferenceSelector *widget = new ReferenceSelector(attr, keeper, workspace);
        widget->setAttribute(Qt::WA_DeleteOnClose, true);

        connect( widget, SIGNAL( destroyed() ), mainWindow, SLOT( closeTab() ) );

        workspace->addWindow(widget);
        widget->show();

        connect(widget, SIGNAL(itemSelected(Object*)), this, SLOT(referenceSelected(Object*)));
    }

    void ObjectSelector::referenceSelected(Object*o)
    {
        if ( o == 0 ) return;

        addReference(o->get("id"));
		emit objectSelected(o);
    }
}
