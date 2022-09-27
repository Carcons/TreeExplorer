#include "AlgorithmSimulator.h"


#include <QDebug>
#include <queue>

using namespace trexp;

// Class AlgorithmSimulator

AlgorithmSimulator::AlgorithmSimulator() : QObject(0) {}

GraphicsNode* AlgorithmSimulator::BSTMaxAlgorithm(GraphicsNode* node) {

	_currentAlgorithm = Algorithms::BST_MAX;

	GraphicsNode* n = node;
	while (n->rightNode() != 0) {
		addNode(n);
		n = n->rightNode();
	}
	addNode(n);
	return n;

}

GraphicsNode* AlgorithmSimulator::BSTFindAlgorithm(GraphicsNode* node, int value) {

	_currentAlgorithm = Algorithms::BST_FIND;

	while (node != 0) {
		addNode(node);
		if (node->intValue() == value) {
			_bstFoundValue = value;
			return node;
		}
		else if (value < node->intValue())
			node = node->leftNode();
		else
			node = node->rightNode();
	}
	return 0;
}

GraphicsNode* AlgorithmSimulator::BSTMinAlgorithm(GraphicsNode* node) {

	_currentAlgorithm = Algorithms::BST_MIN;

	GraphicsNode* n = node;
	while (n->leftNode() != 0) {
		addNode(n);
		n = n->leftNode();
	}

	addNode(n);
	return n;

}

void AlgorithmSimulator::BFSAlgorithm(GraphicsNode* node) {

	_currentAlgorithm = Algorithms::BFS;

	std::queue<GraphicsNode*> C;
	C.push(node);

	while (!C.empty()) {
		GraphicsNode* currentNode = C.front();
		C.pop();
		if (currentNode) {
			addNode(currentNode);
			C.push(currentNode->leftNode());
			C.push(currentNode->rightNode());
		}
	}


}

void AlgorithmSimulator::checkBSTAlgorithm(GraphicsNode* node, QList<int>* values) {

	if (!node)
		return;

	checkBSTAlgorithm(node->leftNode(), values);
	values->append(node->intValue());
	checkBSTAlgorithm(node->rightNode(), values);

}

bool AlgorithmSimulator::checkBST(GraphicsNode* node) {

	QList<int> values;
	
	checkBSTAlgorithm(node, &values);
	for (int i = 1; i < values.size(); ++i) {
		if (values.at(i) < values.at(i - 1))
			return false;
	}

	return true;
}

int AlgorithmSimulator::HeightAlgorithm(GraphicsNode* node) {

	_currentAlgorithm = Algorithms::HEIGHT;

	if (!node)
		return -1;

	int hleft = HeightAlgorithm(node->leftNode());
	addNode(node);
	int hright = HeightAlgorithm(node->rightNode());

	return 1 + (hleft >= hright ? hleft : hright);

}

GraphicsNode* AlgorithmSimulator::DFSAlgorithm(GraphicsNode* node, Algorithms algorithmType) {

	_currentAlgorithm = algorithmType;

	if (!node)
		return 0;

	if (algorithmType == Algorithms::DFS_PRE_ORDER) 
		addNode(node);

	DFSAlgorithm(node->leftNode(), algorithmType);

	if (algorithmType == Algorithms::DFS_ORDERED) 
		addNode(node);

	DFSAlgorithm(node->rightNode(), algorithmType);

	if (algorithmType == Algorithms::DFS_POST_ORDER) 
		addNode(node);

	return 0;
}

QMap<GraphicsNode*, Direction> AlgorithmSimulator::BSTInsertAlgorithm(GraphicsNode* node, int value) {

	_currentAlgorithm = Algorithms::BST_INSERT;

	GraphicsNode* parentNode = 0, *childrenNode = 0;
	Direction lastDir;

	while (node != 0 && parentNode == 0) {
		addNode(node);
		if (node->intValue() == value) 
			return QMap<GraphicsNode*, Direction>();
		else if (value < node->intValue()) {
			lastDir = Direction::LEFT;
			if (!node->leftNode())
				parentNode = node;
			else
				node = node->leftNode();
		}
		else {
			lastDir = Direction::RIGHT;
			if (!node->rightNode())
				parentNode = node;
			else
				node = node->rightNode();
		}
	}

	if (!parentNode)
		return QMap<GraphicsNode*, Direction>();
		
	QMap<GraphicsNode*, Direction> map;
	map.insert(parentNode, lastDir);

	return map;
	
}

GraphicsNode* AlgorithmSimulator::BSTPredecessor(GraphicsNode* node) {

	_currentAlgorithm = Algorithms::BST_PREDECESSOR;

	if (!node)
		return 0;

	if (node->leftNode())
		return BSTMaxAlgorithm(node->leftNode());

	qDebug() << node->intValue();
	qDebug() << node->parentNode()->intValue();
	
	while (node->parentNode() != 0 && node->parentNode()->leftNode() == node) {
		addNode(node);
		node = node->parentNode();
	}

	addNode(node);
	addNode(node->parentNode());
	return node->parentNode();

}

GraphicsNode* AlgorithmSimulator::addNode(GraphicsNode* node) {

	// Add to the set of nodes
	if (node != 0)
		_nodes.push_back(node);

	return node;

}

GraphicsNode* AlgorithmSimulator::algorithmStep() {

	if (_nodes.isEmpty()) 
		return 0;

	if (_nodeIndex + 1 < _nodes.size())
		++_nodeIndex;

	GraphicsNode* node = _nodes.at(_nodeIndex);
	
	return node;

}

GraphicsNode* AlgorithmSimulator::algorithmCurrentNode() {
	if (_nodeIndex < _nodes.size()) {
		return _nodes.at(_nodeIndex);
	}
	return 0; 
}


GraphicsNode* AlgorithmSimulator::algorithmStepBack() {

	if (_nodes.isEmpty() || _nodeIndex < 0)
		return 0;
	
	GraphicsNode* node = _nodes.at(_nodeIndex);
	
	--_nodeIndex;

	return node;

}

void AlgorithmSimulator::stop() {

	_nodes.clear();
	_nodeIndex = -1;
	_bstFoundValue = -1;

}