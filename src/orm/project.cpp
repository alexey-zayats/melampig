#include <QSettings>
#include <QtGlobal>
#include <QMessageBox>
#include <QDebug>

#include "errors.h"
#include "project.h"
#include "keeper.h"

namespace Melampig
{
	Project::Project ( QObject *parent ) : QObject( parent )
	{
		keeper = 0;
		alias = "";
		host = "localhost";
		dbname = "";
		username = "";
		password = "";
		sslMode = SSLPrefer;
		port = 5432;
		timeout = 15;
	}

	Project::Project( const QString &h, const QString &a, QObject *parent )
	: QObject( parent )
	{
		keeper = 0;
		QSettings s;
		s.beginGroup ( "projects" );

		QStringList hosts = s.childGroups();

		if ( hosts.contains( h ) )
		{
			s.beginGroup ( h );
			QStringList aliases = s.childGroups();
			if ( aliases.contains( a ) )
			{
				s.beginGroup( a );
				this->host = s.value("host").toString();
				alias = s.value("alias").toString();
				dbname = s.value( "dbname" ).toString();
				port = s.value("port").toInt();
				sslMode = s.value("ssl").toInt();
				timeout = s.value("timeout").toInt();
				int size = s.beginReadArray("logins");
				for (int i = 0; i < size; ++i) {
					s.setArrayIndex(i);
					loginList.append( s.value("item").toString() );
				}
				s.endArray();
				s.endGroup(); // end dbname
			}

			s.endGroup(); // end host
		}

		s.endGroup();
	}

	Project::~Project()
	{
		close();
	}

	void Project::close()
	{
		if ( keeper )
		{
			keeper->close();
			delete keeper;
			keeper = 0;
		}
	}

	bool Project::isOpen()
	{
		if ( !keeper ) return false;
		return keeper->database().isOpen();
	}

	void Project::setLoginList( const QStringList &list )
	{
		loginList = list;
	}

	void Project::open()
	{
		QHash<QString,QString> opt;
		opt.insert( "alias", alias );
		opt.insert( "dbname", dbname );
		opt.insert( "user", username );
		opt.insert( "host", host );
		opt.insert( "port", QString("%1").arg( port) );
		opt.insert( "password", password );

		QStringList list;
		list.append( "connect_timeout=" + QString("%1").arg( timeout ) );

		switch( sslMode )
		{
			case SSLDisable:
				list.append( "sslmode=disable" );
				break;
			case SSLAllow:
				list.append( "sslmode=allow" );
				break;
			case SSLPrefer:
				list.append( "sslmode=prefer" );
				break;
			case SSLRequire:
				list.append( "sslmode=require" );
				break;
		}

		opt.insert( "options", list.join(";") );

		try
		{
			keeper = new Keeper();
			keeper->open( opt );
		}
		catch ( SqlError &err ) {
			throw SqlError(err);
		}
	}

	void Project::store()
	{
		QSettings s;

		s.beginGroup( "projects" );
		s.beginGroup( host );
		s.beginGroup( alias );

		s.setValue( "host", host );
		s.setValue( "port", port );
		s.setValue( "alias", alias );
		s.setValue( "dbname", dbname );
		s.setValue( "ssl", sslMode );
		s.setValue( "timeout", timeout );

		s.beginWriteArray("logins");
		for( int i = 0; i < loginList.size(); i++ ) {
			s.setArrayIndex(i);
			s.setValue("item", loginList.at(i) );
		}
		s.endArray();

		s.endGroup();	// alias
		s.endGroup();	// host
		s.endGroup();	// projects
	}

	void Project::remove()
	{
		close();
		QSettings s;

		s.beginGroup( "projects" );

		s.beginGroup( host );
		s.remove( alias );

		if ( s.childGroups().size() == 0 )
		{
			s.endGroup();
			s.remove( host );
		}
	}
}


