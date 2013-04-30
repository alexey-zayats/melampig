#include "style.h"

namespace Melampig
{

	Style::Style(Keeper *keeper) : Object(keeper)
	{
		init();
	}

	Style::Style( int id, Keeper *keeper) : Object(keeper)
	{
		init();
		restore( id );
	}

	Style::Style ( const TQueryMap &opts, Keeper *keeper) : Object(keeper)
	{
		init();
		restore( opts );
	}

	void Style::init()
	{
		table = QString("style");
		type = OStyle;
		mtype = aTable;

		Object::setObjectName("Melampig::Style");

		//		Name		Name			Title			           ViewType       ro     visible
		schema.append( new Attr( "title",  tr("Title"),                 Attr::String, false, true ) );
		schema.append( new Attr( "rnum",   tr("Round num"),             Attr::Int,    false, true ) );
		schema.append( new Attr( "rtime",  tr("Round time"),            Attr::Int,    false, true ) );
		schema.append( new Attr( "otime",  tr("Overtime"),              Attr::Int,    false, true ) );
		schema.append( new Attr( "rstime", tr("Resttime"),              Attr::Int,    false, true ) );
		schema.append( new Attr( "rtdir",  tr("Round time direction"),  Attr::Bool,   false, true ) );
		schema.append( new Attr( "otdir",  tr("Overtime direction"),    Attr::Bool,   false, true ) );
		schema.append( new Attr( "rstdir", tr("Rest time direction"),   Attr::Bool,   false, true ) );

		viewItems.append( "title" );
		viewItems.append( "rnum" );
		viewItems.append( "rtime" );
		viewItems.append( "otime" );
		viewItems.append( "rstime" );
		viewItems.append( "rtdir" );
		viewItems.append( "otdir" );
		viewItems.append( "rstdir" );
	}
}
