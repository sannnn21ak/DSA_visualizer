#include "QtVisuals.h"
#include <QGraphicsRectItem>
#include <QGraphicsItemAnimation>
#include <QGraphicsTextItem>
#include <QGraphicsLineItem>
#include <QGraphicsPolygonItem>
#include <QTimeLine>
#include <QMetaType>
#include <QFont> 
#include <QPainter> 
#include <QDebug>
#include <QVBoxLayout>
#include <list>     // <--- ADDED
#include <vector>   // <--- ADDED

Q_DECLARE_METATYPE(DryRunStep)

// --- Helper function for Aggressive Cleanup ---
void aggressiveClear(QGraphicsScene* scene) {
    // Manually delete items to ensure no lingering QObject issues
    for (QGraphicsItem* item : scene->items()) {
        delete item;
    }
    scene->clear();
}

// --- Constructor (Correct and Final) ---
QtVisuals::QtVisuals(VisualizerEngine* engine_, AI_Service* ai_, QWidget* parent)
    : QWidget(parent), engine(engine_), ai(ai_) 
{
    qRegisterMetaType<DryRunStep>("DryRunStep");

    scene = new QGraphicsScene(this);
    view = new QGraphicsView(scene, this);
    view->setRenderHint(QPainter::Antialiasing);
    view->setMinimumSize(820, 520);
    view->setBackgroundBrush(QColor("#1b1b1b"));
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff); 

    aiLabel = new QLabel("AI explanation will appear here.", this);
    aiLabel->setStyleSheet("QLabel { color: #cfcfcf; font-size: 13px; padding: 6px; }");
    aiLabel->setWordWrap(true);
    aiLabel->setMinimumHeight(60);

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(view);
    layout->addWidget(aiLabel);
    setLayout(layout);

    engine->onNewStep = [this](const DryRunStep& s) {
        QMetaObject::invokeMethod(this, "onNewStep", Qt::QueuedConnection, Q_ARG(DryRunStep, s));
    };
    engine->onCleared = [this]() {
        QMetaObject::invokeMethod(this, "onCleared", Qt::QueuedConnection);
    };

    connect(ai, &AI_Service::explanationReady, this, &QtVisuals::showAI);

    refreshAll();
}

// --- Logic functions (Dispatch and Cleanup) ---

void QtVisuals::refreshAll() {
    aggressiveClear(scene); 
    const auto& steps = engine->getSteps();
    if (!steps.empty()) {
        const DryRunStep& step = steps.back();
        if (step.dataStructure == "BST") {
            drawBST(step);
        } 
        else if (step.dataStructure == "Graph") {
            drawGraph(step);
        }
        else if (step.dataStructure == "LinkedList") {
            drawLinkedList(step);
        }
        else if (step.dataStructure == "HashTable") {
            drawHashTable(step);
        }
        // --- END ADD ---
        else {
            drawLinearArray(step);
        }
    }
    if (!scene->items().isEmpty()) {
        scene->setSceneRect(scene->itemsBoundingRect());
        view->centerOn(scene->itemsBoundingRect().center());
    }
}

void QtVisuals::onNewStep(const DryRunStep& step) {
    aggressiveClear(scene); // CRITICAL FIX: Aggressively delete all items first
    
    // Visualization Dispatcher
    if (step.dataStructure == "BST") {
        drawBST(step);
    } 
    else if (step.dataStructure == "Graph") {
        drawGraph(step);
    }
    else if (step.dataStructure == "LinkedList") {
        drawLinkedList(step);
    }
    else if (step.dataStructure == "HashTable") {
        drawHashTable(step);
    }
    // --- END ADD ---
    else {
        drawLinearArray(step);
    }   

    
    if (!scene->items().isEmpty()) {
        scene->setSceneRect(scene->itemsBoundingRect());
        view->centerOn(scene->itemsBoundingRect().center());
    }
    
    ai->explainStepAsync(step);
}

void QtVisuals::onCleared() {
    aggressiveClear(scene); 
    aiLabel->setText("AI explanation will appear here.");
}


// --- START: HELPER FUNCTIONS (drawNode, drawConnector, drawArrayBox) ---
// These were the missing functions from your drawHashTable implementation


QGraphicsItem* QtVisuals::drawNode(const QString& text, int x, int y, const QColor& color, int radius = 20) {
    QGraphicsEllipseItem* node = scene->addEllipse(
        x - radius, y - radius,
        radius * 2, radius * 2,
        QPen(Qt::white, 2), QBrush(color)
    );

    QGraphicsTextItem* label = scene->addText(text);
    label->setDefaultTextColor(Qt::white);
    label->setFont(QFont("Arial", 10, QFont::Bold));
    // Center the text
    label->setPos(
        x - label->boundingRect().width() / 2, 
        y - label->boundingRect().height() / 2
    );
    return node;
}


