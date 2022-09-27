#pragma once

#include "global.h"

#include <QGraphicsScene>
#include <QGraphicsEllipseItem>
#include <QGraphicsTextItem>
#include <QInputDialog>
#include <QPainter>
#include <QGraphicsLineItem>
#include <QTextCursor>
#include <QGraphicsSceneMouseEvent>

namespace trexp {
	class GraphicsNodeAdder;
	class GraphicsArc;
	class GraphicsText;
	class GraphicsNode;
}

class trexp::GraphicsNodeAdder : public QGraphicsPixmapItem {


private:

	GraphicsNode* _node = 0;
	QGraphicsScene* _parentScene = 0;

	bool _add = true;

	Direction _direction;

public:

	GraphicsNodeAdder(GraphicsNode* parent, Direction direction, bool add = true);
	~GraphicsNodeAdder();
	void mousePressEvent(QGraphicsSceneMouseEvent* event) override;



};

class trexp::GraphicsArc : public QGraphicsLineItem {

private:

	GraphicsNode* _startNode = 0;
	GraphicsNode* _endNode = 0;


public:
	GraphicsArc(qreal x1, qreal y1, qreal x2, qreal y2, GraphicsNode* startNode, GraphicsNode* endNode);
	~GraphicsArc();
	GraphicsNode* startNode() { return _startNode; }
	GraphicsNode* endNode() { return _endNode; }
	qreal posX1() { return line().x1(); }
	qreal posY1() { return line().y1(); }
	qreal posX2() { return line().x2(); }
	qreal posY2() { return line().y2(); }

};


class trexp::GraphicsText : public QGraphicsTextItem {

private:

	GraphicsNode* _parent = 0;

	void setTextAlignment();
	void keyPressEvent(QKeyEvent* e) override;
	void mousePressEvent(QGraphicsSceneMouseEvent* e) override;
	

public:

	GraphicsText(GraphicsNode* parent);
	~GraphicsText();

	void setValue(int value);
	int value() { return toPlainText().toInt(); }

	GraphicsNode* parent() { return _parent; }



};



class trexp::GraphicsNode : public QGraphicsEllipseItem {

	private:

		bool _canMove = true;

		GraphicsNodeAdder* _leftAdder = 0;
		GraphicsNodeAdder* _rightAdder = 0;
		GraphicsNodeAdder* _remover = 0;

		GraphicsText* _value = 0;

		GraphicsNode* _leftNode = 0;
		GraphicsNode* _rightNode = 0;
		GraphicsNode* _parentNode = 0;

		GraphicsArc* _rightArc = 0;
		GraphicsArc* _leftArc = 0;

		void mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event) override;
		void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;
		
		
	public:

		GraphicsNode(qreal x, qreal y, GraphicsNode* parentNode = 0);
		~GraphicsNode();

		bool canMove() { return _canMove; }

		int intValue() { return _value->value(); }

		qreal rectX() { return rect().x(); }
		qreal rectY() { return rect().y(); }
		qreal rectCenterX() { return rect().center().rx(); }
		qreal rectCenterY() { return rect().center().ry(); }
		GraphicsNode* parentNode() { return _parentNode; }
		GraphicsNode* leftNode() { return _leftNode; }
		GraphicsNode* rightNode() { return _rightNode; }
		GraphicsArc* leftArc() { return _leftArc; }
		GraphicsArc* rightArc() { return _rightArc; }
		GraphicsText* value() { return _value; }
		QString calculatePathToRoot();

		void setLeftNode(GraphicsNode* node) { _leftNode = node; }
		void setRightNode(GraphicsNode* node) { _rightNode = node; }
		void setRightArc(GraphicsArc* arc) { _rightArc = arc; }
		void setLeftArc(GraphicsArc* arc) { _leftArc = arc; }
		void resetColor() { setBrush(Qt::GlobalColor::white); }
		void fillColor(Qt::GlobalColor color) { setBrush(color); }
		void removeLeftArc();
		void removeRightArc();
		void removeLeftNode();
		void removeRightNode();
		void enableEditing(bool edit);
		void setValue(int value) { _value->setValue(value); }

};

