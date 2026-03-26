/*==========================================================
 * Program : adapter.cpp                 Project : ratatoskr
 * Author  : Michael Zanetti, Ian L., Philippe Andersson
 * Date    : 2025-12-18
 * Version : 0.0.1
 * Notice  : (c) Original work by Michael Zanetti, Canonical
 *           Adapted by Ian L. and Philippe Andersson
 * License : GNU GPL v3 or later
 * Comment : BlueZ adapter management.
 *           Based on 'ubtd' by Michael Zanetti and Ian L.
 *           Revived with the help of the Copilot CLI coding agent.
 * Modification History:
 * - 2025-12-18 (0.0.1) : Adapted from ubtd-20.04.
 *========================================================*/

#include "adapter.h"

#include <QDebug>
#include <QDBusReply>

Adapter::Adapter(QObject *parent) :
    QObject(parent),
    m_dbus("org.bluez", "/", "org.bluez.Manager", QDBusConnection::systemBus(), this)
{
    init();
}

void Adapter::init()
{

//    auto objectList = m_dbus.GetManagedObjects().argumentAt<0>();
//    for (QDBusObjectPath path : objectList.keys()) {
//    }


//    QDBusMessage reply = m_dbus.call("ListAdapters");
//    QDBusArgument arg = reply.arguments().first().value<QDBusArgument>();

//    // We only bother with the first adapter
//    arg.beginArray();
//    QDBusObjectPath path;
//    arg >> path;
//    arg.endArray();

//    qDebug() << "have adapter" << path.path();

    QDBusInterface iface("org.bluez", "/org/bluez/hci0", "org.freedesktop.DBus.Properties", QDBusConnection::systemBus(), this);
//    QDBusMessage reply = iface.call("SetProperty", "Discoverable", QVariant::fromValue(QDBusVariant(true)));
    QDBusMessage reply = iface.call("Set", "org.bluez.Adapter1", "Discoverable", QVariant::fromValue(QDBusVariant(true)));
    qDebug() << "reply" << reply;

}
