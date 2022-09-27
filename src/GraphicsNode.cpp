#include "GraphicsNode.h"
#include "Editor.h"

#include <QString>
#include <QInputDialog>

#include <QDebug>

using namespace trexp;


// Class GraphicsNodeAdder

GraphicsNodeAdder::GraphicsNodeAdder(GraphicsNode* parent, Direction direction, bool add) :
	QGraphicsPixmapItem(parent) {

	_node = parent;
	_direction = direction;
	_add = add;

	setZValue(2);

	QPixmap pixmap;
	if (add)
		pixmap = pixmap.fromImage(QImage(":icons/addIcon.png"));
	else
		pixmap = pixmap.fromImage(QImage(":icons/delIcon.png"));

	pixmap = pixmap.scaled(QSize(40, 40), Qt::IgnoreAspectRatio);

	setPixmap(pixmap);

}

GraphicsNodeAdder::~GraphicsNodeAdder() {}

void GraphicsNodeAdder::mousePressEvent(QGraphicsSceneMouseEvent* event) {

	event->accept();
	prepareGeometryChange();
	if (_add) {
		if (_direction == Direction::LEFT && !_node->leftNode())
			_node->setLeftNode(Editor::instance()->drawNode(_node, Direction::LEFT));
		else if (_direction == Direction::RIGHT && !_node->rightNode())
			_node->setRightNode(Editor::instance()->drawNode(_node, Direction::RIGHT));
	}
	else 
		Editor::instance()->removeNode(_node);


}

// Class GraphicsArc

GraphicsArc::GraphicsArc(qreal x1, qreal y1, qreal x2, qreal y2, GraphicsNode* startNode, GraphicsNode* endNode)
	: QGraphicsLineItem(x1, y1, x2, y2) {

	_startNode = startNode;
	_endNode = endNode;

	setPen(QPen(Qt::black, 4, Qt::SolidLine));

}

GraphicsArc::~GraphicsArc() {}

// Class GraphicsText
GraphicsText::GraphicsText(GraphicsNode* parent) : QGraphicsTextItem(parent) {

	_parent = parent;
	
	setTextInteractionFlags(Qt::TextEditable);

	QFont f;
	f.setPointSize(20);
	setFont(f);

	setX(parent->boundingRect().center().x() - 45);
	setY(parent->boundingRect().center().y() - boundingRect().height() / 2);
	setZValue(3);

	setTextAlignment();

	setTextWidth(90);
	

}

void GraphicsText::mousePressEvent(QGraphicsSceneMouseEvent* e) {

	if (!_parent->canMove()) {
		e->ignore();
		return;
	}

	if (e->button() == Qt::MouseButton::RightButton) {
		setTextInteractionFlags(Qt::NoTextInteraction);
		e->ignore();
	}
	else if (e->button() == Qt::MouseButton::LeftButton) {
		e->accept();
		setTextInteractionFlags(Qt::TextEditable);
	}

}

void GraphicsText::keyPressEvent(QKeyEvent* e) {

	if (!_parent->canMove())
		return;


	if (e->key() >= Qt::Key_0 && e->key() <= Qt::Key_9 && toPlainText().size() <= 1) {
		QGraphicsTextItem::keyPressEvent(e);
		return;
	} 

	if (e->key() != Qt::Key_Delete && e->key() != Qt::Key_Backspace)
		return;
	else
		QGraphicsTextItem::keyPressEvent(e);
	
		

}

void GraphicsText::setTextAlignment() {
	QTextBlockFormat format;
	format.setAlignment(Qt::AlignCenter);
	QTextCursor cursor = textCursor();
	cursor.select(QTextCursor::Document);
	cursor.mergeBlockFormat(format);
	cursor.clearSelection();
}

void GraphicsText::setValue(int value) {
	setPlainText(QString::number(value));
	setTextAlignment();
}

GraphicsText::~GraphicsText() {}

