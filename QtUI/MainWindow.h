#pragma once
#include <QMainWindow>
#include <QComboBox>
#include <QLineEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include "../EngineCore/VisualizerEngine.h"
#include "../AI_Service/AI_Service.h"
#include "QtVisuals.h"
#include "../DataStructures/DynamicArray.h"
#include "../DataStructures/Queue.h"
#include "../DataStructures/Stack.h"
#include "../DataStructures/LinkedList.h"
#include "../DataStructures/Graph.h"
#include "../DataStructures/BST.h"
#include "../DataStructures/HashTable.h" // <--- ADDED THIS INCLUDE

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget* parent = nullptr);
private slots:
    void onAddClicked();
    void onRemoveClicked();
    void onClearClicked();
    void onStructureChanged(int idx);

private:
    VisualizerEngine* engine;
    AI_Service* ai;
    QtVisuals* visuals;
    DynamicArray* dynamicArray;
    Queue* queue;
    Stack* stack;
    LinkedList* linkedList;
    Graph* graph;
    BST* bst;
    HashTable* hashTable; // <--- ADDED THIS POINTER

    QComboBox* structureBox;
    QLineEdit* valueEdit;
    QPushButton* addBtn;
    QPushButton* removeBtn;
    QPushButton* clearBtn;
};