void QtVisuals::drawConnector(QGraphicsItem* startItem, QGraphicsItem* endItem, const QColor& color) {
    QPointF start = startItem->boundingRect().center() + startItem->pos();
    QPointF end = endItem->boundingRect().center() + endItem->pos();
    
    // Shorten the line so it doesn't go inside the nodes
    qreal dx = end.x() - start.x();
    qreal dy = end.y() - start.y();
    qreal length = std::sqrt(dx*dx + dy*dy);
    if (length == 0) return;
    
    qreal offset = 22; // approx radius
    QPointF startOffset(start.x() + (dx * offset / length), start.y() + (dy * offset / length));
    QPointF endOffset(end.x() - (dx * offset / length), end.y() - (dy * offset / length));

    drawConnector(startOffset, endOffset, color); // Call the point-based overload
}


void QtVisuals::drawConnector(QPointF start, QPointF end, const QColor& color) {
    QPen pen(color, 2);
    scene->addLine(start.x(), start.y(), end.x(), end.y(), pen);
    
    // Add arrowhead
    double angle = std::atan2(-(end.y() - start.y()), end.x() - start.x());
    QPointF arrowP1 = end + QPointF(sin(angle - M_PI / 3) * 10, cos(angle - M_PI / 3) * 10);
    QPointF arrowP2 = end + QPointF(sin(angle - M_PI + M_PI / 3) * 10, cos(angle - M_PI + M_PI / 3) * 10);
    
    QPolygonF arrowhead;
    arrowhead << end << arrowP1 << arrowP2;
    scene->addPolygon(arrowhead, pen, QBrush(color));
}


QGraphicsItem* QtVisuals::drawArrayBox(const QString& text, int x, int y, int width, int height, const QColor& color) {
    QGraphicsRectItem* rect = scene->addRect(x, y, width, height, QPen(Qt::white), QBrush(color));
    
    QGraphicsTextItem* label = scene->addText(text);
    label->setDefaultTextColor(Qt::white);
    label->setFont(QFont("Arial", 10, QFont::Bold));
    label->setPos(
        x + (width - label->boundingRect().width()) / 2, 
        y + (height - label->boundingRect().height()) / 2
    );
    return rect;
}

// --- END: HELPER FUNCTIONS ---



// --- 1. drawLinearArray (For DynamicArray, Stack, Queue) ---
// This is the stable, uniform box function.

void QtVisuals::drawLinearArray(const DryRunStep& step) {
    const int BAR_WIDTH = 50;           
    const int BAR_HEIGHT = 50;          
    const int FONT_SIZE = 12;
    const int LABEL_Y_OFFSET = 10;      
    const int ITEM_SPACING = 60;        
    const int BASELINE_Y = 250;         

    int x = 0; 

    for (size_t i = 0; i < step.values.size(); ++i) {
        int value = step.values[i];
        
        // Use the new helper function
        QColor color = Qt::blue;
        if (step.dataStructure == "Queue" && i == 0) {
            color = QColor("#009688"); // Teal for Queue Front
        } else if (step.dataStructure == "Stack" && i == step.values.size() - 1) {
            color = QColor("#FF5722"); // Deep Orange for Stack Top
        }
        
        drawArrayBox(
            QString::number(value), 
            x, 
            BASELINE_Y - BAR_HEIGHT / 2, 
            BAR_WIDTH, 
            BAR_HEIGHT, 
            color
        );

        x += ITEM_SPACING; 
    }
}

// --- 2. drawLinkedList (Nodes and Pointers) ---

