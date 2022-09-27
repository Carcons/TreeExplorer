#pragma once

#include "GraphicsNode.h"

#include <QObject>
#include <QList>
#include <QMap>
#include <QDebug>

namespace trexp{
	class AlgorithmSimulator;
}


class trexp::AlgorithmSimulator : public QObject {

	Q_OBJECT

	private:

		int _nodeIndex = -1;
		int _bstFoundValue = -1;

		QList<GraphicsNode*> _nodes;
		Algorithms _currentAlgorithm;

		void checkBSTAlgorithm(GraphicsNode* node, QList<int>* values);

		GraphicsNode* addNode(GraphicsNode* node);

	public:

		AlgorithmSimulator();
		
		GraphicsNode* algorithmStep();
		GraphicsNode* algorithmStepBack();
		GraphicsNode* algorithmCurrentNode();
	
		GraphicsNode* DFSAlgorithm(GraphicsNode* node, Algorithms algorithmType);
		GraphicsNode* BSTMaxAlgorithm(GraphicsNode* node);
		GraphicsNode* BSTMinAlgorithm(GraphicsNode* node);
		GraphicsNode* BSTFindAlgorithm(GraphicsNode* node, int value);
		GraphicsNode* BSTPredecessor(GraphicsNode* node);

		QMap<GraphicsNode*, Direction> BSTInsertAlgorithm(GraphicsNode* node, int value);

		Algorithms currentAlgorithm() { return _currentAlgorithm; }

		QList<GraphicsNode*> nodes() { return _nodes; }

		int HeightAlgorithm(GraphicsNode* node);
		int currentNodesIndex() { return _nodeIndex; }
		int bstFoundValue() { return _bstFoundValue; }
	
		bool checkBST(GraphicsNode* node);
		

		void stop();
		void BSTInsert(GraphicsNode* node) { addNode(node); }
		void BFSAlgorithm(GraphicsNode* node);
	
		


};