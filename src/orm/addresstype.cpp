#include "addresstype.h"

namespace Melampig
{
	AddressType::AddressType(Keeper *keeper) : Object(keeper)
	{
		init();
	}

	AddressType::AddressType( int id, Keeper *keeper ) : Object(keeper)
	{
		init();
		restore( id );
	}

	AddressType::AddressType ( const TQueryMap &opts, Keeper *keeper ) : Object(keeper)
	{
		init();
		restore( opts );
	}

	void AddressType::init()
	{
		table = QString("addresstype");
		type = OAddressType;
		mtype = aList;

		Object::setObjectName("Melampig::AddressType");

		//		Name		Name			Title			ViewType	ro	visible
		schema.append( new Attr( "title", tr("Title"),       Attr::String,      false, true ) );

        viewItems.append( "title" );
	}
}