void QtVisuals::drawLinkedList(const DryRunStep& step) {
    const int NODE_WIDTH = 80;      
    const int NODE_HEIGHT = 50;
    const int POINTER_AREA_WIDTH = 20; 
    const int VALUE_AREA_WIDTH = NODE_WIDTH - POINTER_AREA_WIDTH;
    const int HORIZONTAL_GAP = 70;  
    const int BASELINE_Y = 250; 
    const int FONT_SIZE = 12;

    int x = 0; // Horizontal start position
    QGraphicsItem* lastItem = nullptr;

    for (size_t i = 0; i < step.values.size(); ++i) {
        int value = step.values[i];
        QPointF nodePos(x, BASELINE_Y - NODE_HEIGHT / 2); 
        
        // 1. Draw the Node Box (Value area + Pointer area)
        QGraphicsRectItem* nodeRect = new QGraphicsRectItem(0, 0, NODE_WIDTH, NODE_HEIGHT);
        nodeRect->setPen(QPen(Qt::white));
        nodeRect->setBrush(QBrush(QColor("#9C27B0"))); // Purple for Linked List
        nodeRect->setPos(nodePos); 
        scene->addItem(nodeRect);
        
        // 2. Draw the Separator Line for the Pointer
        QGraphicsLineItem* separator = scene->addLine(
            nodePos.x() + VALUE_AREA_WIDTH, 
            nodePos.y(), 
            nodePos.x() + VALUE_AREA_WIDTH, 
            nodePos.y() + NODE_HEIGHT,
            QPen(Qt::white, 1)
        );

        // 3. Draw the Value Label
        QGraphicsTextItem* label = new QGraphicsTextItem(QString::number(value)); 
        label->setFont(QFont("Arial", FONT_SIZE));
        label->setDefaultTextColor(Qt::white);
        qreal labelX = nodePos.x() + (VALUE_AREA_WIDTH / 2) - (label->boundingRect().width() / 2);
        qreal labelY = nodePos.y() + (NODE_HEIGHT / 2) - (label->boundingRect().height() / 2); 
        label->setPos(labelX, labelY);
        scene->addItem(label);

        // 4. Draw the Pointer (Arrow)
        if (lastItem) {
            // Draw connector from last item's pointer area to this item
            QPointF start(lastItem->pos().x() + VALUE_AREA_WIDTH + (POINTER_AREA_WIDTH / 2), BASELINE_Y);
            QPointF end(nodePos.x(), BASELINE_Y);
            drawConnector(start, end, Qt::green);
        }
        
        if (i == step.values.size() - 1) {
             // Draw a "NULL" text label below the pointer area
             QGraphicsTextItem* nullLabel = new QGraphicsTextItem("NULL");
             nullLabel->setDefaultTextColor(Qt::red);
             nullLabel->setPos(x + VALUE_AREA_WIDTH + 5, BASELINE_Y + 10);
             scene->addItem(nullLabel);
        }

        lastItem = nodeRect;
        x += NODE_WIDTH + HORIZONTAL_GAP; 
    }
}

// --- 3. drawGraph ---

void QtVisuals::drawGraph(const DryRunStep& step) {
    const int NODE_RADIUS = 25;
    const int CENTER_X = 400;
    const int CENTER_Y = 250;
    const int RADIUS = 150;

    // This is a very simple graph drawing algorithm.
    // It assumes the DryRunStep::values contains the ADJACENCY LIST
    // encoded as: [num_nodes, node1, node2, ..., nodeN, num_edges, n1_src, n1_dest, n2_src, n2_dest, ...]
    
    // For your test case (10, 20, 30) and edges (10,20), (20,30)
    // Your Graph.cpp's data() function must return:
    // [3, 10, 20, 30, 2, 10, 20, 20, 30]
    
    if (step.values.empty()) return;
    
    int num_nodes = step.values[0];
    if (num_nodes == 0 || step.values.size() < 1 + num_nodes) return;
    
    std::map<int, QPointF> positions;
    std::map<int, QGraphicsItem*> nodeItems;

    // 1. Draw nodes in a circle
    for (int i = 0; i < num_nodes; ++i) {
        int value = step.values[i + 1];
        double angle = (2 * M_PI * i) / num_nodes;
        double x = CENTER_X + RADIUS * cos(angle);
        double y = CENTER_Y + RADIUS * sin(angle);
        positions[value] = QPointF(x, y);
        
        // Use the drawNode helper
        nodeItems[value] = drawNode(QString::number(value), x, y, Qt::blue, NODE_RADIUS);
    }

    // 2. Draw edges
    if (step.values.size() > 1 + num_nodes) {
        int num_edges = step.values[1 + num_nodes];
        int edge_index = 2 + num_nodes;
        
        for (int i = 0; i < num_edges; ++i) {
            if (edge_index + 1 >= step.values.size()) break;
            int src_val = step.values[edge_index];
            int dest_val = step.values[edge_index + 1];
            
            if (nodeItems.count(src_val) && nodeItems.count(dest_val)) {
                drawConnector(nodeItems[src_val], nodeItems[dest_val], Qt::green);
            }
            edge_index += 2;
        }
    }
}

// --- 4. drawBST ---

