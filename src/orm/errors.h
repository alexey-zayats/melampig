// errors.h             

#ifndef MELAMPIG_ERRORS_H
#define MELAMPIG_ERRORS_H

#include <QString>

#include "exports.h"

namespace Melampig
{
	// BASE ERROR
	class MELAMPIG_ORM_EXPORT Error
	{
	public:
		Error( const QString &msg )
		{ 
			setText( msg );
		}

		Error( const Error &er )
		{
			setText( er.getText() );
		}

		Error &operator=( const Error &er )
		{ 
			setText( er.getText() );
			return *this;
		}

		QString getText() const { return text; }
		void setText( const QString &str ) { text = str; }

	protected:
		QString text;
	};

	// SQL ERROR
	class MELAMPIG_ORM_EXPORT SqlError : public Error
	{
	public:
		SqlError( const QString &msg, const QString &sql ) : Error( msg )
		{
			setText( msg );
			setQuery( sql );
		}

		SqlError( const SqlError &er ) : Error( er.getText() )
		{ 
			setText( er.getText() );
			setQuery( er.getQuery() );
		}

		SqlError &operator=( const SqlError &er )
		{ 
			setText( er.getText() );
			setQuery( er.getQuery() );
			return *this;
		}

		virtual ~SqlError() {}
		QString getQuery() const { return query; }
		void setQuery( const QString &str ) { query = str; }

	private:
		QString query;
	};
}

#endif // MELAMPIG_ERRORS_H


