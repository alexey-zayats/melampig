#include "classify.h"

namespace Melampig
{
	Classify::Classify(Keeper *keeper) : Object(keeper)
	{
		init();
	}

	Classify::Classify( int id, Keeper *keeper) : Object(keeper)
	{
		init();
		restore( id );
	}

	Classify::Classify ( const TQueryMap &opts, Keeper *keeper) : Object(keeper)
	{
		init();
		restore( opts );
	}

	void Classify::init()
	{
		table = QString("classify");
		type = OClassify;
		mtype = aTable;

		Object::setObjectName("Melampig::Classify");

		//		Name		Name			Title			ViewType	ro	visible
		schema.append( new Attr( "title",          tr("Title"),      Attr::String,   false, true ) );
		schema.append( new Attr( "win",            tr("Win"),        Attr::Int,   false, true ) );
		schema.append( new Attr( "fail",           tr("Fail"),       Attr::Int,   false, true ) );

		viewItems.append( "win" );
		viewItems.append( "fail" );
		viewItems.append( "title" );
	}
}
