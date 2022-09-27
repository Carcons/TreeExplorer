#include "MainWindow.h"

#include <QFileDialog>
#include <QString>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QMap>

using namespace trexp;


MainWindow::MainWindow() : QMainWindow() {

	// Window settings
	setWindowTitle("TreeExplorer");
	setWindowIcon(QIcon(":/icons/logo.png"));
	setMinimumWidth(640);
	setMinimumHeight(480);

	_algoSimulator = new AlgorithmSimulator;

	// Actions
	_exportImageAct = new QAction("Export as image");
	_eraseTreeAct = new QAction("Erase tree");
	_openTreeAct = new QAction("Open tree");
	_newTreeAct = new QAction("New tree");
	_saveTreeAct = new QAction("Save tree");
	_saveTreeWithNameAct = new QAction("Save Tree as");
	_editModeAct = new QAction("Disable editing");
	_randomTreeAct = new QAction("Generate random tree (Beta)");
	_randomTreeValuesAct = new QAction("Generate random tree values");
	_algoStepAct = new QAction(QIcon(""), "Step Algorithm");
	_algoStepBackAct = new QAction(QIcon(""), "Step Back Algorithm");
	_algoStopAct = new QAction(QIcon(""), "Close Algorithm");
	_checkBSTAct = new QAction("Check");
	_aboutAct = new QAction("About");

	_dfsAlgorithmPreOrderAct = new QAction("Pre-order traversal");
	_dfsAlgorithmOrderedAct = new QAction("In-order traversal");
	_dfsAlgorithmPostOrderAct = new QAction("Post-order traversal");
	_heightAlgorithmAct = new QAction("Height");
	_bfsAlgorithmAct = new QAction("BFS");
	_bstMaxAlgorithmAct = new QAction("Find Max");
	_bstMinAlgorithmAct = new QAction("Find Min");
	_bstFindAlgorithmAct = new QAction("Find element");
	_bstInsertAlgorithmAct = new QAction("Insert element");
	_bstPredecessorAlgorithmAct = new QAction("Element's predecessor");

	// Menus
	_algoToolbar = new QToolBar(this);
	_algoToolbar->addAction(_algoStepBackAct);
	_algoToolbar->addAction(_algoStepAct);
	_algoToolbar->addAction(_algoStopAct);
	_algoToolbar->setVisible(false);


	_fileMenu = new QMenu("File");
	_fileMenu->addAction(_newTreeAct);
	_fileMenu->addSeparator();
	_fileMenu->addAction(_openTreeAct);
	_fileMenu->addSeparator();
	_fileMenu->addAction(_saveTreeAct);
	_fileMenu->addAction(_saveTreeWithNameAct);
	_fileMenu->addSeparator();
	_fileMenu->addAction(_exportImageAct);
	

	_commandsMenu = new QMenu("Commands", this);
	_commandsMenu->addAction(_eraseTreeAct);
	_commandsMenu->addSeparator();
	_commandsMenu->addAction(_editModeAct);
	_commandsMenu->addSeparator();
	_commandsMenu->addAction(_randomTreeAct);
	_commandsMenu->addAction(_randomTreeValuesAct);
	_commandsMenu->addSeparator();

	_algorithmsMenu = new QMenu("Algorithms", this);
	_algorithmsMenu->addAction(_heightAlgorithmAct);
	_algorithmsMenu->addAction(_bfsAlgorithmAct);


	_dfsAlgorithmMenu = new QMenu("DFS", this);
	_dfsAlgorithmMenu->addAction(_dfsAlgorithmPreOrderAct);
	_dfsAlgorithmMenu->addAction(_dfsAlgorithmOrderedAct);
	_dfsAlgorithmMenu->addAction(_dfsAlgorithmPostOrderAct);

	_bstAlgorithmMenu = new QMenu("BST", this);
	_bstAlgorithmMenu->addAction(_checkBSTAct);
	_bstAlgorithmMenu->addAction(_bstMaxAlgorithmAct);
	_bstAlgorithmMenu->addAction(_bstMinAlgorithmAct);
	_bstAlgorithmMenu->addAction(_bstFindAlgorithmAct);
	_bstAlgorithmMenu->addAction(_bstInsertAlgorithmAct);
	_bstAlgorithmMenu->addAction(_bstPredecessorAlgorithmAct);


	_algorithmsMenu->addMenu(_dfsAlgorithmMenu);
	_algorithmsMenu->addMenu(_bstAlgorithmMenu);
	_commandsMenu->addMenu(_algorithmsMenu);

	// Create menu bar
	_menuBar = new QMenuBar(this);
	_menuBar->addMenu(_fileMenu);
	_menuBar->addMenu(_commandsMenu);
	_menuBar->addAction(_aboutAct);

	setMenuBar(_menuBar);
	addToolBar(_algoToolbar);

	setCentralWidget(Editor::instance(this));

	// Menus connections
	connect(_aboutAct, &QAction::triggered, this, &MainWindow::about);
	connect(_exportImageAct, &QAction::triggered, this, &MainWindow::exportImage);
	connect(_eraseTreeAct, &QAction::triggered, this, &MainWindow::eraseTree);
	connect(_newTreeAct, &QAction::triggered, this, &MainWindow::newTree);
	connect(_openTreeAct, &QAction::triggered, this, &MainWindow::openTree);
	connect(_saveTreeAct, &QAction::triggered, this, &MainWindow::save);
	connect(_saveTreeWithNameAct, &QAction::triggered, this, &MainWindow::saveAs);
	connect(_editModeAct, &QAction::triggered, this, &MainWindow::editMode);
	connect(_randomTreeAct, &QAction::triggered, this, &MainWindow::randomTree);
	connect(_randomTreeValuesAct, &QAction::triggered, this, &MainWindow::randomTreeValues);
	connect(_dfsAlgorithmPreOrderAct, &QAction::triggered, this, &MainWindow::algorithms);
	connect(_bfsAlgorithmAct, &QAction::triggered, this, &MainWindow::algorithms);
	connect(_dfsAlgorithmOrderedAct, &QAction::triggered, this, &MainWindow::algorithms);
	connect(_dfsAlgorithmPostOrderAct, &QAction::triggered, this, &MainWindow::algorithms);
	connect(_heightAlgorithmAct, &QAction::triggered, this, &MainWindow::algorithms);
	connect(_checkBSTAct, &QAction::triggered, this, &MainWindow::checkBST);
	connect(_bstMaxAlgorithmAct, &QAction::triggered, this, &MainWindow::algorithms);
	connect(_bstMinAlgorithmAct, &QAction::triggered, this, &MainWindow::algorithms);
	connect(_bstFindAlgorithmAct, &QAction::triggered, this, &MainWindow::algorithms);
	connect(_bstInsertAlgorithmAct, &QAction::triggered, this, &MainWindow::algorithms);
	connect(_bstPredecessorAlgorithmAct, &QAction::triggered, this, &MainWindow::algorithms);

	// Toolbar connections
	connect(_algoStepBackAct, &QAction::triggered, this, &MainWindow::algorithmStepBack);
	connect(_algoStepAct, &QAction::triggered, this, &MainWindow::algorithmStep);
	connect(_algoStopAct, &QAction::triggered, this, &MainWindow::algorithmStop);

}

