#ifndef COMPETITION_WEIGHT_H
#define COMPETITION_WEIGHT_H

#include "object.h"

namespace Melampig
{
	class MELAMPIG_ORM_EXPORT CompetitionWeight: public Object
	{
		Q_OBJECT

	public:
		CompetitionWeight( Keeper *keeper );
		CompetitionWeight( int id, Keeper *keeper );
		CompetitionWeight( const TQueryMap &opts, Keeper *keeper );

	protected:
		void init();

	};
}

#endif // COMPETITION_WEIGHT_H

