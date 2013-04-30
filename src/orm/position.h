#ifndef MELAMPIG_POSITION_H
#define MELAMPIG_POSITION_H

#include "object.h"

namespace Melampig
{
	class MELAMPIG_ORM_EXPORT Position : public Object
	{
		Q_OBJECT

	public:
		Position( Keeper *keeper );
		Position( int id, Keeper *keeper );
		Position( const TQueryMap &opts, Keeper *keeper );

	protected:
		void init();

	};
}

#endif // MELAMPIG_POSITION_H
