#include "MainWindow.h"
#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QIntValidator>
#include <QInputDialog> // <-- ADDED for HashTable buckets

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
{
    setWindowTitle("FODS — Data Structure Visualizer (AI-backed)");
    resize(980, 820);

    QString key = QStringLiteral("AIzaSyDEOSAq7wsdKp0_i__fKNkT3CVO8desQ9s");
    ai = new AI_Service(key, "gemini-2.0-flash", this);
    engine = new VisualizerEngine();

    // --- Create all data structure objects ---
    dynamicArray = new DynamicArray(engine);
    queue = new Queue(engine);
    stack = new Stack(engine);
    linkedList = new LinkedList(engine);
    graph = new Graph(engine);
    bst = new BST(engine);
    // Initialize with 5 buckets as per your test case.
    // onClearClicked will also reset to 5 buckets.
    hashTable = new HashTable(5, engine); 
    // --- END ADD ---

    visuals = new QtVisuals(engine, ai, this);

    QWidget* central = new QWidget(this);
    setCentralWidget(central);
    QVBoxLayout* mainLayout = new QVBoxLayout(central);

    QHBoxLayout* topBar = new QHBoxLayout();
    QLabel* lbl = new QLabel("Structure:");
    structureBox = new QComboBox();
    
    // --- ADDED FOR ANVI ---
    structureBox->addItems({"DynamicArray","LinkedList","Stack","Queue","BST","Graph","HashTable"}); // <-- Added "HashTable"
    // --- END ADD ---

    connect(structureBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MainWindow::onStructureChanged);

    valueEdit = new QLineEdit();
    valueEdit->setPlaceholderText("Value (integer)");
    valueEdit->setValidator(new QIntValidator(INT_MIN, INT_MAX, this));

    addBtn = new QPushButton("Add");
    removeBtn = new QPushButton("Remove");
    clearBtn = new QPushButton("Clear Steps");

    topBar->addWidget(lbl);
    topBar->addWidget(structureBox);
    topBar->addWidget(valueEdit);
    topBar->addWidget(addBtn);
    topBar->addWidget(removeBtn);
    topBar->addWidget(clearBtn);

    mainLayout->addLayout(topBar);
    mainLayout->addWidget(visuals);

    connect(addBtn, &QPushButton::clicked, this, &MainWindow::onAddClicked);
    connect(removeBtn, &QPushButton::clicked, this, &MainWindow::onRemoveClicked);
    connect(clearBtn, &QPushButton::clicked, this, &MainWindow::onClearClicked);

    onStructureChanged(structureBox->currentIndex()); // Set initial placeholder text
}

// --- UPDATED to handle placeholder text ---
void MainWindow::onStructureChanged(int idx) { 
    Q_UNUSED(idx);
    QString s = structureBox->currentText();
    
    if (s == "DynamicArray" || s == "LinkedList") {
        valueEdit->setPlaceholderText("Value (or Index,Value for InsertAt)");
    } else if (s == "Graph") {
        valueEdit->setPlaceholderText("Node (e.g., 10) or Edge (e.g., 10,20)");
    } else { // Stack, Queue, BST, HashTable
        valueEdit->setPlaceholderText("Value (integer)");
    }
}

