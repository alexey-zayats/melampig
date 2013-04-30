#ifndef MELAMPIG_WRESTLERADDRESS_H
#define MELAMPIG_WRESTLERADDRESS_H

#include "object.h"

namespace Melampig
{
	class MELAMPIG_ORM_EXPORT WrestlerAddress : public Object
	{
		Q_OBJECT

	public:
		WrestlerAddress( Keeper *keeper );
		WrestlerAddress( int id, Keeper *keeper );
		WrestlerAddress( const TQueryMap &opts, Keeper *keeper );

	protected:
		void init();

	};
}

#endif // MELAMPIG_WRESTLERADDRESS_H
