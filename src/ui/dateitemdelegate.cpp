#include "dateitemdelegate.h"

#include <QtGui>

namespace Melampig
{
    DateItemDelegate::DateItemDelegate(QObject *parent) :
        QItemDelegate(parent)
    {
    }

    QWidget *DateItemDelegate::createEditor ( QWidget * parent, const QStyleOptionViewItem & /*option*/, const QModelIndex & /*index*/ ) const
    {
        QDateEdit *edit = new QDateEdit(parent);
        edit->setCalendarPopup(true);
        edit->setDisplayFormat("dd.MM.yyyy");
        return edit;
    }

    void DateItemDelegate::setEditorData ( QWidget * editor, const QModelIndex & index ) const
    {
        QDateEdit *dateEdit = qobject_cast<QDateEdit*>(editor);
        dateEdit->setDate( QDate::fromString(index.data().toString(), "dd.MM.yyyy") );
    }

    void DateItemDelegate::updateEditorGeometry ( QWidget * editor, const QStyleOptionViewItem & option, const QModelIndex & /*index*/ ) const
    {
        QRect r(option.rect.x(), option.rect.y(),
                option.rect.width(), option.rect.height());
        editor->setGeometry(r);
    }

    void DateItemDelegate::setModelData ( QWidget * editor, QAbstractItemModel * model, const QModelIndex & index ) const
    {
        QDateEdit *dateEdit = qobject_cast<QDateEdit*>(editor);
        model->setData(index, dateEdit->date(), Qt::EditRole );
    }
}
