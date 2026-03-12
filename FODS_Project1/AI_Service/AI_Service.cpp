#include "AI_Service.h"
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonArray>
#include <QJsonValue>
#include <QUrl>
#include <QDebug>

AI_Service::AI_Service(const QString& apiKey_, const QString& model, QObject* parent)
    : QObject(parent), apiKey(apiKey_), modelName(model)
{
    // QNetworkAccessManager is parented to this QObject, so will auto-delete
    netManager = new QNetworkAccessManager(this);
}

AI_Service::~AI_Service() {
    // Nothing to manually delete
}

void AI_Service::explainStepAsync(const DryRunStep& step) {
    QString prompt = QString(
        "You are a helpful computer science tutor. Explain clearly and briefly the following data structure operation.\n"
        "Operation: %1\n"
        "Data structure: %2\n"
        "Details: %3\n\n"
        "Give a 2-3 sentence explanation and one short note about complexity if relevant."
    ).arg(QString::fromStdString(step.operation),
          QString::fromStdString(step.dataStructure),
          QString::fromStdString(step.details));

    postPromptAsync(prompt);
}

void AI_Service::postPromptAsync(const QString& prompt) {
    QString endpoint = QString("https://generativelanguage.googleapis.com/v1beta/models/%1:generateContent?key=%2")
                           .arg(modelName, apiKey);

    QNetworkRequest request{ QUrl(endpoint) };
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json; charset=utf-8");

    // JSON body according to new API spec
    QJsonObject textPart;
    textPart["text"] = prompt;

    QJsonArray parts;
    parts.append(textPart);

    QJsonObject contentObj;
    contentObj["parts"] = parts;

    QJsonArray contents;
    contents.append(contentObj);

    QJsonObject payload;
    payload["contents"] = contents;

    QJsonDocument doc(payload);
    QNetworkReply* reply = netManager->post(request, doc.toJson());

    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        QString parsed;

        if (reply->error() != QNetworkReply::NoError) {
            parsed = "AI Error: " + reply->errorString();
            reply->deleteLater();
            emit explanationReady(parsed);
            return;
        }

        QByteArray resp = reply->readAll();
        reply->deleteLater();

        QJsonParseError parseError;
        QJsonDocument respDoc = QJsonDocument::fromJson(resp, &parseError);
        if (parseError.error != QJsonParseError::NoError) {
            emit explanationReady("AI Error: invalid JSON response");
            return;
        }

        QJsonObject root = respDoc.object();
        QString text;

        if (root.contains("candidates")) {
            QJsonArray cand = root["candidates"].toArray();
            if (!cand.isEmpty()) {
                QJsonObject c0 = cand[0].toObject();
                if (c0.contains("content")) {
                    QJsonObject content = c0["content"].toObject();
                    if (content.contains("parts")) {
                        QJsonArray p = content["parts"].toArray();
                        if (!p.isEmpty() && p[0].toObject().contains("text"))
                            text = p[0].toObject()["text"].toString();
                    }
                }
            }
        }

        if (text.isEmpty()) text = QString::fromUtf8(resp);
        emit explanationReady(text);
    });
}

