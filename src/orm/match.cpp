#include "match.h"

namespace Melampig
{
	Match::Match(Keeper *keeper) : Object(keeper)
	{
		init();
	}

	Match::Match( int id, Keeper *keeper) : Object(keeper)
	{
		init();
		restore( id );
	}

	Match::Match ( const TQueryMap &opts, Keeper *keeper) : Object(keeper)
	{
		init();
		restore( opts );
	}

	void Match::init()
	{
		table = QString("match");
		type = OMatch;
		mtype = aTable;

		Object::setObjectName("Melampig::Match");

		//		Name		Name			Title			ViewType	ro	visible
		schema.append( new Attr( "competition", tr("competition"),      Attr::ObjectRef,   false, true, OCompetition ) );
		schema.append( new Attr( "num", tr("Num"), Attr::Int,   false, true ) );
		schema.append( new Attr( "mat", tr("Mat"), Attr::ObjectRef, false, true, OMat ) );
		schema.append( new Attr( "cgroup", tr("Group"), Attr::ObjectRef, false, true, OCGroup ) );
		schema.append( new Attr( "circle", tr("Circle"), Attr::ObjectRef, false, true, OCircle) );
		schema.append( new Attr( "style", tr("Style"), Attr::ObjectRef, false, true, OStyle ) );
		schema.append( new Attr( "team_a", tr("Team (A)"), Attr::ObjectRef, false, true, OTeam) );
		schema.append( new Attr( "team_b", tr("Team (B)"), Attr::ObjectRef, false, true, OTeam ) );
		schema.append( new Attr( "cround", tr("Round"), Attr::ObjectRef, false, true, OCRound) );
		schema.append( new Attr( "ctour", tr("Tour"), Attr::ObjectRef, false, true, OCTour) );
		schema.append( new Attr( "winner", tr("Winner"), Attr::ObjectRef, false, true, OTeam) );
		schema.append( new Attr( "classify", tr("Classify"), Attr::ObjectRef, false, true, OClassify) );
		schema.append( new Attr( "title", tr("Title"), Attr::String, false, true ) );

		schema.append( new Attr( "team_a_win", tr("Team (A) win"), Attr::Int, false, true) );
		schema.append( new Attr( "team_b_win", tr("Team (B) win"), Attr::Int, false, true) );

		viewItems.append( "competition" );
		viewItems.append( "num" );
		viewItems.append( "mat" );
		viewItems.append( "cgroup" );
		viewItems.append( "circle" );
		viewItems.append( "style" );
		viewItems.append( "cround" );
		viewItems.append( "ctour" );
		viewItems.append( "team_a" );
		viewItems.append( "team_a_win" );
		viewItems.append( "team_b" );
		viewItems.append( "team_b_win" );
		viewItems.append( "winner" );
		viewItems.append( "classify" );
	}
}