void MainWindow::openTree() {

	QString path = QFileDialog::getOpenFileName(this, "Open tree", QString(), "Tree explorer file (*.trexp)");
	if (!path.isEmpty()) {
		eraseTree();
		Editor::instance()->readTree(path);
	}

}

void MainWindow::eraseTree() {

	_editModeAct->setText("Disable editing");
	Editor::instance()->eraseTree();

}

void MainWindow::about() {

	QMessageBox::about(this, "TreeExplorer",
		"<html><body><h1><strong>Tree Explorer</strong></h1><h3><i>Simple binary tree drawer.</i></h3>"
		"<p>Credits:</p>"
		"<p>Written by Andrea Carcone (<a href='https://github.com/carcons'>Github</a>) <br />"
		"<p>Contributors:</p>"
		"<ul>"
		"<li>Roberta Ruscetta</li>"
		"</ul><body>< / html>");

}

void MainWindow::exportImage() {

	QString fileName = QFileDialog::getSaveFileName(this, "Save image as", QString(), "PNG (*.png);;JPG (*.jpg);;BMP (*.bmp)");
	if (fileName.isEmpty())
		return;

	Editor::instance()->enableEditing(false);

	QList<QPoint> points = Editor::instance()->nodesCoordinates(); 
	QPoint minPoint(
		Editor::instance()->rootNode()->boundingRect().toRect().x() + Editor::instance()->rootNode()->x(),
		Editor::instance()->rootNode()->boundingRect().toRect().y()
	);
	QPoint maxPoint = minPoint;


	// Find max and min in points
	for (QPoint& p : points) { 
		if (p.x() >= maxPoint.x())
			maxPoint.setX(p.x());
		if (p.y() >= maxPoint.y())
			maxPoint.setY(p.y());

		if (p.x() <= minPoint.x())
			minPoint.setX(p.x());
		if (p.y() <= minPoint.y())
			minPoint.setY(p.y());
	}


	QSize imageSize(
		minPoint.x() < 0 || maxPoint.x() < 0 ? qAbs(minPoint.x()) + qAbs(maxPoint.x()) + 300 : maxPoint.x() + 300,
		qAbs(maxPoint.y()) + 200
	);

	QPoint sourceStartPoint(
		minPoint.x() - 100,
		0
	);


	QImage img(imageSize, QImage::Format_ARGB32_Premultiplied);
	
	if (!fileName.endsWith(".png"))
		img.fill(Qt::lightGray);
	else
		img.fill(Qt::transparent);

	QPainter p(&img);
	Editor::instance()->scene()->render(
		&p,
		QRect(QPoint(0, 0), imageSize),
		QRect(sourceStartPoint, imageSize)
	);
	p.end();

	img.save(fileName);
	QMessageBox::information(this, "Image saved", "Image saved at " + fileName);

	Editor::instance()->enableEditing(true);
	
}

