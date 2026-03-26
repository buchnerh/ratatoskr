/*==========================================================
 * Program : adapter.h                   Project : ratatoskr
 * Author  : Michael Zanetti, Ian L., Philippe Andersson
 * Date    : 2025-12-18
 * Version : 0.0.1
 * Notice  : (c) Original work by Michael Zanetti, Canonical
 *           Adapted by Ian L. and Philippe Andersson
 * License : GNU GPL v3 or later
 * Comment : BT Adapter class
 *           Based on 'ubtd' by Michael Zanetti and Ian L.
 *           Revived with the help of the Copilot CLI coding agent.
 * Modification History:
 * - 2025-12-18 (0.0.1) : Adapted from ubtd-20.04.
 *========================================================*/

#ifndef ADAPTER_H
#define ADAPTER_H

#include <QObject>
#include <QDBusConnection>
#include <QDBusInterface>

class Adapter : public QObject
{
    Q_OBJECT
public:
    explicit Adapter(QObject *parent = 0);

signals:

public slots:

private:
    void init();

private:
    QDBusInterface m_dbus;
};

#endif // ADAPTER_H
