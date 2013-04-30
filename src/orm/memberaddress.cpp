#include "memberaddress.h"

namespace Melampig
{
	MemberAddress::MemberAddress(Keeper *keeper) : Object(keeper)
	{
		init();
	}

	MemberAddress::MemberAddress( int id, Keeper *keeper ) : Object(keeper)
	{
		init();
		restore( id );
	}

	MemberAddress::MemberAddress ( const TQueryMap &opts, Keeper *keeper ) : Object(keeper)
	{
		init();
		restore( opts );
	}

	void MemberAddress::init()
	{
		table = QString("member_address");
		type = OMemberAddress;
		mtype = aList;

		Object::setObjectName("Melampig::MemberAddress");

		//		Name		Name			Title			ViewType	ro	visible
		schema.append( new Attr( "contact",      tr("Contact"),     Attr::ObjectRef,   false, true, OMember ) );
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