void MainWindow::editMode() {

	algorithmStop();

	if (_editModeAct->text() == "Disable editing") {
		_editModeAct->setText("Enable editing");
		Editor::instance()->enableEditing(false);
	}
	else {
		_editModeAct->setText("Disable editing");
		Editor::instance()->enableEditing(true);
	}

}

void MainWindow::algorithmStepBack() {
	
	GraphicsNode* node = _algoSimulator->algorithmStepBack();

	if (!node)
		return;

	node->fillColor(Qt::white);

}

void MainWindow::algorithmStep() {

	GraphicsNode* node = _algoSimulator->algorithmStep();
	if (!node)
		return;

	Algorithms currentAlgorithm = _algoSimulator->currentAlgorithm();
	if (	currentAlgorithm == Algorithms::BST_MAX || 
			currentAlgorithm == Algorithms::BST_MIN || 
			currentAlgorithm == Algorithms::BST_INSERT ||
			currentAlgorithm == Algorithms::BST_PREDECESSOR
		) {

		if (_algoSimulator->nodes().size() - 1 == _algoSimulator->currentNodesIndex())
			node->fillColor(Qt::green);
		else
			node->fillColor(Qt::yellow);

	}
	else if (currentAlgorithm == Algorithms::BST_FIND) {
		if (
			(_algoSimulator->nodes().size() - 1 == _algoSimulator->currentNodesIndex()) &&
			_algoSimulator->bstFoundValue() == _algoSimulator->nodes().at(_algoSimulator->currentNodesIndex())->intValue()
			)
			node->fillColor(Qt::green);
		else
			node->fillColor(Qt::yellow);
	}
	else
		node->fillColor(Qt::green);

}

bool MainWindow::checkBST() {

	bool isBST = _algoSimulator->checkBST(Editor::instance()->rootNode());
	if (isBST)
		QMessageBox::information(this, "BST Check", "This tree is a Binary Search Tree");
	else
		QMessageBox::information(this, "BST Check", "This tree is not a Binary Search Tree");

	return isBST;

}


