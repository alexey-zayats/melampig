// keeper.cpp

#include <QSqlError>
#include <QString>
#include <QByteArray>
#include <QCoreApplication>
#include <QSqlQuery>
#include <QDebug>

#include "keeper.h"
#include "orm.h"

// TODO:
// 1. check default schema on connection
// 2. fetch user on connection

namespace Melampig
{
    Keeper::Keeper() : QObject()
    {
        setObjectName("CMS::Keeper");
    }

    Keeper::~Keeper()
    {
        close();
    }

    void Keeper::open( const QHash<QString,QString> &opt )
    {
        connectOpt = opt;
        alias = connectOpt["alias"];
        db = QSqlDatabase::addDatabase( "QPSQL", alias );
        db.setDatabaseName ( connectOpt[ "dbname" ] );
        db.setUserName ( connectOpt["user"] );
        db.setPassword ( connectOpt["password"] );
        db.setHostName ( connectOpt["host"] );
        db.setPort ( connectOpt["port"].toInt() );
        db.setConnectOptions ( connectOpt["options"] );
        if ( !db.open() )
        {
            QStringList query;
            query.append( "Connection error" );
            for ( QHash<QString, QString>::const_iterator it = opt.constBegin(); it != opt.constEnd(); ++it )
                if ( it.key().compare("password") )
                    query.append( it.key() + "=" + it.value() );

            throw SqlError( db.lastError().text(), query.join("\n") );
        }

        roleName = connectOpt["user"];
        rolePass = connectOpt["password"];
    }

    void Keeper::reOpen()
    {
        open( connectOpt );
    }

    void Keeper::close()
    {
        db.close();
        QSqlDatabase::removeDatabase( alias );
    }

    TQueryParam Keeper::prepareParam( int flags, TQueryParam &values ) {
        TQueryParam tmp;
        tmp.append( flags );
        tmp += values;
        return tmp;
    }

    TQueryParam Keeper::prepareParam( int flags, QVariant value )
    {
        TQueryParam tmp;
        tmp.append( flags );
        tmp.append( value );
        return tmp;
    }

    QSqlQuery Keeper::parseQueryMap( const QString &table, const QStringList &fileds, const QStringList &wList, const TQueryMap &opts )
    {
        if ( !db.isOpen() ) reOpen();

        QString sql;
        TQueryMap opt = opts;
        QStringList names = wList;

        // clearing buffers
        binds.clear();
        wheres.clear();
        orders.clear();
        groups.clear();
        offset.clear();
        limit.clear();

        if ( opt.contains( "order" ) )
        {
            orders = " ORDER BY " + opt[ "order" ].toStringList().join( ", " );
        }

        if ( opt.contains( "group" ) )
        {
            QStringList params = opt[ "group" ].toStringList();
            groups = " GROUP BY " + params.join( ", " );
        }

        for ( QStringList::Iterator it = names.begin(); it != names.end(); ++it ) {
            if ( opt.contains( *it ) ) parseQueryParam( *it, opt[ *it ] );
        }

        if ( opt.contains( "limit" ) ) limit = QString( " LIMIT %1" ).arg( opt[ "limit" ].toInt() );
        if ( opt.contains( "offset" ) ) offset = QString( " OFFSET %1" ).arg( opt[ "offset" ].toInt() );

        // perform sql computing
        sql = QString( "SELECT %1 FROM %2" ).arg( fileds.join(", ") ).arg( table );

        if ( wheres.count() ) {
            sql.append(" WHERE ");
            sql.append( wheres.join( " " ) );
        }

        sql.append( groups );
        sql.append( orders );
        sql.append( limit );
        sql.append( offset );

        QSqlQuery query( db );
        query.prepare( sql );

        qDebug() << sql << binds;

        for( QList<QVariant>::Iterator it = binds.begin(); it != binds.end(); ++it )
            query.addBindValue( (*it) );

        return query;
    }

