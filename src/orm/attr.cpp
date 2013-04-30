// attr.cpp

#include "attr.h"

namespace Melampig
{
	Attr::Attr()
	{}

    Attr::Attr( const QString &str_name,
                const QString &str_title,
                ViewType view_type,
                bool is_ro,
                bool is_visible,
                ObjectType ref,
                bool isArray )
        : name ( str_name ),
          title( str_title ),
          type( view_type ),
          ro( is_ro ),
          visible( is_visible ),
          anArray(isArray),
          ref(ref)
    {
    }

	Attr& Attr::operator=( const Attr &a )
	{
		setName( a.getName() );
		setTitle( a.getTitle() );
		setType( a.getType() );
		setReadOnly( a.isReadOnly() );
		setVisible( a.isVisible() );
		setRef( a.getRef() );
		setArray( a.isArray() );
		return *this;
	}

    Attr::~Attr()
    {
    }

}
