// util.h

#ifndef MELAMPIG_UTIL_H
#define MELAMPIG_UTIL_H

class QString;
class QStringList;

#include "exports.h"

namespace Melampig
{
	class MELAMPIG_ORM_EXPORT Util
	{
		public:
			//Util();

			static QStringList toList( const QString &str );
			static QString toArray( const QStringList &list );
			static int factorial(int);
	};
}

#endif // CMSUTIL_H
