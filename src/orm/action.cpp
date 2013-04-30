#include "action.h"

namespace Melampig
{
	Action::Action(Keeper *keeper) : Object(keeper)
	{
		init();
	}

	Action::Action( int id, Keeper *keeper ) : Object(keeper)
	{
		init();
		restore( id );
	}

	Action::Action ( const TQueryMap &opts, Keeper *keeper ) : Object(keeper)
	{
		init();
		restore( opts );
	}

	void Action::init()
	{
		table = QString("action");
		type = OAction;
		mtype = aList;

		Object::setObjectName("Melampig::Action");

		//		Name		Name			Title			ViewType	ro	visible
		schema.append( new Attr( "title",          tr("Title"),       Attr::String,   false, true ) );

		viewItems.append( "title" );
	}
}
