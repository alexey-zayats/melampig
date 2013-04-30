#include "fight.h"

namespace Melampig
{

	Fight::Fight(Keeper *keeper) : Object(keeper)
	{
		init();
	}

	Fight::Fight( int id, Keeper *keeper) : Object(keeper)
	{
		init();
		restore( id );
	}

	Fight::Fight ( const TQueryMap &opts, Keeper *keeper) : Object(keeper)
	{
		init();
		restore( opts );
	}

	void Fight::init()
	{
		table = QString("fight");
		type = OFight;
		mtype = aTable;

		Object::setObjectName("Melampig::Fight");

		//		Name		Name			Title			ViewType	ro	visible
		schema.append( new Attr( "competition",       tr("competition"),      Attr::ObjectRef,   false, true, OCompetition ) );
		schema.append( new Attr( "style",             tr("style"),            Attr::ObjectRef,   false, true, OStyle ) );
		schema.append( new Attr( "weight",            tr("weight"),           Attr::ObjectRef,   false, true, OWeight ) );
		schema.append( new Attr( "match",             tr("match"),            Attr::ObjectRef,   false, true, OMatch ) );
		schema.append( new Attr( "cround",            tr("round"),            Attr::ObjectRef,   false, true, OCRound ) );
		schema.append( new Attr( "ctour",             tr("tour"),             Attr::ObjectRef,   false, true, OCTour) );
		schema.append( new Attr( "circle",            tr("circle"),            Attr::ObjectRef,   false, true, OCircle ) );
		schema.append( new Attr( "mat",               tr("mat"),              Attr::ObjectRef,   false, true, OMat ) );
		schema.append( new Attr( "num",               tr("num"),              Attr::Int,         false, true ) );
		schema.append( new Attr( "red",               tr("red"),              Attr::ObjectRef,   false, true, OWrestler ) );
		schema.append( new Attr( "red_pen",           tr("red penalti"),      Attr::IntArray,    false, true ) );
		schema.append( new Attr( "red_score",         tr("red score"),        Attr::IntArray,    false, true ) );
		schema.append( new Attr( "blue",              tr("blue"),             Attr::ObjectRef,   false, true, OWrestler ) );
		schema.append( new Attr( "blue_pen",          tr("blue penalti"),     Attr::IntArray,    false, true ) );
		schema.append( new Attr( "blue_score",        tr("blue score"),       Attr::IntArray,    false, true ) );
		schema.append( new Attr( "winner",            tr("winner"),           Attr::ObjectRef,   false, true, OWrestler ) );
		schema.append( new Attr( "start",             tr("start"),            Attr::DateTime,    false, true ) );
		schema.append( new Attr( "stop",              tr("stop"),             Attr::DateTime,    false, true ) );
		schema.append( new Attr( "classify",          tr("classify"),         Attr::ObjectRef,   false, true, OClassify ) );
		schema.append( new Attr( "on_fight",          tr("Fight in process"), Attr::Bool,        false, true) );

		viewItems.append( "num" );
		viewItems.append( "competition" );
		viewItems.append( "style" );
		viewItems.append( "weight" );
		viewItems.append( "match" );
		viewItems.append( "circle" );
		viewItems.append( "cround" );
		viewItems.append( "ctour" );
		viewItems.append( "mat" );
		viewItems.append( "red" );
		viewItems.append( "blue" );
		viewItems.append( "winner" );
		viewItems.append( "classify" );
	}
}
