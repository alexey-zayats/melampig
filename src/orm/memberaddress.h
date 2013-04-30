#ifndef MELAMPIG_MEMBERADDRESS_H
#define MELAMPIG_MEMBERADDRESS_H

#include "object.h"

namespace Melampig
{
	class MELAMPIG_ORM_EXPORT MemberAddress : public Object
	{
		Q_OBJECT

	public:
		MemberAddress( Keeper *keeper );
		MemberAddress( int id, Keeper *keeper );
		MemberAddress( const TQueryMap &opts, Keeper *keeper );

	protected:
		void init();

	};
}

#endif // MELAMPIG_MEMBERADDRESS_H
