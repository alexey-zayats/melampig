#include "objectcompleter.h"
#include "referencedialog.h"
#include "mainwindow.h"
#include "objectform.h"
#include "referenceslector.h"

#include <QtGui>
#include <QtSql>

namespace Melampig
{
    ObjectCompleter::ObjectCompleter(Attr *a, Keeper *keeper, QWidget *pw, QWidget *parent) :
        QLineEdit(parent), attr(a), keeper(keeper), parentWidget(pw)
    {
         obj = keeper->objectByType(attr->getRef());

         model = new QSqlQueryModel(parent);
         ((QSqlQueryModel*)model)->setQuery( QString("SELECT id, title FROM %1").arg(obj->getTable()), keeper->database() );

         completer = new QCompleter(model, parent);
         completer->setCompletionColumn(1);
         completer->setCaseSensitivity(Qt::CaseInsensitive);

         setCompleter(completer);

         connect(completer, SIGNAL(activated(QModelIndex)), this, SLOT(setCompletedIndex(QModelIndex)));

         findButton = new QToolButton(this);
         findButton->setIcon(QIcon(":icons/magnifier.png"));
         findButton->setCursor(Qt::ArrowCursor);
         findButton->setStyleSheet("QToolButton { border: none; padding: 0px; }");
         connect(findButton, SIGNAL(clicked()), this, SLOT(findReference()));

         clearButton = new QToolButton(this);
         clearButton->setIcon(QIcon(":icons/delete.png"));
         clearButton->setCursor(Qt::ArrowCursor);
         clearButton->setStyleSheet("QToolButton { border: none; padding: 0px; }");
         connect(clearButton, SIGNAL(clicked()), this, SLOT(clearReference()));

         int frameWidth = style()->pixelMetric(QStyle::PM_DefaultFrameWidth);
         setStyleSheet(QString("QLineEdit { padding-right: %1px; } ").
                       arg(findButton->sizeHint().width() + clearButton->sizeHint().width() + frameWidth + 1));

         QSize msz = minimumSizeHint();
         setMinimumSize(qMax(msz.width(), clearButton->sizeHint().width() + findButton->sizeHint().width() + frameWidth * 2 + 2),
                        qMax(msz.height(), findButton->sizeHint().height() + frameWidth * 2 + 2));

         workspace = (QWorkspace*)parentWidget;
         mainWindow = ((MainWindow*)workspace->parentWidget());
    }

    ObjectCompleter::~ObjectCompleter()
    {
        delete obj;
    }

    bool ObjectCompleter::event(QEvent *event)
     {
         if (event->type() == QEvent::KeyPress) {
             QKeyEvent *ke = static_cast<QKeyEvent *>(event);
             if (ke->key() == Qt::Key_Down) {
                 completer->setCompletionMode(QCompleter::UnfilteredPopupCompletion);
                 completer->complete();
                 return false;
             } else if ( ke->key() == Qt::Key_Insert ) {
                 findReference();
                 return false;
             }
         }

         return QWidget::event(event);
     }

    void ObjectCompleter::setCompletedIndex(QModelIndex index)
    {
        id = index.model()->data(index.model()->index(index.row(), 0)).toInt();
        data = index.model()->data(index.model()->index(index.row(), 1)).toString();
    }


    int ObjectCompleter::getValue()
    {
        if ( qAbs(id) != id ) {

            try
            {
                data = text();
                obj->set("title", text() );
                obj->store();
            } catch ( SqlError &err ) {
                QMessageBox::critical( 0, QObject::tr("Error"), err.getText() + "\n" + err.getQuery() );
            }

            id = obj->get("id").toInt();

            ObjectForm *form = new ObjectForm(obj, keeper, parentWidget);
            form->setWindowTitle(data);
            form->setAttribute(Qt::WA_DeleteOnClose, true);
            form->setWindowIcon( windowIcon() );
            form->setWindowModality(Qt::ApplicationModal);

            workspace->addWindow(form);
            form->show();

            connect(form, SIGNAL(objectSaved(int)), this, SLOT(objectForm_saved(int)) );
            connect( form, SIGNAL( destroyed() ), mainWindow, SLOT( closeTab() ) );
        }

        return id;
    }

    void ObjectCompleter::objectForm_saved(int x)
    {
        setText(obj->get("title"));
        id = x;
    }

    void ObjectCompleter::clearReference()
    {
        data.clear();
        id = 0;
        setText( data );
    }

    void ObjectCompleter::setModelIndex(QModelIndex index)
    {
        QModelIndexList list = model->match( model->index(0, 0),
                                             Qt::DisplayRole,
                                             index.model()->data(index, Qt::EditRole));

        if ( list.size() == 1 ) {
            QModelIndex ix = list.at(0);
            data = ix.model()->data(ix.model()->index(ix.row(), 1)).toString();
            id = ix.model()->data(ix.model()->index(ix.row(), 0)).toInt();
            setText( data );
        }
    }


    void ObjectCompleter::findReference()
    {
        if ( 0 ) {
            ReferenceDialog *dialog = new ReferenceDialog(attr, keeper, this);
            if ( dialog->exec() == QDialog::Accepted )
            {
                Object *o = dialog->getObject();
                if ( o == 0 ) return;

                id = o->get("id").toInt();
                data = o->get("title");
                setText(data);
            }
        } else {
            ReferenceSelector *widget = new ReferenceSelector(attr, keeper, this);
            widget->setAttribute(Qt::WA_DeleteOnClose);
            connect(widget, SIGNAL(itemSelected(Object*)), this, SLOT(setReferenceData(Object*)));
            widget->show();
        }
    }

    void ObjectCompleter::setReferenceData(Object *o) {
        if ( o == 0 ) return;
        id = o->get("id").toInt();
        data = o->get("title");
        setText(data);
    }


    void ObjectCompleter::resizeEvent(QResizeEvent *)
    {
        QSize sz1 = findButton->sizeHint();
        QSize sz2 = clearButton->sizeHint();

        int frameWidth = style()->pixelMetric(QStyle::PM_DefaultFrameWidth);

        findButton->move(rect().right() - frameWidth - sz1.width() - sz2.width(),
                          (rect().bottom() + 1 - sz1.height())/2);

        clearButton->move(rect().right() - frameWidth - sz2.width(),
                          (rect().bottom() + 1 - sz2.height())/2);
    }
}
