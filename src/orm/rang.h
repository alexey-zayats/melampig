#ifndef MELAMPIG_RANG_H
#define MELAMPIG_RANG_H

#include "tree.h"

namespace Melampig
{
	class MELAMPIG_ORM_EXPORT Rang : public Tree
	{
		Q_OBJECT

	public:
		Rang( Keeper *keeper );
		Rang( int id, Keeper *keeper );
		Rang( const TQueryMap &opts, Keeper *keeper );

	protected:
		void init();

	};
}

#endif // MELAMPIG_RANG_H
