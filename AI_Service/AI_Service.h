#pragma once
#include <QObject>
#include <QString>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include "../EngineCore/DryRunStep.h"

// AI_Service asynchronously calls Google Generative Language (Gemini) API
// Emits `explanationReady` when response is available.
class AI_Service : public QObject {
    Q_OBJECT
public:
    AI_Service(const QString& apiKey, const QString& model = "gemini-2.0-flash", QObject* parent = nullptr);
    ~AI_Service();

    void explainStepAsync(const DryRunStep& step); // async call

signals:
    void explanationReady(const QString& text);

private:
    QString apiKey;
    QString modelName;
    QNetworkAccessManager* netManager;

    void postPromptAsync(const QString& prompt); // helper
};
