#ifndef MELAMPIG_CLASSIFY_H
#define MELAMPIG_CLASSIFY_H

#include "object.h"

namespace Melampig
{
	class MELAMPIG_ORM_EXPORT Classify : public Object
	{
		Q_OBJECT

	public:
		Classify( Keeper *keeper );
		Classify( int id, Keeper *keeper );
		Classify( const TQueryMap &opts, Keeper *keeper );

	protected:
		void init();

	};
}

#endif // MELAMPIG_CLASSIFY_H
