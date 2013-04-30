#include "member.h"

namespace Melampig
{
	Member::Member(Keeper *keeper) : Object(keeper)
	{
		init();
	}

	Member::Member( int id, Keeper *keeper) : Object(keeper)
	{
		init();
		restore( id );
	}

	Member::Member ( const TQueryMap &opts, Keeper *keeper) : Object(keeper)
	{
		init();
		restore( opts );
	}

	void Member::init()
	{
		table = QString("member");
		type = OMember;
		mtype = aTable;

		Object::setObjectName("Melampig::Member");

		//		Name		Name			Title			ViewType	ro	visible
		schema.append( new Attr( "title",          tr("Title"),           Attr::String,    true, true ) );
		schema.append( new Attr( "lastname",       tr("Lastname"),      Attr::String,   false, true ) );
		schema.append( new Attr( "firstname",      tr("Firstname"),     Attr::String,   false, true ) );
		schema.append( new Attr( "patronymic",     tr("Patronymic"),    Attr::String,    false, true ) );

		schema.append( new Attr( "gender",         tr("Gender"),        Attr::ObjectRef, false, true, OGender ) );
		schema.append( new Attr( "birthdate",      tr("birthdate"),     Attr::Date,     false, true ) );

		schema.append( new Attr( "phone",          tr("Phone"),         Attr::String,    false, true ) );
		schema.append( new Attr( "email",          tr("Email"),         Attr::String,    false, true ) );

		schema.append( new Attr( "picture",        tr("Picture"),       Attr::Image,    false, true ) );

		viewItems.append( "lastname" );
		viewItems.append( "firstname" );
		viewItems.append( "patronymic" );
		viewItems.append( "birthdate" );
		viewItems.append( "gender" );
	}
}
