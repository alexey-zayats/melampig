#ifndef MELAMPIG_MEMBER_H
#define MELAMPIG_MEMBER_H

#include "object.h"

namespace Melampig
{
	class MELAMPIG_ORM_EXPORT Member : public Object
	{
		Q_OBJECT

	public:
		Member( Keeper *keeper );
		Member( int id, Keeper *keeper );
		Member( const TQueryMap &opts, Keeper *keeper );

	protected:
		void init();

	};
}

#endif // MELAMPIG_WRESTLER_H