// --- RESTRUCTURED for clarity and to add HashTable ---
void MainWindow::onAddClicked() {
    QString s = structureBox->currentText();
    QString val = valueEdit->text();
    if (val.isEmpty()) return;

    // Logic for structures that use complex (comma-separated) input
    if (s == "DynamicArray" || s == "LinkedList" || s == "Graph") {
        QStringList parts = val.split(',', Qt::SkipEmptyParts);
        bool ok_value, ok_index = false;
        int index = -1;
        int value;

        if (s == "Graph") {
            if (parts.size() == 1) { // Add Node
                int node = parts[0].trimmed().toInt(&ok_value);
                if (ok_value) graph->addNode(node);
            } else if (parts.size() == 2) { // Add Edge
                int from = parts[0].trimmed().toInt(&ok_index);
                int to = parts[1].trimmed().toInt(&ok_value);
                if (ok_index && ok_value) graph->addEdge(from, to, false);
            }
        } else { // DynamicArray or LinkedList
            if (parts.size() == 1) {
                value = parts.at(0).trimmed().toInt(&ok_value);
            } else if (parts.size() == 2) {
                index = parts.at(0).trimmed().toInt(&ok_index);
                value = parts.at(1).trimmed().toInt(&ok_value);
                if (!ok_index) ok_value = false;
            } else {
                return; // Invalid
            }
            if (!ok_value) return;

            if (s == "DynamicArray") {
                // Your original code only supported pushBack, so we'll honor that.
                dynamicArray->pushBack(value);
            } else if (s == "LinkedList") {
                if (index != -1) linkedList->insertAt(index, value);
                else linkedList->insertBack(value);
            }
        }
    }
    // Logic for structures that use simple (single value) input
    else { 
        bool ok;
        int value = val.toInt(&ok);
        if (!ok) return;

        if (s == "Queue") {
            queue->enqueue(value);
        } else if (s == "Stack") {
            stack->push(value);
        } else if (s == "BST") {
            bst->insert(value);
        } 
        else if (s == "HashTable") {
            hashTable->insert(value);
        }
        // --- END ADD ---
    }
    
    valueEdit->clear();
}

// --- RESTRUCTURED to fix bugs and add HashTable/BST ---
void MainWindow::onRemoveClicked() {
    QString s = structureBox->currentText();
    QString val = valueEdit->text();

    if (s == "Queue") {
        queue->dequeue();
    } 
    else if (s == "Stack") { 
        stack->pop();
    } 
    else if (s == "DynamicArray") {
        if (val.isEmpty()) { // Default: pop_back
            if (!dynamicArray->data().empty()) {
                dynamicArray->removeAt(dynamicArray->data().size() - 1);
            }
        } else { // Remove at index
            bool ok;
            int index = val.toInt(&ok);
            if (ok) dynamicArray->removeAt(index);
        }
    } 
    else if (s == "LinkedList") {
        if (val.isEmpty()) { // Default: removeBack
            linkedList->removeBack();
        } else { // Remove at index
            bool ok;
            int index = val.toInt(&ok);
            if (ok) linkedList->removeAt(index);
        }
    }
    else if (s == "Graph") {
        if (val.isEmpty()) return; // Must specify a node
        bool ok;
        int node = val.toInt(&ok);
        if (ok) graph->removeNode(node);
    }
    else if (s == "BST") {
        if (val.isEmpty()) return; // Must specify a value
        bool ok;
        int value = val.toInt(&ok);
        if (ok) bst->remove(value); // Assuming bst->remove(int) exists
    }
    else if (s == "HashTable") {
        if (val.isEmpty()) return; // Must specify a value
        bool ok;
        int value = val.toInt(&ok);
        if (ok) hashTable->remove(value);
    }
    // --- END ADD ---
    
    valueEdit->clear();
}
 
void MainWindow::onClearClicked() {
    engine->clearSteps();

    QString s = structureBox->currentText();

    if (s == "DynamicArray") {
        delete dynamicArray;
        dynamicArray = new DynamicArray(engine);
    } 
    else if (s == "Queue") {
        delete queue;
        queue = new Queue(engine);
    } 
    else if (s == "Stack") {
        delete stack;
        stack = new Stack(engine);
    } 
    else if (s == "LinkedList") {
        delete linkedList;
        linkedList = new LinkedList(engine);
    }
    else if (s == "Graph") {
        delete graph;
        graph = new Graph(engine);
    }
    else if (s == "BST") {
        delete bst;
        bst = new BST(engine);
    }
    else if (s == "HashTable") {
        delete hashTable;
        // Re-create with 5 buckets for the test case.
        // For a real app, you might ask the user again.
        hashTable = new HashTable(5, engine);
    }
    // --- END ADD ---

    visuals->refreshAll(); // Redraw the (now empty) scene
}