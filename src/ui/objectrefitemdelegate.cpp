#include "objectrefitemdelegate.h"

#include "objectselector.h"
#include "objectcompleter.h"
#include "treemodelcompleter.h"

#include "tablemodel.h"
#include "linkedtablemodel.h"

#include <QtGui>
#include <QtSql>
#include <ui.h>

#define COMPLETER 1

namespace Melampig
{
    ObjectRefItemDelegate::ObjectRefItemDelegate(Attr *a, Keeper *keeper, QObject *parent) :
        QItemDelegate(parent), attr(a), keeper(keeper)
    {

    }

    ObjectRefItemDelegate::~ObjectRefItemDelegate()
    {
    }

    bool ObjectRefItemDelegate::editorEvent ( QEvent * event, QAbstractItemModel * model, const QStyleOptionViewItem & option, const QModelIndex & index )
    {
        return QItemDelegate::editorEvent(event, model, option, index);
    }

    QWidget *ObjectRefItemDelegate::createEditor ( QWidget * parent, const QStyleOptionViewItem & /*option*/, const QModelIndex & /*index*/ ) const
    {
        if ( COMPLETER ) {
            ObjectCompleter *editor = new ObjectCompleter(attr, keeper, parentWidget, parent);
            return editor;
        } else {
            ObjectSelector *editor = new ObjectSelector(attr, keeper, parent);
            editor->setParentWidget(parentWidget);
            return editor;
        }
    }

    void ObjectRefItemDelegate::setEditorData ( QWidget * editor, const QModelIndex & index ) const
    {
        if ( COMPLETER ) {
            ObjectCompleter *lineEdit = qobject_cast<ObjectCompleter*>(editor);
            lineEdit->setModelIndex(index);
        } else {
            ObjectSelector *edit = qobject_cast<ObjectSelector*>(editor);
            edit->setReference(index.model()->data(index, Qt::EditRole).toString());
        }
    }

    void ObjectRefItemDelegate::updateEditorGeometry ( QWidget * editor, const QStyleOptionViewItem & option, const QModelIndex & /*index*/ ) const
    {
        QRect r(option.rect.x(), option.rect.y(), option.rect.width(), option.rect.height());
        if ( COMPLETER ) {
            QLineEdit *edit = qobject_cast<QLineEdit*>(editor);
            edit->setGeometry(r);
        } else {
            ObjectSelector *edit = qobject_cast<ObjectSelector*>(editor);
            edit->setGeometry(r);
        }
    }

    void ObjectRefItemDelegate::setModelData ( QWidget* editor,
                                               QAbstractItemModel *model,
                                               const QModelIndex & index ) const
    {
        if ( COMPLETER )
        {
            ObjectCompleter *lineEdit = qobject_cast<ObjectCompleter*>(editor);
            int value = lineEdit->getValue();
            if ( value == 0 ) return;
            model->setData(index, lineEdit->getValue(), Qt::EditRole);
        } else {
            ObjectSelector *edit = qobject_cast<ObjectSelector*>(editor);
            model->setData(index, Object::toList(edit->value()), Qt::EditRole);
        }

        TableModel *_model_0 = qobject_cast<TableModel*>(model);
        if ( _model_0 )
            _model_0->changeData(index, index);
        else {
            LinkedTableModel *_model_1 = qobject_cast<LinkedTableModel*>(model);
            if ( _model_1 ) _model_1->changeData(index, index);
        }
    }
}
