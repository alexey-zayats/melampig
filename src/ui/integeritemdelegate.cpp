#include "integeritemdelegate.h"
#include <QtGui>

namespace Melampig
{
    IntegerItemDelegate::IntegerItemDelegate(QObject *parent) :
        QItemDelegate(parent)
    {
    }

    QWidget *IntegerItemDelegate::createEditor ( QWidget * parent, const QStyleOptionViewItem & /*option*/, const QModelIndex & /*index*/ ) const
    {
        QSpinBox *edit = new QSpinBox(parent);
        edit->setMaximum(99999);
        return edit;
    }

    void IntegerItemDelegate::setEditorData ( QWidget * editor, const QModelIndex & index ) const
    {
        QSpinBox *edit = qobject_cast<QSpinBox*>(editor);
        edit->setValue(index.data().toInt());
    }

    void IntegerItemDelegate::updateEditorGeometry ( QWidget * editor, const QStyleOptionViewItem & option, const QModelIndex & /*index*/ ) const
    {
        QRect r(option.rect.x(), option.rect.y(),
                option.rect.width(), option.rect.height());
        editor->setGeometry(r);
    }

    void IntegerItemDelegate::setModelData ( QWidget * editor, QAbstractItemModel * model, const QModelIndex & index ) const
    {
        QSpinBox *edit = qobject_cast<QSpinBox*>(editor);
        model->setData(index, edit->value(), Qt::EditRole );
    }

}
