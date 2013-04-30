#ifndef MELAMPIG_ACTION_H
#define MELAMPIG_ACTION_H

#include "object.h"

namespace Melampig
{
	class MELAMPIG_ORM_EXPORT Action : public Object
	{
		Q_OBJECT

	public:
		Action( Keeper *keeper );
		Action( int id, Keeper *keeper );
		Action( const TQueryMap &opts, Keeper *keeper );

	protected:
		void init();

	};
}

#endif // MELAMPIG_ACTION_H