void MainWindow::algorithms() {

	algorithmStop();

	QAction* action = dynamic_cast<QAction*>(sender());
	if (!action)
		return;

	if (action == _bstMaxAlgorithmAct || 
		action == _bstMinAlgorithmAct || 
		action == _bstFindAlgorithmAct ||
		action == _bstInsertAlgorithmAct ||
		action == _bstPredecessorAlgorithmAct) {
		if (!_algoSimulator->checkBST(Editor::instance()->rootNode())) {
			QMessageBox::critical(this, "Bad BST", "This tree is not a Binary Search Tree.\nPlease create a BST in the editor to run this algorithm");
			return;
		}
	}

	if (action == _dfsAlgorithmPreOrderAct)
		_algoSimulator->DFSAlgorithm(Editor::instance()->rootNode(), Algorithms::DFS_PRE_ORDER);
	else if (action == _dfsAlgorithmOrderedAct)
		_algoSimulator->DFSAlgorithm(Editor::instance()->rootNode(), Algorithms::DFS_ORDERED);
	else if (action == _dfsAlgorithmPostOrderAct)
		_algoSimulator->DFSAlgorithm(Editor::instance()->rootNode(), Algorithms::DFS_POST_ORDER);
	else if (action == _heightAlgorithmAct)
		QMessageBox::information(this, "Height", "Height: " + QString::number(_algoSimulator->HeightAlgorithm(Editor::instance()->rootNode())));
	else if (action == _bfsAlgorithmAct)
		_algoSimulator->BFSAlgorithm(Editor::instance()->rootNode());
	else if (action == _bstMaxAlgorithmAct)
		QMessageBox::information(this, "Max value", "Max: " + QString::number(_algoSimulator->BSTMaxAlgorithm(Editor::instance()->rootNode())->intValue()));
	else if (action == _bstMinAlgorithmAct)
		QMessageBox::information(this, "Min value", "Min: " + QString::number(_algoSimulator->BSTMinAlgorithm(Editor::instance()->rootNode())->intValue()));
	else if (action == _bstFindAlgorithmAct) {

		bool okInput = false, okNumber = false;
		QString str = QInputDialog::getText(this, "Find Element", "Element:", QLineEdit::Normal, QString(), &okInput);
		if (okInput) {
			int val = str.toInt(&okNumber);
			if (okNumber) {
				GraphicsNode* found = _algoSimulator->BSTFindAlgorithm(Editor::instance()->rootNode(), val);
				if (found)
					QMessageBox::information(this, "Found", "Node found with value " + QString::number(val));
				else {
					QMessageBox::information(this, "Not found", "Can't find a node with value " + QString::number(val));
					return;
				}
			}
			else {
				QMessageBox::information(this, "Can't search", "Can't search the node, invalid value");
				return;
			}
		}
		else
			return;

	}
	else if (action == _bstInsertAlgorithmAct) {
		bool okInput, okNumber;
		okInput = okNumber = false;
		QString str = QInputDialog::getText(this, "Insert element", "Element:", QLineEdit::Normal, QString(), &okInput);
		if (okInput) {
			int val = str.toInt(&okNumber);
			if (okNumber) {
				QMap<GraphicsNode*, Direction> result = _algoSimulator->BSTInsertAlgorithm(Editor::instance()->rootNode(), val);
				if (!result.isEmpty()) {
					GraphicsNode* node = result.firstKey();
					if (!node) {
						QMessageBox::information(this, "Can't insert", "Can't add the node");
						return;
					}
					GraphicsNode* addedNode = Editor::instance()->drawNode(node, result.value(node));
					addedNode->setValue(val);

					if (result.value(node) == Direction::LEFT)
						node->setLeftNode(addedNode);
					else
						node->setRightNode(addedNode);

					_algoSimulator->BSTInsert(addedNode);
				}
				else {
					QMessageBox::information(this, "Can't insert", "Node already exists");
					return;
				}
			}
			else {
				QMessageBox::information(this, "Can't insert", "Can't insert a node with value " + QString::number(val));
				return;
			}
		}
		else {
			QMessageBox::information(this, "Can't insert", "Can't insert the node, invalid value");
			return;
		}
	}
	else if (action == _bstPredecessorAlgorithmAct) {
		bool okInput, okNumber;
		okInput = okNumber = false;
		QString str = QInputDialog::getText(this, "Specify element", "Element:", QLineEdit::Normal, QString(), &okInput);
		if (okInput) {
			int val = str.toInt(&okNumber);
			if (okNumber) {
				GraphicsNode* startNode = Editor::instance()->findNodeByValue(val);
				if (startNode)
					_algoSimulator->BSTPredecessor(startNode);
				else {
					QMessageBox::information(this, "Can't find", "Node with value " + QString::number(val) + " not found.");
					return;
				}
			}
			else {
				QMessageBox::information(this, "Invalid value", "Please insert a valid value in order to start the algorithm");
				return;
			}
		}
		else {
			QMessageBox::information(this, "Can't find", "Can't find predecessor");
			return;
		}
	}

	
	_editModeAct->setText("Enable editing");
	Editor::instance()->enableEditing(false);
	_algoToolbar->setVisible(true);

}

void MainWindow::algorithmStop() {

	_algoSimulator->stop();
	_algoToolbar->setVisible(false);
	Editor::instance()->resetNodesColors();

}


void MainWindow::newTree() {

	algorithmStop();

	Editor::instance()->newTree();

}

void MainWindow::randomTreeValues() {

	algorithmStop();
	Editor::instance()->randomTree(true);

}

void MainWindow::randomTree() {

	algorithmStop();
	eraseTree();
	Editor::instance()->randomTree();

}

void MainWindow::save() {

	if (Editor::instance()->saveTree())
		QMessageBox::information(this, "Saved", "Tree saved");

}

void MainWindow::saveAs() {

	QString path = QFileDialog::getSaveFileName(this, "Save tree", "", "Tree Explorer file (*.trexp)");
	if (!path.isEmpty()) {
		if(Editor::instance()->saveTree(path))
			QMessageBox::information(this, "Saved", "Tree saved at " + path);
		else
			QMessageBox::critical(this, "Error", "Can't save the tree file");
	}

}