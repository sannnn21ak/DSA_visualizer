#pragma once
#include <vector>
#include <functional>
#include <QObject>
#include "DryRunStep.h"

// VisualizerEngine is NOT a Qt QObject in most files to keep things simple,
// but in this project we'll keep it a regular C++ class and call callbacks.
class VisualizerEngine {
public:
    VisualizerEngine() = default;

    void addStep(const DryRunStep& step) {
        steps.push_back(step);
        if (onNewStep) onNewStep(step);
    }

    const std::vector<DryRunStep>& getSteps() const { return steps; }
    void clearSteps() { steps.clear(); if(onCleared) onCleared(); }

    // Callbacks the UI can set
    std::function<void(const DryRunStep&)> onNewStep = nullptr;
    std::function<void()> onCleared = nullptr;

private:
    std::vector<DryRunStep> steps;
};
