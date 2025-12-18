/*==========================================================
 * Program : main.cpp              Project : ratatoskr
 * Author  : Michael Zanetti, Ian L., Philippe Andersson
 * Date    : 2025-12-18
 * Version : 0.0.1
 * Notice  : (c) Original work by Michael Zanetti, Canonical
 *           Adapted by Ian L. and Philippe Andersson
 * License : GNU GPL v3 or later
 * Comment : ContentHub shareplugin entry point
 * Modification History:
 * - 2025-12-18 (0.0.1) : Adapted from ubtd-20.04.
 *========================================================*/

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickView>
#include <QQmlContext>

#include "adapter.h"
#include "bttransfer.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    qmlRegisterType<BtTransfer>("Shareplugin", 0, 1, "BtTransfer");

    QQuickView view;
    QObject::connect(view.engine(), &QQmlEngine::quit, &app, &QCoreApplication::quit);

    view.setSource(QUrl(QStringLiteral("qrc:///Main.qml")));
    view.setResizeMode(QQuickView::SizeRootObjectToView);
    view.show();
    return app.exec();
}

