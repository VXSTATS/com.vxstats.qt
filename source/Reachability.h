/*
 * Copyright (C) 01/10/2020 VX APPS <sales@vxapps.com>
 *
 * The ownership of this document rests with the VX APPS. It is
 * strictly prohibited to change, sell or publish it in any way. In case
 * you have access to this document, you are obligated to ensure its
 * nondisclosure. Noncompliances will be prosecuted.
 *
 * Diese Datei ist Eigentum der VX APPS. Ändern, verkaufen oder
 * auf eine andere Weise verbreiten und öffentlich machen ist strikt
 * untersagt. Falls Sie Zugang zu dieser Datei haben, sind Sie
 * verpflichtet alles Mögliche für deren Geheimhaltung zu tun.
 * Zuwiderhandlungen werden strafrechtlich verfolgt.
 */

#pragma once

/* qt header */
#include <QObject>

/* local header */
#include "Device.h"

/* qt class */
class QNetworkConfigurationManager;

/**
 * @~english
 * @brief The VX namespace.
 *
 * @~german
 * @brief Der VX Namensbereich.
 */
namespace VX {
  /**
   * @~english
   * @brief The Reachability class.
   * Send changes of current network state.
   *
   * @~german
   * @brief Die Reachability Klasse.
   * Liefert informationen über den geänderten Netzwerkstatus.
   */
  class Reachability : public QObject {

    Q_OBJECT

  public:
    /**
     * @~english
     * @brief Default constructor for Reachability.
     * @param _parent   Parent object.
     *
     * @~german
     * @brief Standardkonstruktor für Reachability.
     * @param _parent   Übergeordnetes Objekt.
     */
    Reachability( QObject *_parent = nullptr );

  public Q_SLOTS:
    /**
     * @~english
     * @brief Changes on Reachability in any case. Online/Offline, Status, Radio.
     *
     * @~german
     * @brief Änderungen im Netzwerk zu jedem Status. Online/Offline, Status,
     * Funk.
     */
    void slotUpdateReachability();

  Q_SIGNALS:
    /**
     * @~english
     * @brief Signal is emitted, when the reachability is changed.
     * @param p_connection   Changed connection.
     * @param p_radio   Changed radio.
     *
     * @~german
     * @brief Signal wird gefeuert, wenn sich die Verbindung geändert hat.
     * @param p_connection   Geänderte Verbindungsart.
     * @param p_radio   Geänderte Funkverbindung.
     */
    void reachabilityChanged( Device::Connection p_connection,
                              Device::Radio p_radio );

  private:
    /**
     * @~english
     * @brief Network configuration manager to bear the correct network type.
     *
     * @~german
     * @brief Manager des konfigurierten Netzwerkes um informationen zu dem
     * aktuellen Typ zu erhalten.
     */
    QNetworkConfigurationManager *m_networkConfigurationManager = nullptr;
  };
}