void QtVisuals::drawBST(const DryRunStep& step) {
    const int NODE_RADIUS = 25;
    const int BASE_Y = 100;           
    const int VERTICAL_SPACING = 80; 
    const int HORIZONTAL_SCALE = 60; 
    const int CENTER_OFFSET = 400;  

    struct DrawNode {
        int value;
        QPointF center;
        QGraphicsItem* item;
    };
    std::map<int, DrawNode> drawnNodes; // Map Value -> DrawNode

    size_t numNodes = step.values.size() / 4; 
    if (numNodes == 0) return;

    // --- PASS 1: Calculate Positions, Draw Nodes, and Record Coordinates ---
    for (size_t i = 0; i < step.values.size(); i += 4) {
        if (i + 3 >= step.values.size()) break;

        int value = step.values[i];
        int depth = step.values[i+1]; 
        int index = step.values[i+2]; 
        int parentValue = step.values[i+3];
        
        int y = BASE_Y + depth * VERTICAL_SPACING;
        int x = CENTER_OFFSET + (index - (int)(numNodes / 2.0)) * HORIZONTAL_SCALE;
        QPointF center(x, y);
        
        // Use the drawNode helper
        QGraphicsItem* nodeItem = drawNode(QString::number(value), x, y, Qt::yellow, NODE_RADIUS);
        
        // Fix label color for yellow background
        for (auto* child : nodeItem->childItems()) {
            if (QGraphicsTextItem* text = dynamic_cast<QGraphicsTextItem*>(child)) {
                text->setDefaultTextColor(Qt::black);
            }
        }
        
        drawnNodes[value] = {value, center, nodeItem};
    }

    // --- PASS 2: Draw Edges using ParentValue Lookup ---
    for (size_t i = 0; i < step.values.size(); i += 4) {
        if (i + 3 >= step.values.size()) break;
        int value = step.values[i];
        int parentValue = step.values[i+3];
        
        if (parentValue == -1) continue; // Root

        if (drawnNodes.count(value) && drawnNodes.count(parentValue)) {
            drawConnector(drawnNodes[parentValue].item, drawnNodes[value].item, Qt::green);
        }
    }
}

// --- 5. drawHashTable (FOR ANVI) ---
/**
 * @brief Draws the Hash Table visualization.
 * Assumes step.values is encoded as:
 * [num_buckets, 
 * val1_in_bucket_0, val2_in_bucket_0, ..., -1, (separator)
 * val1_in_bucket_1, val2_in_bucket_1, ..., -1, (separator)
 * ...
 * val1_in_bucket_N, ..., -1]
 */
void QtVisuals::drawHashTable(const DryRunStep& step) {
    const int BUCKET_WIDTH = 60;
    const int BUCKET_HEIGHT = 40;
    const int NODE_RADIUS = 20;
    const int H_SPACING = 70; // Horizontal space between buckets
    const int V_SPACING = 50; // Vertical space between chained nodes
    const int START_X = 50;
    const int START_Y = 50;

    if (step.values.empty()) return;

    int num_buckets = step.values[0];
    if (num_buckets == 0) return;

    std::vector<std::list<int>> table(num_buckets);
    int current_bucket = 0;
    
    // Decode the step.values vector back into a table structure
    for (size_t i = 1; i < step.values.size(); ++i) {
        if (current_bucket >= num_buckets) break;
        
        if (step.values[i] == -1) {
            current_bucket++;
        } else {
            table[current_bucket].push_back(step.values[i]);
        }
    }
    
    // Now, draw the table we just decoded
    for (int i = 0; i < num_buckets; ++i) {
        int x = START_X + i * H_SPACING;
        
        // 1. Draw the bucket array
        QGraphicsItem* bucketItem = drawArrayBox(
            QString::number(i), 
            x, 
            START_Y, 
            BUCKET_WIDTH, 
            BUCKET_HEIGHT, 
            QColor("#607D8B") // Gray
        );
        
        // 2. Draw the chain for this bucket
        int node_y = START_Y + BUCKET_HEIGHT + V_SPACING;
        QGraphicsItem* lastItem = bucketItem;
        bool firstNode = true;

        for (int value : table[i]) {
            QGraphicsItem* node = drawNode(
                QString::number(value), 
                x + (BUCKET_WIDTH / 2), // Center under the bucket
                node_y, 
                QColor("#E91E63"), // Pink
                NODE_RADIUS
            );
            
            // Draw connector line
            if (firstNode) {
                // Draw line from bucket to first node
                QPointF bucketBottom(x + BUCKET_WIDTH / 2, START_Y + BUCKET_HEIGHT);
                QPointF nodeTop(x + (BUCKET_WIDTH / 2), node_y - NODE_RADIUS);
                drawConnector(bucketBottom, nodeTop, QColor("#E91E63"));
                firstNode = false;
            } else {
                drawConnector(lastItem, node, QColor("#E91E63"));
            }
            
            lastItem = node;
            node_y += V_SPACING;
        }
    }
}


// --- AI Service Slot ---

void QtVisuals::showAI(const QString& txt) {
    aiLabel->setText("AI: " + txt);
}

