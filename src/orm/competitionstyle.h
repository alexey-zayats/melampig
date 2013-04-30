#ifndef COMPETITION_STYLE_H
#define COMPETITION_STYLE_H

#include "object.h"

namespace Melampig
{
	class MELAMPIG_ORM_EXPORT CompetitionStyle : public Object
	{
		Q_OBJECT

	public:
		CompetitionStyle( Keeper *keeper );
		CompetitionStyle( int id, Keeper *keeper );
		CompetitionStyle( const TQueryMap &opts, Keeper *keeper );

	protected:
		void init();

	};
}

#endif // COMPETITION_STYLE_H

