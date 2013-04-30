#ifndef MELAMPIG_CIRCLE_H
#define MELAMPIG_CIRCLE_H

#include "object.h"

namespace Melampig
{
	class MELAMPIG_ORM_EXPORT Circle : public Object
	{
		Q_OBJECT

	public:
		Circle( Keeper *keeper );
		Circle( int id, Keeper *keeper );
		Circle( const TQueryMap &opts, Keeper *keeper );

	protected:
		void init();

	};
}

#endif // MELAMPIG_CIRCLE_H