// GraphicsNode class
GraphicsNode::GraphicsNode(qreal x, qreal y, GraphicsNode* parentNode) : 
	QGraphicsEllipseItem(0) {

	setRect(QRect(x, y, 120, 120));
	setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);

	setZValue(1);
	resetColor();

	_parentNode = parentNode;
	

	// Text settings
	_value = new GraphicsText(this);
	
	// Add left settings
	_leftAdder = new GraphicsNodeAdder(this, Direction::LEFT);
	_leftAdder->setPos(boundingRect().topLeft() + QPoint(-25, +85));

	// Add right settings
	_rightAdder = new GraphicsNodeAdder(this, Direction::RIGHT);
	_rightAdder->setPos(boundingRect().topRight() + QPoint(-15, +85));
	
	// Remove settings (if is not the root (parent = 0))
	if (_parentNode) {
		_remover = new GraphicsNodeAdder(this, Direction::LEFT, false);
		_remover->setPos(boundingRect().center() + QPoint(-20, +40));
		_remover->setVisible(true);
	}

	_leftAdder->setVisible(true);
	_rightAdder->setVisible(true);
	

}

GraphicsNode::~GraphicsNode() {}

void GraphicsNode::enableEditing(bool edit) {

	_leftAdder->setVisible(edit);
	_rightAdder->setVisible(edit);
	if (_remover) _remover->setVisible(edit);

	if (!edit) {
		_canMove = false;
		_value->setTextInteractionFlags(Qt::NoTextInteraction);
	}
	else {
		_canMove = true;
		_value->setTextInteractionFlags(Qt::TextEditable);
	}

	

}

QString GraphicsNode::calculatePathToRoot() {

	QString dir = "root";
	QStringList dirLst;
	GraphicsNode* it = parentNode();
	GraphicsNode* prev = this;
	while (it != 0) {
		if (it->leftNode() && it->leftNode() == prev)
			dirLst << ".left";
		else if (it->rightNode() && it->rightNode() == prev)
			dirLst << ".right";

		prev = it;
		it = it->parentNode();
	}

	// Reverse order
	for (int i = dirLst.size() - 1; i >= 0; i--)
		dir += dirLst.at(i);

	return dir;

}

void GraphicsNode::mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event) {
	if (_canMove)
		_value->setFocus();
}

void GraphicsNode::mouseMoveEvent(QGraphicsSceneMouseEvent* event) {

	if (!_canMove)
		return;
	
	QGraphicsEllipseItem::mouseMoveEvent(event);

	if (y() != 0)
		setPos(x(), 0);

	if (x() != 0) {

		if (_parentNode) {

			GraphicsArc* parentRArc = _parentNode->rightArc();
			GraphicsArc* parentLArc = _parentNode->leftArc();
			if (parentRArc && this == parentRArc->endNode()) 
				parentRArc->setLine(
					parentRArc->posX1(),
					parentRArc->posY1(),
					rectCenterX() + parentRArc->endNode()->x(), 
					parentRArc->posY2()
				);
			else if (parentLArc && this == parentLArc->endNode()) 
				parentLArc->setLine(
					parentLArc->posX1(), 
					parentLArc->posY1(),
					rectCenterX() + parentLArc->endNode()->x(),
					parentLArc->posY2()
				);
			
		}

		if (_leftArc && this == _leftArc->startNode()) 
			_leftArc->setLine(
				rectCenterX() + x(),
				_leftArc->posY1(),
				_leftArc->endNode()->rectCenterX() + _leftArc->endNode()->x(),
				_leftArc->posY2()
			);
		
		if (_rightArc && this == _rightArc->startNode())
			_rightArc->setLine(rectCenterX() + x(),
				_rightArc->posY1(),
				_rightArc->endNode()->rectCenterX() + _rightArc->endNode()->x(),
				_rightArc->posY2()
			);

	}


}


void GraphicsNode::removeLeftArc() { 
	if (_leftArc) {
		Editor::instance()->scene()->removeItem(_leftArc);
		_leftArc = 0;
	}
}
void GraphicsNode::removeRightArc() { 
	if (_rightArc) {
		Editor::instance()->scene()->removeItem(_rightArc);
		_rightArc = 0;
	}
}
void GraphicsNode::removeLeftNode() {
	if (_leftNode) {
		removeLeftArc();
		Editor::instance()->scene()->removeItem(_leftNode);
		_leftNode = 0;
	}
}
void GraphicsNode::removeRightNode() {
	if (_rightNode) {
		removeRightArc();
		Editor::instance()->scene()->removeItem(_rightNode);
		_rightNode = 0;
		
	}
}




