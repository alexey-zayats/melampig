#include "competitionmember.h"

namespace Melampig
{
	CompetitionMember::CompetitionMember(Keeper *keeper) : Object(keeper)
	{
		init();
	}

	CompetitionMember::CompetitionMember( int id, Keeper *keeper) : Object(keeper)
	{
		init();
		restore( id );
	}

	CompetitionMember::CompetitionMember ( const TQueryMap &opts, Keeper *keeper) : Object(keeper)
	{
		init();
		restore( opts );
	}

	void CompetitionMember::init()
	{
		table = QString("competition_member");
		type = OCompetitionMember;
		mtype = aTable;

		Object::setObjectName("Melampig::CompetitionMember");

		//		                Name     Title       ViewType            ro	visible
		schema.append( new Attr( "competition",  tr("Competition"),  Attr::ObjectRef,    false, true, OCompetition ) );
		schema.append( new Attr( "member",       tr("Member"),       Attr::ObjectRef,    false, true, OMember ) );
		schema.append( new Attr( "role",         tr("Role"),         Attr::ObjectRef,    false, true, ORole ) );

		viewItems.append( "competition" );
		viewItems.append( "member" );
		viewItems.append( "role" );
	}
}