    void Keeper::parseQueryParam( const QString &name, const QVariant &params )
    {
        QStringList values;
        TQueryParam list = params.toList();
        TQueryParam::Iterator it = list.begin();
        int flag = (*it++).toInt();

        if ( flag & And ) wheres.append( "AND" ), flag ^= And;
        else if ( flag & Or ) wheres.append( "OR" ), flag ^= Or;

        switch ( flag ) {
        case Is:
            wheres.append( QString( "( %1 IS ? )" ).arg( name ) );
            binds.append( *it );
            break;
        case Is | Field:
            wheres.append( QString( "( %1 IS %2 )" ).arg( name ).arg( (*it).toString() ) );
            break;
        case Equal:
            wheres.append( QString( "( %1 = ? )" ).arg( name ) );
            binds.append( *it );
            break;
        case ( Equal | Field ):
            wheres.append( QString( "( %1 = %2 )" ).arg( name ).arg( (*it).toString() ) );
            break;
        case (Not | Equal):
            wheres.append( QString( "( %1 <> ? )" ).arg( name ) );
            binds.append( *it );
            break;
        case ( Not | Equal | Field ):
            wheres.append( QString( "( %1 <> %2 )" ).arg( name ).arg( (*it).toString() ) );
            break;
        case Greater:
            wheres.append( QString( "( %1 > ? )" ).arg( name ) );
            binds.append( *it );
            break;
        case ( Greater | Field ):
            wheres.append( QString( "( %1 > %2 )" ).arg( name ).arg( (*it).toString() ) );
            break;
        case Less:
            wheres.append( QString( "( %1 < ? )" ).arg( name ) );
            binds.append( *it );
            break;
        case ( Less | Field ):
            wheres.append( QString( "( %1 < %2 )" ).arg( name ).arg( (*it).toString() ) );
            break;
        case ( Equal | Greater ):
            wheres.append( QString( "( %1 >= ? )" ).arg( name ) );
            binds.append( *it );
            break;
        case ( Equal | Less ):
            wheres.append( QString( "( %1 <= ? )" ).arg( name ) );
            binds.append( *it );
            break;
        case Interval:
            wheres.append( QString( "( ( ? < %1 ) AND ( %2 < ? ) )" ).arg( name, name ) );
            binds.append( *it++ );
            binds.append( *it );
            break;
        case Segment:
            wheres.append( QString( "( ( ? <= %1 ) AND ( %2 <= ? ) )" ).arg( name, name ) );
            binds.append( *it++ );
            binds.append( *it );
            break;
        case InSet:
            for( ; it != list.end(); ++it )	{
                values.append( (*it).toString() );
            }
            wheres.append( QString(" ( %1 IN (%2) )" ).arg( name, values.join(",") ) );
            break;
        case ( Not | InSet ):
            for( ; it != list.end(); ++it )	values.append( (*it).toString() );
            wheres.append( QString( "( %1 NOT IN (%2) )" ).arg( name, values.join(",") ) );
            break;
        case ( Like | Case ):
            wheres.append( QString( "( %1 ilike ? )" ).arg( name ) );
            binds.append( *it );
            break;
        case ( Equal | Case ):
            wheres.append( QString( "( %1 = ? )" ).arg( name ) );
            binds.append( *it );
            break;
        case Like:
            wheres.append( QString( "( %1 like ? )" ).arg( name ) );
            binds.append( *it );
            break;
        case ( Equal | IntArray ):
            for( ; it != list.end(); ++it )	values.append( (*it).toString() );
            wheres.append( QString( "( %1 = '{%2}' )" ).arg( name, values.join(",") ) );
            break;
        case ( Equal | RowAcl ):
            for( ; it != list.end(); ++it )	values.append( (*it).toString() );
            wheres.append( QString( "( %1 = '%2' )" ).arg( name, values.join(",") ) );
            break;
            // INTERSECTION
        case ( Intersect | IntArray ):
            for( ; it != list.end(); ++it )	values.append( (*it).toString() );
            wheres.append( QString( "( %1 && '{%2}' )" ).arg( name, values.join(",") ) );
            break;
        case ( Intersect | LTree ):
            for( ; it != list.end(); ++it )	values.append( (*it).toString() );
            wheres.append( QString( "( %1 @> '%2' )" ).arg( name, values.join(".") ) );
            break;
        case ( Intersect | RowAcl ):
            for( ; it != list.end(); ++it )	values.append( (*it).toString() );
            wheres.append( QString( "( %1 && '%2' )" ).arg( name, values.join(",") ) );
            break;
            // CONTAINS
        case ( Contains | IntArray ):
            for( ; it != list.end(); ++it )	values.append( (*it).toString() );
            wheres.append( QString( "( %1 @ '{%2}' )" ).arg( name, values.join(",") ) );
            break;
        case ( Contains | LTree ):
            for( ; it != list.end(); ++it )	values.append( (*it).toString() );
            wheres.append( QString( "( %1 <@ '%2' )" ).arg( name, values.join(".") ) );
            break;
        case ( Contains | RowAcl ):
            for( ; it != list.end(); ++it )	values.append( (*it).toString() );
            wheres.append( QString( "( %1 &> '%2' )" ).arg( name, values.join(",") ) );
            break;
            // WITHINS
        case ( Within | IntArray ):
            for( ; it != list.end(); ++it )	values.append( (*it).toString() );
            wheres.append( QString( "( %1 ~ '{%2}' )" ).arg( name, values.join(",") ) );
            break;
        case ( Within | RowAcl ):
            for( ; it != list.end(); ++it )	values.append( (*it).toString() );
            wheres.append( QString( "( %1 &< '%2' )" ).arg( name, values.join(",") ) );
            break;
        }
    }

