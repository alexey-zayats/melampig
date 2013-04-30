#include "referenceslector.h"

#include <ui.h>

#include <QGridLayout>
#include <QBoxLayout>
#include <QPushButton>
#include <QMainWindow>

namespace Melampig
{

    ReferenceSelector::ReferenceSelector(Attr *a, Keeper *keeper, QWidget *parent) :
        QMainWindow(parent), attr(a)
    {
        ref = keeper->objectByType(attr->getRef());
        setObjectName("dialog" + ref->objectName());

        QWidget *centralWidget = new QWidget(this);

        TQueryMap opt;
        switch (ref->getModelType())
        {
            case aList: {
                widget = new ListWidget(ref->getType(), keeper, opt, parent);
                break;
            }
            case aTable: {
                widget = new TableWidget(ref->getType(), keeper, opt, parent);
                break;
            }
            case aTree: {
                widget = new TreeWidget(ref->getType(), keeper, opt, parent);
                break;
            }
        }

        okButton = new QPushButton(tr("Select"), centralWidget);
        cancelButton = new QPushButton(tr("Cancel"), centralWidget);

        connect(okButton, SIGNAL(clicked()), this, SLOT(okButtonClicked()));
        connect(cancelButton, SIGNAL(clicked()), this, SLOT(close()));

        vbox = new QBoxLayout(QBoxLayout::TopToBottom, centralWidget);
        vbox->addWidget(widget);
        vbox->setMargin(0);

        hbox = new QBoxLayout(QBoxLayout::LeftToRight);
        hbox->addItem(new QSpacerItem(100, 10, QSizePolicy::Expanding));
        hbox->addWidget(okButton);
        hbox->addWidget(cancelButton);
        hbox->setMargin(0);

        vbox->addLayout(hbox);

        setCentralWidget(centralWidget);

        connect(widget, SIGNAL(selectionChanged()), this, SLOT(updateActions()));
    }

    void ReferenceSelector::updateActions()
    {
    }

    ReferenceSelector::~ReferenceSelector()
    {
        delete ref;
    }

    void ReferenceSelector::okButtonClicked()
    {
        Object *item = 0;
        switch (ref->getModelType())
        {
        case aList:
            item = ((ListWidget*)widget)->getSelectedObject();
            break;
        case aTable:
            item = ((TableWidget*)widget)->getSelectedObject();
            break;
        case aTree:
            item = ((TreeWidget*)widget)->getSelectedObject();
            break;
        }
        if ( item ) {
            emit itemSelected(item);
			close();
//            QTimer::singleShot(500, this, SLOT(close()));
        }
    }

}
