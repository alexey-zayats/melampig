#ifndef MELAMPIG_WEIGHT_H
#define MELAMPIG_WEIGHT_H

#include "object.h"

namespace Melampig
{
	class MELAMPIG_ORM_EXPORT Weight : public Object
	{
		Q_OBJECT

	public:
		Weight( Keeper *keeper );
		Weight( int id, Keeper *keeper );
		Weight( const TQueryMap &opts, Keeper *keeper );

	protected:
		void init();

	};
}

#endif // MELAMPIG_WEIGHT_H
