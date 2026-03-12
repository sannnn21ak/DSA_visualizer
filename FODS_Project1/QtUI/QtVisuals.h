#pragma once
#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QLabel>
#include <QVBoxLayout>
#include <QPropertyAnimation>
#include <vector>   // <-- ADDED FOR HASH TABLE
#include <list>     // <-- ADDED FOR HASH TABLE
#include "../EngineCore/VisualizerEngine.h"
#include "../AI_Service/AI_Service.h"
#include "../EngineCore/DryRunStep.h" 

class QtVisuals : public QWidget {
    Q_OBJECT
public:
    QtVisuals(VisualizerEngine* engine, AI_Service* ai, QWidget* parent = nullptr);
    void refreshAll();

  
    // This public function is part of a different design, but we leave it.
    void drawHashTable(const std::vector<std::list<int>>& table, int bucketCount);


private slots:
    void onNewStep(const DryRunStep& step);
    void onCleared();
    void showAI(const QString& txt);

private:
    VisualizerEngine* engine;
    AI_Service* ai;
    QGraphicsScene* scene;
    QGraphicsView* view;
    QLabel* aiLabel;
    
    // nextY REMOVED
    // drawArray RENAMED and no longer needs the Y coordinate parameter
    void drawArray(const DryRunStep& step); 
    void drawLinearArray(const DryRunStep& step);
    void drawLinkedList(const DryRunStep& step);
    void drawGraph(const DryRunStep& step);
    void drawBST(const DryRunStep& step);
    
    void drawHashTable(const DryRunStep& step); // <-- ADDED THIS DECLARATION
    
};