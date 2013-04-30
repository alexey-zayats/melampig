#include "competition.h"

namespace Melampig
{

	Competition::Competition(Keeper *keeper) : Object(keeper)
	{
		init();
	}

	Competition::Competition( int id, Keeper *keeper) : Object(keeper)
	{
		init();
		restore( id );
	}

	Competition::Competition ( const TQueryMap &opts, Keeper *keeper) : Object(keeper)
	{
		init();
		restore( opts );
	}

	void Competition::init()
	{
		table = QString("competition");
		type = OCompetition;
		mtype = aTable;

		Object::setObjectName("Melampig::Competition");

        //		Name		Name		    Title              ViewType           ro     visible
        schema.append( new Attr( "title",     tr("Title"),       Attr::String,      false, true ) );
        schema.append( new Attr( "start",     tr("start"),       Attr::Date,        false, true ) );
        schema.append( new Attr( "stop",      tr("stop"),        Attr::Date,        false, true ) );
        schema.append( new Attr( "age"  ,     tr("Age"),         Attr::ObjectRef,   false, true, OAge ) );
        schema.append( new Attr( "comptype",  tr("Type"),        Attr::ObjectRef,   false, true, OCompType ) );
        schema.append( new Attr( "byteam",    tr("By team"),     Attr::Bool,        false, true ) );
        schema.append( new Attr( "geo",       tr("Geo"),         Attr::ObjectRef,   false, true, OGeo ) );

		viewItems.append( "title" );
		viewItems.append( "start" );
		viewItems.append( "stop" );
		viewItems.append( "age" );
		viewItems.append( "comptype" );
		viewItems.append( "geo" );
        viewItems.append( "byteam" );
	}
}
