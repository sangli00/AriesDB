#ifndef __ARIES_BTREE_H__
#define __ARIES_BTREE_H__

typedef struct BTreeNode *BTree;

struct BTreeNode{
	uint64 key;
	BTree next;	/* next node */
	uint16 type;	/* node type */
}BTreeNode;

#endif
