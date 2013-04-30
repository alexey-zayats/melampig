// link.h

#ifndef MELAMPIG_LINK_H
#define MELAMPIG_LINK_H

#include "object.h"

namespace Melampig
{
	class MELAMPIG_ORM_EXPORT Link : public Object
	{
		Q_OBJECT

	public:
		Link( Keeper *keeper );
		Link( int id, Keeper *keeper );
		Link( const TQueryMap &opts, Keeper *keepers );

	protected:
		void init();
	};
}

#endif // MELAMPIG_LINK_H
