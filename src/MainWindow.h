#pragma once

#include "Editor.h"
#include "AlgorithmSimulator.h"

#include <QMainWindow>
#include <QMenuBar>
#include <QToolBar>
#include <QAction>

namespace trexp {
	class MainWindow;
}


class trexp::MainWindow : public QMainWindow {

	Q_OBJECT

	private:

		AlgorithmSimulator* _algoSimulator = 0;

		QMenuBar* _menuBar = 0;
		QToolBar* _algoToolbar = 0;

		QAction* _exportImageAct = 0;
		QAction* _eraseTreeAct = 0;
		QAction* _newTreeAct = 0;
		QAction* _openTreeAct = 0;
		QAction* _editModeAct = 0;
		QAction* _randomTreeValuesAct = 0;
		QAction* _randomTreeAct = 0;
		QAction* _saveTreeAct = 0;
		QAction* _saveTreeWithNameAct = 0;
		QAction* _algoStepAct = 0;
		QAction* _algoStepBackAct = 0;
		QAction* _algoStopAct = 0;
		QAction* _aboutAct = 0;

		//Algorithms
		QAction* _dfsAlgorithmPreOrderAct = 0;
		QAction* _dfsAlgorithmOrderedAct = 0;
		QAction* _dfsAlgorithmPostOrderAct = 0;
		QAction* _heightAlgorithmAct = 0;
		QAction* _bfsAlgorithmAct = 0;
		QAction* _checkBSTAct = 0;
		QAction* _bstMaxAlgorithmAct = 0;
		QAction* _bstMinAlgorithmAct = 0;
		QAction* _bstFindAlgorithmAct = 0;
		QAction* _bstInsertAlgorithmAct = 0;
		QAction* _bstPredecessorAlgorithmAct = 0;

		QMenu* _commandsMenu = 0;
		QMenu* _algorithmsMenu = 0;
		QMenu* _fileMenu = 0;
		QMenu* _dfsAlgorithmMenu = 0;
		QMenu* _bstAlgorithmMenu = 0;

	public:

		MainWindow();

	private slots:

		void exportImage();
		void newTree();
		void save();
		void saveAs();
		void editMode();
		void eraseTree();
		void randomTree();
		void randomTreeValues();
		bool checkBST();
		void about();
		void openTree();
		
		void algorithmStep();
		void algorithmStepBack();
		void algorithmStop();

		void algorithms();

};