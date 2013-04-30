#ifndef MELAMPIG_PROJECT_H
#define MELAMPIG_PROJECT_H

#include <QStringList>
#include <QString>
#include <QHash>
#include <QObject>

#include "exports.h"

namespace Melampig
{
	class Keeper;

	enum SslMode
	{
		SSLDisable = 0,
		SSLAllow = 1,
		SSLPrefer = 2,
		SSLRequire = 3
	};

	class MELAMPIG_ORM_EXPORT Project : public QObject
	{
		Q_OBJECT
		public:
			Project( QObject *parent = 0 );
			Project( const QString &host, const QString &name, QObject *parent = 0 );
			~Project();

			void open();
			void close();
			void store();
			void remove();
			bool isOpen();

			QString getAlias() const { return alias; }
			QString getName() const { return dbname; }
			QString getHost() const { return host; }
			QStringList getLoginList() const { return loginList; }

			int getPort() const { return port; }
			int getSSLMode() const { return sslMode; }
			int getTimeout() const { return timeout; }

			Keeper *getKeeper() const { return keeper; }

		public slots:
			void setAlias( const QString &str ) { alias = str; }
			void setName( const QString &str ) { dbname = str; }
			void setHost( const QString &str ) { host = str; }
			void setUserName( const QString &str ) { username = str; }
			void setPassword( const QString &str ) { password = str; }
			void setPort( int i ) { port = i; }
			void setSSLMode( int mode ) { sslMode = mode; }
			void setTimeout( int tout ) { timeout = tout; }
			void setLoginList( const QStringList &list );

		private:
			QString alias;
			QString dbname;
			QString host;
			int port;
			QString username;
			QString password;
			int sslMode;
			int timeout;
			Keeper *keeper;
			QStringList loginList;
	};
}

#endif // CMSPROJECT_H