    Object *Keeper::objectByType( const ObjectType &type )
    {
        Object *obj = 0;
        switch ( type )
        {
            case OAction:              obj = new Action(this); break;
            case OMemberAddress:       obj = new MemberAddress(this); break;
            case OCoachAddress:        obj = new CoachAddress(this); break;
            case OWrestlerAddress:     obj = new WrestlerAddress(this); break;
            case ORefereeAddress:      obj = new RefereeAddress(this); break;
            case OAddressType:         obj = new AddressType(this); break;
            case OAge:                 obj = new Age(this); break;
            case OCircle:              obj = new Circle(this); break;
            case OClassify:            obj = new Classify(this); break;
            case OCoach:               obj = new Coach(this); break;
            case OCompetition:         obj = new Competition(this); break;
            case OCompetitionMember:   obj = new CompetitionMember(this); break;
            case OCompetitionReferee:  obj = new CompetitionReferee(this); break;
            case OCompetitionStyle:    obj = new CompetitionStyle(this); break;
            case OCompetitionTeam:     obj = new CompetitionTeam(this); break;
            case OCompetitionWeight:   obj = new CompetitionWeight(this); break;
            case OCompetitionWrestler: obj = new CompetitionWrestler(this); break;
            case OCRound:              obj = new CRound(this); break;
            case OCTour:               obj = new CTour(this); break;
            case OCompType:            obj = new CompType(this); break;
            case OFight:               obj = new Fight(this); break;
            case OFightLog:            obj = new FightLog(this); break;
            case OFRound:              obj = new FRound(this); break;
            case OGender:              obj = new Gender(this); break;
            case OGeo:                 obj = new Geo(this); break;
            case OMat:                 obj = new Mat(this); break;
            case OMatch:               obj = new Match(this); break;
            case OMember:              obj = new Member(this); break;
            case OPosition:            obj = new Position(this); break;
            case ORang:                obj = new Rang(this); break;
            case OReferee:             obj = new Referee(this); break;
            case ORole:                obj = new Role(this); break;
            case OStyle:               obj = new Style(this); break;
            case OTeam:                obj = new Team(this); break;
            case OWeight:              obj = new Weight(this); break;
            case OWrestler:            obj = new Wrestler(this); break;
            case OCGroup:              obj = new CGroup(this); break;
            default:                   obj = new Object(this); break;
        }
        return obj;
    }

    QString Keeper::tableByType( const ObjectType &type )
    {
        Object *obj = this->objectByType(type);
        QString table = obj->getTable();
        delete obj;
        return table;
    }

    QStringList Keeper::filedsByType( const ObjectType &type )
    {
        Object *obj = this->objectByType(type);
        QStringList list = obj->nameSchema();
        delete obj;
        return list;
    }

