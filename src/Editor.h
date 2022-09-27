#pragma once

#include "GraphicsNode.h"
#include "global.h"

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsRectItem>
#include <QPaintEvent>
#include <QWheelEvent>
#include <QThread>
#include <QMap>


namespace trexp {
	class Editor;
}


class trexp::Editor : public QGraphicsView {

	Q_OBJECT

	private:

		const int _SCENE_WIDTH  = 1000,
				  _SCENE_HEIGHT = 1000,
				  _ROOT_POS_X   =  500,
				  _ROOT_POS_Y   =    5,
				  _NEW_NODE_POS =  160;

		QString _treeFilePath;

		QGraphicsScene* _scene = 0;
		GraphicsNode* _rootNode = 0;

		Editor(QWidget* parent = 0);

		void randomTreeFromRootNode(GraphicsNode* rootNode, Direction gotoDir,const int height);
		
		GraphicsNode* drawRoot(qreal x, qreal y);

	public:

		static Editor* instance(QWidget* parent = 0);
		
		GraphicsNode* drawNode(GraphicsNode* parentNode = 0, Direction direction = Direction::LEFT, bool fromFile = false, qreal posx = 0, qreal posy = 0);
		GraphicsNode* rootNode() { return _rootNode; }
		GraphicsNode* removeNode(GraphicsNode* node);
		GraphicsNode* findNodeByValue(int value);

		int  calculateTreeHeight(GraphicsNode* root);

		void enableEditing(bool enable);
		void removeLeaf(GraphicsNode* node);
		void randomTree(bool randomValues = false);
		void newTree() { _treeFilePath = ""; eraseTree(); }
		void eraseTree();
		void resetNodesColors();
		bool saveTree(QString newPath = "");
		void readTree(QString& path);
		void fixScene(GraphicsNode* node);

		QList<QPoint> nodesCoordinates();
		
		 
		
		
};