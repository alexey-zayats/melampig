#include "listmodel.h"

namespace Melampig
{

    ListModel::ListModel(Object *obj, Keeper *keeper,  QObject *parent) :
        QAbstractListModel(parent), obj(obj), keeper(keeper)
    {

    }

    ListModel::~ListModel ()
    {

        qDeleteAll(record);
    }


    int ListModel::rowCount(const QModelIndex &/*parent*/) const
    {

        return rows.size();
    }

    bool ListModel::setData(const QModelIndex &index,
                            const QVariant &value, int role)
    {

        if (!index.isValid() || role != Qt::EditRole)
            return false;

        Attr *a = attr[titleIndex];
        if ( a == 0 )
            return false;

        Object *o = record[index.row()];
        o->set(a->getName(), value.toString());

        try
        {
            o->store();
        } catch ( SqlError &err ) {
            QMessageBox::critical( 0, QObject::tr("Error"), err.getText() + "\n" + err.getQuery() );
        }

        rows[index.row()] = o->get("id").toInt();

        emit dataChanged(index, index);

        return true;
    }

    QVariant ListModel::data(const QModelIndex &index, int role) const
    {
        if (!index.isValid())
            return QVariant();

        Attr *a = attr[titleIndex];
        if ( a == 0 )
            return QVariant();

        QString value = record[index.row()]->get( a->getName() );

        switch (role)
        {
            case Qt::DisplayRole:
            {
                if ( a->getType() == Attr::Text ) {
                    return value;
                } else if ( a->getType() == Attr::DateTime ) {
                    return QDateTime::fromString( value, "yyyy-MM-ddTHH:mm:ss" );
                } else if ( a->getType() == Attr::Svg ) {
                    return QString("[SVG]");
                } else {
                    return value;
                };
                break;
            }
            case Qt::EditRole: {
                if ( a->getType() == Attr::DateTime )
                    return QDateTime::fromString( value, "yyyy-MM-ddTHH:mm:ss" );
                else
                    return value;
                break;
            }
            case Qt::CheckStateRole:
                if ( a->getType() == Attr::Bool )
                    return value.compare("true") == 0 ? Qt::Checked : Qt::Unchecked;
                break;
            case Qt::UserRole:
                return record[index.row()]->get( "id" );
            default:
                break;
        }
        return QVariant();
    }

    QVariant ListModel::headerData(int section, Qt::Orientation orientation, int role) const
    {

        if ( orientation == Qt::Horizontal && role == Qt::DisplayRole )
            return attr[titleIndex]->getTitle();

        return QAbstractListModel::headerData( section, orientation, role );
    }

    Qt::ItemFlags ListModel::flags(const QModelIndex &index) const
    {
        if (!index.isValid())
            return Qt::ItemIsEnabled;

        Qt::ItemFlags flags = Qt::ItemIsSelectable;

        if ( attr[titleIndex]->isReadOnly() == false ) {
            flags |= Qt::ItemIsEditable;
        };

        if ( attr[titleIndex]->isVisible() == true ) {
            flags |= Qt::ItemIsEnabled;
        };

        if ( attr[titleIndex]->getType() == Attr::Bool ) {
            flags |= Qt::ItemIsUserCheckable;
            flags |= Qt::ItemIsTristate;
        };

        if ( attr[titleIndex]->getRef() != ONull ) {
            flags ^= Qt::ItemIsEditable;
        };

        if ( attr[titleIndex]->getType() == Attr::Svg ) {
            flags ^= Qt::ItemIsEnabled;
        };

        if ( checkable ) {
            flags |= Qt::ItemIsUserCheckable;
            flags |= Qt::ItemIsTristate;
        }

        return flags;
    }

    bool ListModel::insertRows(int position, int count, const QModelIndex &parent)
    {
        beginInsertRows(parent, position, position + count - 1 );

        Object *o = keeper->objectByType( obj->getType() );
        record.insert(position, o);
        rows.insert(position, 0);

        endInsertRows();

        return true;
    }

    bool ListModel::removeRows(int position, int /*count*/, const QModelIndex &parent)
    {
        beginRemoveRows(parent, position, position );

        rows.takeAt(position);

        Object *o = record.takeAt(position);
        o->remove();
        delete o;

        endRemoveRows();

        return true;
    }


    void ListModel::initModel()
    {
        attr = obj->attrSchema();
        for( int i = 0; i < attr.size(); i++) {
            if ( attr.at(i)->getName().compare("title") == 0 ) {
                titleIndex = i;
            }
            setHeaderData(i, Qt::Horizontal, attr.at(i)->getTitle() );
        }

        TQueryMap opts;
        rows = keeper->getFieldList(obj->getType(), "id", opts);

        fetchMore(QModelIndex());

        reset();
    }

    bool ListModel::canFetchMore ( const QModelIndex &/*parent*/) const
    {
        return record.size() < rows.size();
    }

    void ListModel::fetchMore ( const QModelIndex &/*parent*/)
    {
        TQueryMap opts;
        Object *o = 0;
        QList<Object*> list = keeper->getObjectList(obj->getType(), opts);
        for( int i = 0; i < list.size(); i++)
        {
            o = list.at(i);
            record.insert(i, o);
        }
    }

    Object *ListModel::getObject(const QModelIndex &index)
    {
        if (!index.isValid())
            return 0;

        return record[index.row()];
    }

    void ListModel::changeData( const QModelIndex &from, const QModelIndex &to )
    {
        Object *o = record[from.row()];
        o->restore( o->get("id").toInt() );

        emit dataChanged(from, to);
    }
}

