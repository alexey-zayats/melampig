#ifndef COMPETITION_TEAM_H
#define COMPETITION_TEAM_H

#include "object.h"

namespace Melampig
{
	class MELAMPIG_ORM_EXPORT CompetitionTeam : public Object
	{
		Q_OBJECT

	public:
		CompetitionTeam( Keeper *keeper );
		CompetitionTeam( int id, Keeper *keeper );
		CompetitionTeam( const TQueryMap &opts, Keeper *keeper );

	protected:
		void init();

	};
}

#endif // COMPETITION_TEAM_H

