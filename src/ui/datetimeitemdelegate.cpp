#include "datetimeitemdelegate.h"

#include <QtGui>

namespace Melampig
{
    DateTimeItemDelegate::DateTimeItemDelegate(QObject *parent) :
        QItemDelegate(parent)
    {
    }

    QWidget *DateTimeItemDelegate::createEditor ( QWidget * parent, const QStyleOptionViewItem & /*option*/, const QModelIndex & /*index*/ ) const
    {
        QDateTimeEdit *edit = new QDateTimeEdit(parent);
        edit->setCalendarPopup(true);
        edit->setDisplayFormat("dd.MM.yyyy");
        return edit;
    }

    void DateTimeItemDelegate::setEditorData ( QWidget * editor, const QModelIndex & index ) const
    {
        QDateTimeEdit *edit = qobject_cast<QDateTimeEdit*>(editor);
        edit->setDateTime( QDateTime::fromString(index.data().toString(), "dd.MM.yyyy HH:mm:ss") );
    }

    void DateTimeItemDelegate::updateEditorGeometry ( QWidget * editor, const QStyleOptionViewItem & option, const QModelIndex & /*index*/ ) const
    {
        QRect r(option.rect.x(), option.rect.y(),
                option.rect.width(), option.rect.height());
        editor->setGeometry(r);
    }

    void DateTimeItemDelegate::setModelData ( QWidget * editor, QAbstractItemModel * model, const QModelIndex & index ) const
    {
        QDateTimeEdit *edit = qobject_cast<QDateTimeEdit*>(editor);
        model->setData(index, edit->dateTime(), Qt::EditRole );
    }
}
