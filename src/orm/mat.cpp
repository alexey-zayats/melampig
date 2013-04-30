#include "mat.h"

namespace Melampig
{
	Mat::Mat(Keeper *keeper) : Object(keeper)
	{
		init();
	}

	Mat::Mat( int id, Keeper *keeper) : Object(keeper)
	{
		init();
		restore( id );
	}

	Mat::Mat ( const TQueryMap &opts, Keeper *keeper) : Object(keeper)
	{
		init();
		restore( opts );
	}

	void Mat::init()
	{
		table = QString("mat");
		type = OMat;
		mtype = aList;

		Object::setObjectName("Melampig::Mat");

		//		Name		Name			Title			ViewType	ro	visible
		schema.append( new Attr( "title",          tr("Title"),       Attr::String,   false, true ) );

		viewItems.append( "title" );
	}
}
