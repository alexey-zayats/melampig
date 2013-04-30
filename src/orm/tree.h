// tree.h

#ifndef MELAMPIG_TREE_H
#define MELAMPIG_TREE_H

#include "object.h"

namespace Melampig
{
	class MELAMPIG_ORM_EXPORT Tree : public Object
	{
		Q_OBJECT
	public:
		Tree( Keeper *keeper );
		Tree( int id, Keeper *keeper );
		Tree( const TQueryMap &opts, Keeper *keeper );

		~Tree();

		Tree *parent();
		Tree *child(int id);

		int childCount();
		int childIndex(Tree *child);

		void setParent(Tree *prn);
		void addChild( Tree *tree, int position = -1 );
		void removeChild(int position);
		void setChildCount(int count) { childsCount = count; }

		bool insertChild(int position);
		bool deleteChild(int position);

		Tree* search(int id);
		void populateChilds();

	protected:
		void init();


		bool childsPopulated;
		int childsCount;
		QList<Object*> childs;
		Tree *_parent;
	};
}

#endif // MELAMPIG_TREE_H
