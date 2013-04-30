#include "util.h"

#include <QString>
#include <QStringList>

namespace Melampig
{
	QStringList Util::toList( const QString &str )
	{
		QString a = str;
		a.remove( "{" );
		a.remove( "}" );
		return a.isEmpty() ? QStringList() : a.split( "," );
	}

	QString Util::toArray( const QStringList &list )
	{
		return "{" + list.join( "," ) + "}";
	}

	int Util::factorial(int num)
	{
		int result = 1;
		for( int i = 1; i <= num; i++ ) {
			result *= i;
		}
		return result;
	}
}
