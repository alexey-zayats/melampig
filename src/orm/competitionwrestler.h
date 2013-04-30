#ifndef COMPETITION_WRESTLER_H
#define COMPETITION_WRESTLER_H

#include "object.h"

namespace Melampig
{
	class MELAMPIG_ORM_EXPORT CompetitionWrestler : public Object
	{
		Q_OBJECT

	public:
		CompetitionWrestler( Keeper *keeper );
		CompetitionWrestler( int id, Keeper *keeper );
		CompetitionWrestler( const TQueryMap &opts, Keeper *keeper );

	protected:
		void init();

	};
}

#endif // COMPETITION_WRESTLER_H

