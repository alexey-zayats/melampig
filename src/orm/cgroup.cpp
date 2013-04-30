#include "cgroup.h"

namespace Melampig
{
	CGroup::CGroup(Keeper *keeper) : Object(keeper)
	{
		init();
	}

	CGroup::CGroup( int id, Keeper *keeper) : Object(keeper)
	{
		init();
		restore( id );
	}

	CGroup::CGroup ( const TQueryMap &opts, Keeper *keeper) : Object(keeper)
	{
		init();
		restore( opts );
	}

	void CGroup::init()
	{
		table = QString("cgroup");
		type = OCGroup;
		mtype = aList;

		Object::setObjectName("Melampig::CGroup");

		//		Name		Name			Title			ViewType	ro	visible
		schema.append( new Attr( "title",          tr("Title"),       Attr::String,   false, true ) );

		viewItems.append( "title" );
	}
}
