QT += core gui widgets network
CONFIG += c++17 console

SOURCES += \
    main.cpp \
    EngineCore/VisualizerEngine.cpp \
    AI_Service/AI_Service.cpp \
    DataStructures/DynamicArray.cpp \
    DataStructures/LinkedList.cpp \
    DataStructures/Stack.cpp \
    DataStructures/Queue.cpp \
    DataStructures/BST.cpp \
    DataStructures/Graph.cpp \
    DataStructures/HashTable.cpp \   
    QtUI/QtVisuals.cpp \
    QtUI/MainWindow.cpp

HEADERS += \
    EngineCore/DryRunStep.h \
    EngineCore/VisualizerEngine.h \
    AI_Service/AI_Service.h \
    DataStructures/DynamicArray.h \
    DataStructures/LinkedList.h \
    DataStructures/Stack.h \
    DataStructures/Queue.h \
    DataStructures/BST.h \
    DataStructures/Graph.h \
    DataStructures/HashTable.h \    
    QtUI/QtVisuals.h \
    QtUI/MainWindow.h