    void Keeper::clearObjectList( QList<Object*> list )
    {
        while (!list.isEmpty())
            delete list.takeFirst();
    }

    // --------------------------------------------------------------------------------------------------
    // DATA RETRIVING FUNCTIONS

    QList<Object*> Keeper::getObjectList( const ObjectType &type, const TQueryMap &opts )
    {
        Object *obj = 0;
        QList<Object*> list;
        QString table = tableByType( type );
        QStringList fileds = filedsByType( type );

        QSqlQuery query = parseQueryMap( table, fileds, fileds, opts );
        if ( !query.exec() ) throw SqlError( query.lastError().text(), query.executedQuery() );

        while ( query.next() )
        {
            QCoreApplication::processEvents ();
            obj = objectByType(type);
            if ( obj )
            {
                obj->restore( query );
                list.append( obj );
            }
        }

        return list;
    }

    int Keeper::countObjects( const ObjectType &type, const TQueryMap &opts )
    {
        QString table = tableByType( type );
        QStringList fileds = ( QStringList() << "count(id)" );
        QStringList wList = filedsByType( type );

        QSqlQuery query = parseQueryMap( table, fileds, wList, opts );
        if ( !query.exec() ) {
            throw SqlError( query.lastError().text(), query.executedQuery() );
        }

        query.next();
        return query.value(0).toInt();
    }

    QList<QVariant> Keeper::getFieldList( const ObjectType &type, const QString &filed, const TQueryMap &opts )
    {
        QList<QVariant> list;
        QString table = tableByType( type );
        QStringList fileds( filed );
        QStringList wList = filedsByType( type );

        QSqlQuery query = parseQueryMap( table, fileds, wList, opts );
        if ( !query.exec() ) throw SqlError( query.lastError().text(), query.executedQuery() );

        while ( query.next() ) {
            QCoreApplication::processEvents ();
            list.append( query.value(0) );
        }

        return list;
    }

    QList<QVariant> Keeper::getListOfFields( const ObjectType &type, const QStringList &fileds, const TQueryMap &opts )
    {
        QList<QVariant> list;
        QString table = tableByType( type );
        QStringList wList = filedsByType( type );

        QSqlQuery query = parseQueryMap( table, fileds, wList, opts );
        if ( !query.exec() ) throw SqlError( query.lastError().text(), query.executedQuery() );

        while ( query.next() ) {
            QStringList row;
            for( int i = 0; i < fileds.size(); i++ ) {
                QCoreApplication::processEvents ();
                row.append( query.value( i ).toString() );
            }
            list.append( row );
        }
        return list;
    }

    QHash<int,QVariant> Keeper::getFieldMap( const ObjectType &type, const QString &filed, const TQueryMap &opts )
    {
        QHash<int,QVariant> map;
        QString table = tableByType( type );
        QStringList wList = filedsByType( type );
        QStringList fileds = ( QStringList() << "id" << filed );

        QSqlQuery query = parseQueryMap( table, fileds, wList, opts );
        if ( !query.exec() ) throw SqlError( query.lastError().text(), query.executedQuery() );

        while ( query.next() ) {
            QCoreApplication::processEvents ();
            map.insert( query.value(0).toInt(), query.value(1) );
        }

        return map;
    }

    QHash<QString,Object*> Keeper::getObjectMap( const ObjectType &type, const QString &filed, const TQueryMap &opts )
    {
        QHash<QString,Object*> map;
        QString table = tableByType( type );
        QStringList fileds = filedsByType( type );

        QSqlQuery query = parseQueryMap( table, fileds, fileds, opts );
        if ( !query.exec() ) throw SqlError( query.lastError().text(), query.executedQuery() );

        Object *obj = 0;

        int i = 0;
        bool by_order = filed.compare("order") == 0;

        while ( query.next() )
        {
            QCoreApplication::processEvents ();
            obj = objectByType(type);
            if ( obj )
            {
                obj->restore( query );
                if ( by_order ) {
                    map.insert( QString::number(++i), obj );
                } else {
                    map.insert( obj->get(filed), obj );
                }
            }
        }

        return map;
    }

}
