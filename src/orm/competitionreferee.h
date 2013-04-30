#ifndef COMPETITION_REFEREE_H
#define COMPETITION_REFEREE_H

#include "object.h"

namespace Melampig
{
	class MELAMPIG_ORM_EXPORT CompetitionReferee : public Object
	{
		Q_OBJECT

	public:
		CompetitionReferee( Keeper *keeper );
		CompetitionReferee( int id, Keeper *keeper );
		CompetitionReferee( const TQueryMap &opts, Keeper *keeper );

	protected:
		void init();

	};
}

#endif // COMPETITION_REFEREE_H

