#include "tablemodel.h"
#include "svgitemdelegate.h"
#include "dateitemdelegate.h"
#include "datetimeitemdelegate.h"
#include "objectrefitemdelegate.h"
#include "integeritemdelegate.h"

#include <QtGui>

namespace Melampig
{

    TableModel::TableModel(Object *obj, Keeper *keeper, QObject *parent) :
        QAbstractTableModel(parent), obj(obj), keeper(keeper), parentWidget((QWidget*)parent)
    {
        rows = 0;
    }

    TableModel::~TableModel ()
    {
        qDeleteAll(record);
        record.clear();
    }

	int TableModel::rowCount(const QModelIndex &/*parent*/) const
	{
		return rows;
	}

	int TableModel::columnCount(const QModelIndex &/*parent*/) const
	{
		return attr.size();
	}

	QVariant TableModel::data(const QModelIndex &index, int role) const
	{
		if (!index.isValid())
			return QVariant();

        int col = index.column();
        if ( col < 0 || col >= attr.size() )
            return QVariant();

        int row = index.row();
        if ( row < 0 || row >= record.size() )
            return QVariant();

        Attr *a = attr[col];
        if ( a == 0 )
            return QVariant();

        QString value = record[index.row()]->get( a->getName() );

        switch (role)
        {
            case Qt::DisplayRole:
            {
                if ( a->getType() == Attr::Text )
                    return value;
                else if ( a->getType() == Attr::DateTime ) {
                    QDateTime dt = value.isEmpty() ? QDateTime::currentDateTime():
                                                     QDateTime::fromString( value, "yyyy-MM-ddTHH:mm:ss" );
                    return dt.toString("dd.MM.yyyy HH:mm:ss");
                    //                return dt;
                }
                else if ( a->getType() == Attr::Date ) {
                    QDate d = value.isEmpty() ? QDate::currentDate() :
                                                QDate::fromString( value, "yyyy-MM-dd" );
                    return d.toString("dd.MM.yyyy");
                    //                return d;
                } else if ( a->getType() == Attr::Time ) {
                    QTime t = value.isEmpty() ? QTime::currentTime() :
                                                QTime::fromString( value, "HH:mm:ss" );
                    return t.toString("HH:mm:ss");
                    //                return t;
                }
                else if ( a->getType() == Attr::ObjectRef )
                {
                    QStringList idList;
                    if ( a->isArray() )
                        idList << Object::toList(value);
                    else
                        idList << value;

                    ObjectType type = a->getRef();
                    if ( !refs.contains(type) )
                        return value;

                    QStringList out;
                    QString id;
                    foreach (id, idList) {
                        int  i = id.toInt();
                        if ( refs[type].contains(i) )
                            out.append(refs[type][i].toString());
                    }
                    return out.join(",");
                } else if ( a->getType() == Attr::Svg ) {
                    return QString("[SVG]");
                } else if ( a->getType() == Attr::Bool ) {
                    return value.compare("true") == 0 ? true : false;
                } else {
                    return value;
                };
                break;
            }
            case Qt::EditRole: {
                if ( a->getType() == Attr::DateTime ) {
                    QDateTime dt = value.isEmpty() ? QDateTime::currentDateTime() :
                                                     QDateTime::fromString( value, "yyyy-MM-ddTHH:mm:ss" );
                    //                return dt.toString("dd.MM.yyyy HH:mm:ss");
                    return dt;
                } else if ( a->getType() == Attr::Date ) {
                    QDate d = value.isEmpty() ? QDate::currentDate() :
                                                QDate::fromString( value, "yyyy-MM-dd" );
                    //                return d.toString("dd.MM.yyyy");
                    return d;
                } else if ( a->getType() == Attr::Time ) {
                    QTime t = value.isEmpty() ? QTime::currentTime() :
                                                QTime::fromString( value, "HH:mm:ss" );
                    //                return t.toString("HH:mm:ss");
                    return t;
                } else if ( a->getType() == Attr::Bool )
                    return value.compare("true") == 0 ? true : false;
                else
                    return value;
                break;
            }
            case Qt::CheckStateRole:
                if ( a->getType() == Attr::Bool ) {
                    return value.compare("true") == 0 ? Qt::Checked : Qt::Unchecked;
                }
                break;
            case Qt::UserRole:
                return record[index.row()]->get( "id" );
            default:
                break;
        }
        return QVariant();
    }

