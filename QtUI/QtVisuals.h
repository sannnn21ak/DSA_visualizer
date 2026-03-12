#pragma once
#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QLabel>
#include <QVBoxLayout>
#include <QPropertyAnimation>
#include <vector>
#include <list>
#include "../EngineCore/VisualizerEngine.h"
#include "../AI_Service/AI_Service.h"
#include "../EngineCore/DryRunStep.h"

class QtVisuals : public QWidget {
    Q_OBJECT
public:
    QtVisuals(VisualizerEngine* engine, AI_Service* ai, QWidget* parent = nullptr);
    void refreshAll();

    // Optional: public overload for showing a manual hash table
    void drawHashTable(const std::vector<std::list<int>>& table, int bucketCount);

private slots:
    void onNewStep(const DryRunStep& step);
    void onCleared();
    void showAI(const QString& txt);

private:
    // --- Draw functions for each data structure ---
    void drawArray(const DryRunStep& step);
    void drawLinearArray(const DryRunStep& step);
    void drawLinkedList(const DryRunStep& step);
    void drawGraph(const DryRunStep& step);
    void drawBST(const DryRunStep& step);
    void drawHashTable(const DryRunStep& step);

    // --- Helper drawing functions ---
    QGraphicsItem* drawNode(const QString& label, int x, int y, const QColor& color, int radius = 20);
    void drawConnector(QGraphicsItem* start, QGraphicsItem* end, const QColor& color);
    void drawConnector(QPointF start, QPointF end, const QColor& color);
    QGraphicsItem* drawArrayBox(const QString& label, int x, int y, int width, int height, const QColor& color);

    // --- Members ---
    VisualizerEngine* engine;
    AI_Service* ai;
    QGraphicsScene* scene;
    QGraphicsView* view;
    QLabel* aiLabel;
};
