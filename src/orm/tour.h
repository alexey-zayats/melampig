#ifndef MELAMPIG_TOUR_H
#define MELAMPIG_TOUR_H

#include "object.h"

namespace Melampig
{
	class MELAMPIG_ORM_EXPORT Tour : public Object
	{
		Q_OBJECT

	public:
		Tour( Keeper *keeper );
		Tour( int id, Keeper *keeper );
		Tour( const TQueryMap &opts, Keeper *keeper );

	protected:
		void init();

	};
}

#endif // MELAMPIG_TOUR_H