	bool TableModel::canFetchMore ( const QModelIndex &/*parent*/) const
	{
		return record.size() < rows;
	}

	void TableModel::fetchMore ( const QModelIndex &/*parent*/)
	{
		Object *o = 0;
		TQueryMap opt = options;
		// opt.insert("order", QStringList() << "id" );
		QList<Object*> list = keeper->getObjectList(obj->getType(), opt);
		for( int i = 0; i < list.size(); i++)
		{
			o = list.at(i);
			record.insert(i, o);

			if ( refName.size() == 0 )
				continue;

			QHash<ObjectType,QString>::iterator it;
			for( it = refName.begin(); it != refName.end(); it++)
			{
				ObjectType t = it.key();
				QString name = it.value();
				Attr *a = obj->attribute(name);
				QStringList id;
				if ( a->isArray() ) {
					id << Object::toList(o->get(name));
				} else {
					id << o->get(name);
				}
				foreach (QString x, id) {
					if ( x.toInt() == 0 ) continue;
					if ( refId.contains(t) ) {
						refId[t].append(x.toInt());
					} else {
						QList<QVariant> a;
						a.append(x.toInt());
						refId.insert(t, a);
					}
				}
			}
		}

		QHash<ObjectType, QList<QVariant> >::const_iterator it;
		for( it = refId.begin(); it != refId.end(); ++it )
		{
			fillRefMap(it.key(), it.value());
		}
	}

	void TableModel::fillRefMap(const ObjectType &type, const QList<QVariant> &idlist)
	{
		QList<QVariant> data;
		for( int i = 0; i < idlist.size(); i++ )
		{
			if ( !data.contains(idlist.at(i))) {
				data.append(idlist.at(i));
			}
		}

		TQueryMap opts;
		opts.insert("id", keeper->prepareParam(InSet, data) );
		opts.insert("order", QStringList() << "id" );

		QStringList flist;
		flist << "id";
		flist << "title";

		QList<QVariant> list = keeper->getListOfFields(type, flist, opts);

		QHash<int, QVariant> map;
		for( int i = 0; i < list.size(); i++) {
			QStringList fd = list.at(i).toStringList();
			map.insert(fd.at(0).toInt(), fd.at(1));
		}


		refs.insert( type, map );
	}

    QVariant TableModel::headerData(int section, Qt::Orientation orientation, int role) const
    {
        if ( orientation == Qt::Horizontal && role == Qt::DisplayRole )
            return attr[section]->getTitle();

        return QAbstractTableModel::headerData( section, orientation, role );
    }

    Qt::ItemFlags TableModel::flags(const QModelIndex &index) const
    {
        if (!index.isValid())
            return Qt::ItemIsEnabled;

        Qt::ItemFlags flags = Qt::ItemIsEnabled |  Qt::ItemIsSelectable;

        int col = index.column();

        if ( attr[col]->isReadOnly() == false ) {
            flags |= Qt::ItemIsEditable;
        };

        if ( attr[col]->getType() == Attr::Bool ) {
            flags |= Qt::ItemIsUserCheckable;
            flags |= Qt::ItemIsTristate;
        };

        return flags;
    }

    bool TableModel::setData(const QModelIndex &index,
                            const QVariant &value, int role)
    {
        if (!index.isValid() || role != Qt::EditRole)
            return false;

        int col = index.column();
        if ( col < 0 || col >= attr.size() )
            return false;

        Attr *a = attr[col];
        if ( a == 0 )
            return false;

        Object *o = record[index.row()];
        QString aname = a->getName();

        QString oldValue = o->get(aname);

        try
        {
            switch(a->getType())
            {
            case Attr::DateTime:
                o->set(aname, value.toDateTime().toString("yyyy-MM-ddTHH:mm:ss"));
                break;
            case Attr::Date:
                o->set(aname, value.toDate().toString("yyyy-MM-dd"));
                break;
            case Attr::Time:
                o->set(aname, value.toTime().toString("HH:mm:ss"));
                break;
            case Attr::Bool:
                o->set(aname, QString(value.toBool() ? "true" : "false"));
                break;
            default: {
                    o->set(aname, a->isArray() ? Object::toArray(value.toStringList()) :  value.toString());
                    break;
                }
            }

            o->store();
        } catch ( SqlError &err ) {
            o->set(aname, oldValue);
            QMessageBox::critical( 0, QObject::tr("Error"), err.getText() + "\n" + err.getQuery() );
        }

        emit dataChanged(index, index);
        //rows[index.row()] = o->get("id").toInt();



        return true;
    }

