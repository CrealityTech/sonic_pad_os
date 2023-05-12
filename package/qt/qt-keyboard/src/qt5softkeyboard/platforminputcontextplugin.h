#ifndef PLATFORMINPUTCONTEXTPLUGIN_H
#define PLATFORMINPUTCONTEXTPLUGIN_H

#include <QtGui/qpa/qplatforminputcontextplugin_p.h>

QT_BEGIN_NAMESPACE

class  PlatformInputContextPlugin: public QPlatformInputContextPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID QPlatformInputContextFactoryInterface_iid FILE "Qt5Input.json")

public:
    QPlatformInputContext *create(const QString& key, const QStringList &paramList);
};

QT_END_NAMESPACE
#endif // PLATFORMINPUTCONTEXTPLUGIN_H
