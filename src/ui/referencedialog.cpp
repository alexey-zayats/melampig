#include "referencedialog.h"

#include <ui.h>

#include <QGridLayout>
#include <QBoxLayout>
#include <QPushButton>
#include <QMainWindow>

namespace Melampig
{

    ReferenceDialog::ReferenceDialog(Attr *a, Keeper *keeper, QWidget *parent) :
        QDialog(parent), attr(a)
    {
        ref = keeper->objectByType(attr->getRef());
        setObjectName("dialog" + ref->objectName());

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

        okButton = new QPushButton(tr("Select"), this);
        cancelButton = new QPushButton(tr("Cancel"), this);

        connect(okButton, SIGNAL(clicked()), this, SLOT(accept()));
        connect(cancelButton, SIGNAL(clicked()), this, SLOT(close()));

        vbox = new QBoxLayout(QBoxLayout::TopToBottom);
        vbox->addWidget(widget);

        hbox = new QBoxLayout(QBoxLayout::LeftToRight);
        hbox->addItem(new QSpacerItem(10, 10, QSizePolicy::Maximum));
        hbox->addWidget(okButton);
        hbox->addWidget(cancelButton);

        hbox->setMargin(0);

        vbox->addLayout(hbox);

        vbox->setMargin(0);

        setLayout(vbox);

        connect(widget, SIGNAL(selectionChanged()), this, SLOT(updateActions()));
    }

    void ReferenceDialog::updateActions()
    {
    }

    ReferenceDialog::~ReferenceDialog()
    {
        delete ref;
    }

    Object *ReferenceDialog::getObject()
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
        return item;
    }

}
