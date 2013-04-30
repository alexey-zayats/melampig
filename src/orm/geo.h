#ifndef MELAMPIG_GEO_H
#define MELAMPIG_GEO_H

#include "tree.h"

namespace Melampig
{
	class MELAMPIG_ORM_EXPORT Geo : public Tree
	{
		Q_OBJECT

	public:
		Geo( Keeper *keeper );
		Geo( int id, Keeper *keeper );
		Geo( const TQueryMap &opts, Keeper *keeper );

	protected:
		void init();

	};
}

#endif // MELAMPIG_GEO_H
