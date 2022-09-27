#include "Editor.h"
#include "Graphics_view_zoom.h"

#include <QDebug>
#include <QTimer>
#include <QScrollBar>
#include <QMessageBox>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <queue>
#include <QRandomGenerator>
#include <QFileDialog>


using namespace trexp;

// Class PaintArea
static Editor* uniqueInstance = 0;

Editor* Editor::instance(QWidget* parent) {
	if (uniqueInstance == 0)
		uniqueInstance = new Editor(parent);
	return uniqueInstance;
}

Editor::Editor(QWidget* parent) : QGraphicsView(parent) {

	_scene = new QGraphicsScene(this);
	setScene(_scene);

	_rootNode = drawRoot(_ROOT_POS_X, _ROOT_POS_Y);

	Graphics_view_zoom* z = new Graphics_view_zoom(this);
	z->set_modifiers(Qt::NoModifier);

	_scene->setSceneRect(0, 0, _SCENE_WIDTH, _SCENE_HEIGHT);

	setCacheMode(CacheBackground);
	setBackgroundBrush(QBrush(Qt::lightGray));

}

void Editor::readTree(QString& path) {

	QFile f(path);
	if (!f.open(QIODevice::ReadOnly)) {
		QMessageBox::critical(this, "Error", "Can't open the tree file");
		f.close();
		return;
	}

	QJsonObject json = QJsonDocument::fromJson(f.readAll()).object();
	f.close();

	
	for (auto& key : json.keys()) {

		if (key == "root") {
			qreal x = json.value(key).toObject().value("x").toDouble();
			qreal y = json.value(key).toObject().value("y").toDouble();
			_scene->clear();
			_rootNode = drawRoot(x, y);
			_rootNode->setValue(json.value(key).toObject().value("value").toInt());
			_scene->update();
			continue;
		}

		QStringList movements = key.remove("root").split('.');
		GraphicsNode* currNode = _rootNode;
		QString path = "root";
		for (QString movement : movements) {
			if (movement == "left") {
				path += ".left";
				if (!currNode->leftNode()) {
					qreal posx = json.value(path).toObject().value("x").toDouble();
					qreal posy = json.value(path).toObject().value("y").toDouble();
					GraphicsNode* newNode = drawNode(currNode, Direction::LEFT, true, posx, posy);
					newNode->setValue(json.value(path).toObject().value("value").toInt());
					currNode->setLeftNode(newNode);
					currNode = newNode;
				}
				else 
					currNode = currNode->leftNode();
			}
			else if (movement == "right") {
				path += ".right";
				if (!currNode->rightNode()) {
					qreal posx = json.value(path).toObject().value("x").toDouble();
					qreal posy = json.value(path).toObject().value("y").toDouble();
					GraphicsNode* newNode = drawNode(currNode, Direction::RIGHT, true, posx, posy);
					newNode->setValue(json.value(path).toObject().value("value").toInt());
					currNode->setRightNode(newNode);
					currNode = newNode;
				}
				else
					currNode = currNode->rightNode();
			}
		}

	}
	

	_treeFilePath = path;


}

bool Editor::saveTree(QString newPath) {


	if (_treeFilePath.isEmpty()) {
		QString fileName = QFileDialog::getSaveFileName(this, "Save tree", QString(), "Tree Explorer file (*.trexp)");
		if (fileName.isEmpty())
			return false;
		_treeFilePath = fileName;
	}

	QFile f(newPath == "" ? _treeFilePath : newPath);
	if (!f.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
		f.close();
		return false;
	}

	if (newPath != "")
		_treeFilePath = newPath;

	QJsonObject json;

	std::queue<GraphicsNode*> queue;
	queue.push(_rootNode);
	while (!queue.empty()) {

		GraphicsNode* node = queue.front();
		queue.pop();

		if (!node)
			continue;


		QJsonObject nodeObj;
		nodeObj.insert("value", node->intValue());
		nodeObj.insert("x", node->boundingRect().x() + node->x());
		nodeObj.insert("y", node->boundingRect().y());
		if (node != _rootNode)
			json.insert(node->calculatePathToRoot(), nodeObj);
		else
			json.insert("root", nodeObj);

		queue.push(node->leftNode());
		queue.push(node->rightNode());

	}

	f.write(QJsonDocument(json).toJson());
	
	f.close();
	
	return true;
}

void Editor::removeLeaf(GraphicsNode* node) {

	GraphicsNode* parent = node->parentNode();

	if (!parent) return;

	if (parent->rightNode() == node)
		parent->removeRightNode();
	else if (parent->leftNode() == node)
		parent->removeLeftNode();


}

void Editor::resetNodesColors() {

	for (QGraphicsItem* item : _scene->items()) {
		GraphicsNode* node = dynamic_cast<GraphicsNode*>(item);
		if (!node) continue;
		node->fillColor(Qt::white);
	}

}

void Editor::eraseTree() {

	_scene->clear();
	_rootNode = drawNode();

	_scene->update();

}

QList<QPoint> Editor::nodesCoordinates() {

	QList<QPoint> points;

	for (QGraphicsItem* item : _scene->items()) {
		GraphicsNode* node = dynamic_cast<GraphicsNode*>(item);
		if (node) 
			points.append(QPoint(
				node->boundingRect().toRect().x() + node->x(),
				node->boundingRect().toRect().y()
			));
	}

	return points;

}