    void TableModel::changeData( const QModelIndex &from, const QModelIndex &to )
    {
        if (!from.isValid()) return;
        if (!to.isValid()) return;

        Object *o = record[from.row()];
        if (!o) return;

        o->restore( o->get("id").toInt() );

		QHash<ObjectType,QString>::iterator it;
		for( it = refName.begin(); it != refName.end(); it++)
		{
			ObjectType t = it.key();
			QString name = it.value();
			Attr *a = obj->attribute(name);
			QStringList id;
			if ( a->isArray() ) {
				id << Object::toList(o->get(name));
			} else {
				id << o->get(name);
			}
			foreach (QString x, id) {
				if ( x.toInt() == 0 ) continue;
				if ( refId.contains(t) ) {
					refId[t].append(x.toInt());
				} else {
					QList<QVariant> a;
					a.append(x.toInt());
					refId.insert(t, a);
				}
			}
		}

		QHash<ObjectType, QList<QVariant> >::const_iterator it1;
		for( it1 = refId.begin(); it1 != refId.end(); ++it1 )
		{
			fillRefMap(it1.key(), it1.value());
		}

        emit dataChanged(from, to);
    }

    bool TableModel::insertRows(int position, int count, const QModelIndex &parent)
    {
        beginInsertRows(parent, position, position + count - 1 );

        Object *o = keeper->objectByType( obj->getType() );

        TQueryMap::Iterator it = options.begin();
        for(; it != options.end(); ++it)
        {
            QList<QVariant> list = it.value().toList();
            if ( list.size() == 2 ) {
                o->set(it.key(), list.at(1).toString() );
            }
        }

        record.insert(position, o);
        rows++;

        endInsertRows();

        return true;
    }

    bool TableModel::removeRows(int position, int /*count*/, const QModelIndex &parent)
    {
        beginRemoveRows(parent, position, position );

        rows--;

        Object *o = record.takeAt(position);
        o->remove();
        delete o;

        endRemoveRows();

        return true;
    }

    void TableModel::setQueryOptions(const TQueryMap &opt)
    {
        options = opt;
    }

    void TableModel::initModel()
    {
        qDeleteAll(record);
        record.clear();
        attr.clear();

        Attr *a = 0;
        views = obj->getViewItem();
        for( int i = 0; i < views.size(); i++)
        {
            a = obj->attribute(views.at(i));
            attr.append(a);
            if ( a->getType() == Attr::ObjectRef ) {
                refName.insertMulti(a->getRef(), a->getName());
            }

            setHeaderData(i, Qt::Horizontal, a->getTitle() );
        }

        TQueryMap opts = options;
        opts.remove("order");
        rows = keeper->countObjects(obj->getType(), opts);

        options.insert("order", QStringList() << "id" );

        fetchMore(QModelIndex());

        reset();
    }

    QItemDelegate *TableModel::getItemDelegate(int idx)
    {
        Attr *a = attr.at(idx);
        switch(a->getType())
        {
        case Attr::ObjectRef: {
            ObjectRefItemDelegate *w = new ObjectRefItemDelegate(a, keeper, this);
            w->setParentWidget(parentWidget);
            return w;
        }
        case Attr::DateTime:
            return new DateTimeItemDelegate(this);
        case Attr::Date:
            return new DateItemDelegate(this);
        case Attr::Svg:
            return new SvgItemDelegate(this);
        case Attr::Int:
            return new IntegerItemDelegate(this);
        default:
            return new QItemDelegate(this);
        }
    }

    Object *TableModel::getObject(const QModelIndex &index)
    {
        if (!index.isValid())
            return 0;

        return record[index.row()];
    }

    void TableModel::sort ( int column, Qt::SortOrder order )
    {
        options.insert("order", QStringList() << QString("%1 %2").arg(attr.at(column)->getName()).arg(order == Qt::AscendingOrder ? "asc" : "desc") );

        qDeleteAll(record);
        record.clear();

        fetchMore(QModelIndex());

        reset();
    }
}
