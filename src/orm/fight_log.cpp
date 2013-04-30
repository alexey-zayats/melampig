#include "fight_log.h"

namespace Melampig
{
	FightLog::FightLog(Keeper *keeper) : Object(keeper)
	{
		init();
	}

	FightLog::FightLog( int id, Keeper *keeper) : Object(keeper)
	{
		init();
		restore( id );
	}

	FightLog::FightLog ( const TQueryMap &opts, Keeper *keeper) : Object(keeper)
	{
		init();
		restore( opts );
	}

	void FightLog::init()
	{
		table = QString("fight_log");
		type = OFightLog;
		mtype = aTable;

		Object::setObjectName("Melampig::FightLog");

		//		Name		Name			Title			ViewType	ro	visible
		schema.append( new Attr( "fight",     tr("fight"),    Attr::ObjectRef,   false, true, OFight ) );
		schema.append( new Attr( "wrestler",  tr("wrestler"), Attr::ObjectRef,   false, true, OWrestler ) );
		schema.append( new Attr( "action",    tr("action"),   Attr::ObjectRef,   false, true, OAction ) );
		schema.append( new Attr( "fround",    tr("round"),    Attr::ObjectRef,   false, true, OFRound ) );
		schema.append( new Attr( "score",     tr("score"),    Attr::Int,         false, true ) );
		schema.append( new Attr( "etime",     tr("atime"),    Attr::DateTime,    false, true ) );

		viewItems.append( "fight" );
		viewItems.append( "wrestler" );
		viewItems.append( "action" );
		viewItems.append( "fight_round" );
		viewItems.append( "score" );
		viewItems.append( "etime" );
	}
}
