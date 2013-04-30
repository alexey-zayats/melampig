#include "refereeaddress.h"

namespace Melampig
{
	RefereeAddress::RefereeAddress(Keeper *keeper) : Object(keeper)
	{
		init();
	}

	RefereeAddress::RefereeAddress( int id, Keeper *keeper ) : Object(keeper)
	{
		init();
		restore( id );
	}

	RefereeAddress::RefereeAddress ( const TQueryMap &opts, Keeper *keeper ) : Object(keeper)
	{
		init();
		restore( opts );
	}

	void RefereeAddress::init()
	{
		table = QString("Referee_address");
		type = ORefereeAddress;
		mtype = aList;

		Object::setObjectName("Melampig::RefereeAddress");

		//		Name		Name			Title			ViewType	ro	visible
		schema.append( new Attr( "contact",      tr("Contact"),     Attr::ObjectRef,   false, true, OReferee ) );
		schema.append( new Attr( "addresstype",  tr("Type"),        Attr::ObjectRef,   false, true, OAddressType ) );
		schema.append( new Attr( "geo",          tr("Geo"),         Attr::ObjectRef,   false, true, OGeo ) );
		schema.append( new Attr( "postcode",     tr("Post"),        Attr::String,      false, true ) );
		schema.append( new Attr( "street",       tr("Street"),      Attr::String,      false, true ) );
		schema.append( new Attr( "house",        tr("House"),       Attr::String,      false, true ) );

		viewItems.append( "contact" );
		viewItems.append( "addresstype" );
		viewItems.append( "geo" );
		viewItems.append( "postcode" );
		viewItems.append( "street" );
		viewItems.append( "house" );

	}
}
