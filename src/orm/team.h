#ifndef MELAMPIG_TEAM_H
#define MELAMPIG_TEAM_H

#include "object.h"

namespace Melampig
{
	class MELAMPIG_ORM_EXPORT Team : public Object
	{
		Q_OBJECT

	public:
		Team( Keeper *keeper );
		Team( int id, Keeper *keeper );
		Team( const TQueryMap &opts, Keeper *keeper );

	protected:
		void init();

	};
}

#endif // MELAMPIG_TEAM_H
