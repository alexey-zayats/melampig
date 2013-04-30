#ifndef MELAMPIG_CROUND_H
#define MELAMPIG_CROUND_H

#include "object.h"

namespace Melampig
{
	class MELAMPIG_ORM_EXPORT ARound : public Object
	{
		Q_OBJECT

	public:
		ARound( Keeper *keeper );
		ARound( int id, Keeper *keeper );
		ARound( const TQueryMap &opts, Keeper *keeper );

	protected:
		void init();

	};
}

#endif // MELAMPIG_CROUND_H
