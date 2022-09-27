#pragma once


namespace trexp {
	enum class Direction { LEFT, RIGHT };
	enum class Algorithms { 
		DFS_PRE_ORDER, 
		DFS_ORDERED,
		DFS_POST_ORDER,
		HEIGHT,
		BFS,
		BST_MAX,
		BST_MIN,
		BST_FIND,
		BST_INSERT,
		BST_PREDECESSOR
	};
}