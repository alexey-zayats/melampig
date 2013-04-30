#ifndef COMPETITION_MEMBER_H
#define COMPETITION_MEMBER_H

#include "object.h"

namespace Melampig
{
	class MELAMPIG_ORM_EXPORT CompetitionMember : public Object
	{
		Q_OBJECT

	public:
		CompetitionMember( Keeper *keeper );
		CompetitionMember( int id, Keeper *keeper );
		CompetitionMember( const TQueryMap &opts, Keeper *keeper );

	protected:
		void init();

	};
}

#endif // COMPETITION_MEMBER_H

