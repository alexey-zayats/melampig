#ifndef MELAMPIG_AGE_H
#define MELAMPIG_AGE_H

#include "object.h"

namespace Melampig
{
	class MELAMPIG_ORM_EXPORT Age : public Object
	{
		Q_OBJECT

	public:
		Age( Keeper *keeper );
		Age( int id, Keeper *keeper );
		Age( const TQueryMap &opts, Keeper *keeper );

	protected:
		void init();

	};
}

#endif // MELAMPIG_AGE_H