void Editor::randomTreeFromRootNode(GraphicsNode* rootNode, Direction gotoDir, const int height) {

	int randomValue;
	GraphicsNode* node = gotoDir == Direction::LEFT ? rootNode->leftNode() : rootNode->rightNode();
	while (calculateTreeHeight(gotoDir == Direction::LEFT ? rootNode->leftNode() : rootNode->rightNode()) <= height) {

		randomValue = QRandomGenerator::global()->bounded(1, 99);

		node->setValue(randomValue);

		if (randomValue % 2 == 0)
			node->setLeftNode(drawNode(node, Direction::LEFT));
		else
			node->setRightNode(drawNode(node, Direction::RIGHT));

		if (node->rightNode()) {
			// 50% create a left node and go left, 50% go right
			randomValue = QRandomGenerator::global()->bounded(1, 99);
			if (randomValue % 2 == 0) {
				node->setLeftNode(drawNode(node, Direction::LEFT));
				node->rightNode()->setValue(randomValue);
				node = node->leftNode();
			}
			else
				node = node->rightNode();
		}
		else if (node->leftNode()) {
			// 50% create a right node and go right, 50% go left
			randomValue = QRandomGenerator::global()->bounded(1, 99);
			if (randomValue % 2 == 0) {
				node->setRightNode(drawNode(node, Direction::RIGHT));
				node->leftNode()->setValue(randomValue);
				node = node->rightNode();
			}
			else
				node = node->leftNode();
		}

		node->setValue(randomValue);

	}
}

void Editor::randomTree(bool randomValues) {

	if (randomValues) {
		for (int i = 0; i < _scene->items().size(); i++) {
			GraphicsNode* node = dynamic_cast<GraphicsNode*>(_scene->items().at(i));
			if (node)
				node->setValue(QRandomGenerator::global()->bounded(1, 99));
		}

		return;
	}

	int height = QRandomGenerator::global()->bounded(1, 5);

	// Setup left and right ( try to balance the tree )
	_rootNode->setValue(QRandomGenerator::global()->bounded(1, 99));
	_rootNode->setLeftNode(drawNode(_rootNode, Direction::LEFT));
	_rootNode->leftNode()->setValue(QRandomGenerator::global()->bounded(1, 99));
	_rootNode->setRightNode(drawNode(_rootNode, Direction::RIGHT));
	_rootNode->rightNode()->setValue(QRandomGenerator::global()->bounded(1, 99));

	randomTreeFromRootNode(_rootNode, Direction::LEFT, height);
	randomTreeFromRootNode(_rootNode, Direction::RIGHT, height);


}


GraphicsNode* Editor::removeNode(GraphicsNode* node) {


	if (!node)
		return 0;

	if (!node->leftArc() && !node->rightArc()) {
		removeLeaf(node);
		return 0;
	}

	removeNode(node->leftNode());
	removeNode(node->rightNode());

	removeLeaf(node);

	_scene->update();
		
	return 0;

}

GraphicsNode* Editor::findNodeByValue(int value) {

	for (int i = 0; i < _scene->items().size(); i++) {
		GraphicsNode* currNode = dynamic_cast<GraphicsNode*>(_scene->items().at(i));
		if (currNode) {
			if (currNode->intValue() == value)
				return currNode;
		}
	}

	return 0;

}


void Editor::enableEditing(bool enable) {

	for (QGraphicsItem* item : _scene->items()) {
		GraphicsNode* node = dynamic_cast<GraphicsNode*>(item);
		if (!node) continue;
		node->enableEditing(enable);
	}

}



int Editor::calculateTreeHeight(GraphicsNode* root) {

	if (!root) return 0;

	int heightLeft = calculateTreeHeight(root->leftNode());
	int heightRight = calculateTreeHeight(root->rightNode());

	return 1 + std::max(heightLeft, heightRight);

}

GraphicsNode* Editor::drawRoot(qreal x, qreal y) {

	_scene->clear();
	_scene->update();

	GraphicsNode* node = new GraphicsNode(x, y);
	_scene->addItem(node);

	return node;
}


GraphicsNode* Editor::drawNode(GraphicsNode* parentNode, Direction direction, bool fromFile, qreal posx, qreal posy) {

	GraphicsNode* node = 0;

	if (!parentNode) {
		// If parentNode is null then a root will be added to scene
		node = drawRoot(_ROOT_POS_X, _ROOT_POS_Y);
		node->value()->setFocus();
		return node;
	}
	
	qreal nodex, nodey;
	if (fromFile) {
		nodex = posx;
		nodey = posy;
	}
	else {
		nodex = parentNode->rectX() + parentNode->x() + (direction == Direction::LEFT ? -_NEW_NODE_POS : _NEW_NODE_POS);
		nodey = parentNode->rectY() + _NEW_NODE_POS;
	}
	
	node = new GraphicsNode(
		nodex,
		nodey,
		parentNode
	);
	_scene->addItem(node);


	GraphicsArc* arc = new GraphicsArc(
		parentNode->rectCenterX() + parentNode->x(),
		parentNode->rectCenterY(),
		node->rectCenterX(),
		node->rectCenterY(),
		parentNode,
		node
	);
	_scene->addItem(arc);

	if (direction == Direction::LEFT)
		parentNode->setLeftArc(arc);
	else
		parentNode->setRightArc(arc);
	
	node->value()->setFocus();

	fixScene(node);

	return node;
	
}

void Editor::fixScene(GraphicsNode* node) {

	if (!_scene->sceneRect().contains(node->boundingRect())) {
		QRectF rect = _scene->itemsBoundingRect();
		rect.adjust(-400, -10, 400, 400);
		_scene->setSceneRect(rect);
	}

	_scene->update();

}