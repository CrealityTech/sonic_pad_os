#include "QKeyboardLayout.h"
#include <QJsonDocument>
#include <QFile>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>

QKeyboardLayout::QKeyboardLayout(QObject *parent) : QObject(parent)
{

}

bool QKeyboardLayout::LoadLayout(const QString &strJsonPath)
{
    keyboardRows.clear();

    QFile jsonFile(strJsonPath);
    if(!jsonFile.open(QIODevice::ReadOnly))
        return false;
    QJsonParseError error;
    QJsonDocument jsonDocument = QJsonDocument::fromJson(jsonFile.readAll(),&error);
    if (error.error == QJsonParseError::NoError && jsonDocument.isObject())
    {
        {
            QJsonObject obj = jsonDocument.object();
            if(obj.contains("KeyboardRow"))
            {
                QJsonArray rows = obj["KeyboardRow"].toArray();
                for(int i = 0;i < rows.size(); ++i)
                    keyboardRows.append(rows.at(i));
            }
        }

    }

    return false;
}
