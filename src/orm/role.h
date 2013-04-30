#ifndef MELAMPIG_ROLE_H
#define MELAMPIG_ROLE_H

#include "object.h"

namespace Melampig
{
	class MELAMPIG_ORM_EXPORT Role : public Object
	{
		Q_OBJECT

	public:
		Role( Keeper *keeper );
		Role( int id, Keeper *keeper );
		Role( const TQueryMap &opts, Keeper *keeper );

	protected:
		void init();

	};
}

#endif // MELAMPIG_TOUR_H
