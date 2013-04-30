#ifndef MELAMPIG_CGROUP_H
#define MELAMPIG_CGROUP_H

#include "object.h"

namespace Melampig
{
	class MELAMPIG_ORM_EXPORT CGroup : public Object
	{
		Q_OBJECT

	public:
		CGroup( Keeper *keeper );
		CGroup( int id, Keeper *keeper );
		CGroup( const TQueryMap &opts, Keeper *keeper );

	protected:
		void init();

	};
}

#endif // MELAMPIG_MAT_H
