#ifndef MELAMPIG_REFEREE_H
#define MELAMPIG_REFEREE_H

#include "object.h"

namespace Melampig
{
	class MELAMPIG_ORM_EXPORT Referee : public Object
	{
		Q_OBJECT

	public:
		Referee( Keeper *keeper );
		Referee( int id, Keeper *keeper );
		Referee( const TQueryMap &opts, Keeper *keeper );

	protected:
		void init();

	};
}

#endif // MELAMPIG_REFEREE_H
