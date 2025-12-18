/*==========================================================
 * Program : obexd.h              Project : ratatoskr
 * Author  : Michael Zanetti, Ian L., Philippe Andersson
 * Date    : 2025-12-18
 * Version : 0.0.1
 * Notice  : (c) Original work by Michael Zanetti, Canonical
 *           Adapted by Ian L. and Philippe Andersson
 * License : GNU GPL v3 or later
 * Comment : OBEX daemon D-Bus interface header
 * Modification History:
 * - 2025-12-18 (0.0.1) : Adapted from ubtd-20.04.
 *========================================================*/

#ifndef OBEXD_H
#define OBEXD_H

#include <QAbstractListModel>
#include <QDBusConnection>
#include <QDBusInterface>

#include "transfer.h"

class ObexAgent;

class Obexd : public QAbstractListModel
{
    Q_OBJECT
public:
    enum Roles {
        RoleFilename,
        RoleFilePath,
        RoleSize,
        RoleTransferred,
        RoleStatus,
        RoleDate
    };

    explicit Obexd(QObject *parent = 0);

    int rowCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QHash<int, QByteArray> roleNames() const;

public slots:
    void deleteFile(int index);

private slots:
    void newTransfer(const QString &path);

    void transferProgress();
    void transferStatusChanged();

private:
    ObexAgent *m_agent;
    QDBusConnection m_dbus;
    QDBusInterface m_manager;

    QList<Transfer*> m_transfers;
};

#endif // OBEXD_H
