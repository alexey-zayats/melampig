#include "referee.h"

namespace Melampig
{

	Referee::Referee(Keeper *keeper) : Object(keeper)
	{
		init();
	}

	Referee::Referee( int id, Keeper *keeper) : Object(keeper)
	{
		init();
		restore( id );
	}

	Referee::Referee ( const TQueryMap &opts, Keeper *keeper) : Object(keeper)
	{
		init();
		restore( opts );
	}

	void Referee::init()
	{
		table = QString("referee");
		type = OReferee;
		mtype = aTable;

		Object::setObjectName("Melampig::Referee");

		//		Name		Name			Title			ViewType	ro	visible
		schema.append( new Attr( "title",        tr("Title"),           Attr::String,    true, true ) );

		schema.append( new Attr( "lastname",       tr("Lastname"),      Attr::String,   false, true ) );
		schema.append( new Attr( "firstname",      tr("Firstname"),     Attr::String,   false, true ) );
		schema.append( new Attr( "patronymic",     tr("Patronymic"),    Attr::String,    false, true ) );

		schema.append( new Attr( "rang",           tr("Rang"),          Attr::ObjectRef, false, true, ORang ) );
		schema.append( new Attr( "license",        tr("License"),       Attr::String,    false, true ) );
		schema.append( new Attr( "gender",         tr("Gender"),        Attr::ObjectRef, false, true, OGender ) );
		schema.append( new Attr( "geo",            tr("Geo"),           Attr::ObjectRef, false, true, OGeo ) );

		schema.append( new Attr( "birthdate",      tr("birthdate"),     Attr::Date,     false, true ) );

		schema.append( new Attr( "phone",          tr("Phone"),         Attr::String,    false, true ) );
		schema.append( new Attr( "email",          tr("Email"),         Attr::String,    false, true ) );
		schema.append( new Attr( "picture",        tr("Picture"),       Attr::Image,    false, true ) );

		viewItems.append( "lastname" );
		viewItems.append( "firstname" );
		viewItems.append( "patronymic" );
		viewItems.append( "gender" );
		viewItems.append( "rang" );
		viewItems.append( "license" );
		viewItems.append( "geo" );
	}
}
