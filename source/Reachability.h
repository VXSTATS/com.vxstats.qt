/*
 * Copyright (C) 01/10/2020 VX APPS <sales@vxapps.com>
 *
 * This document is property of VX APPS. It is
 * strictly prohibited to modify, sell or publish it in any way. In case
 * you have access to this document, you are obligated to ensure its
 * nondisclosure. Noncompliances will be prosecuted.
 *
 * Diese Datei ist Eigentum der VX APPS. Jegliche Änderung, Verkauf oder
 * andere Verbreitung und Veröffentlichung ist strikt
 * untersagt. Falls Sie Zugang zu dieser Datei haben, sind Sie
 * verpflichtet, alles in Ihrer Macht stehende für deren Geheimhaltung zu tun.
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
 * @brief Der VX Namensraum.
 */
namespace VX {
  /**
   * @~english
   * @brief The Reachability class.
   * Sends changes of current network state.
   *
   * @~german
   * @brief Die Klasse Reachability.
   * Liefert informationen über den aktuellen Netzwerkstatus.
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
     * @brief Any changes on Reachability status. Online/Offline, Status, Radio.
     *
     * @~german
     * @brief Änderungen im Netzwerk zu jedem Status. Online/Offline, Status,
     * Funk.
     */
    void slotUpdateReachability();

  Q_SIGNALS:
    /**
     * @~english
     * @brief Signal is emitted, if the reachability is changed.
     * @param p_connection   Changed connection.
     * @param p_radio   Changed radio.
     *
     * @~german
     * @brief Signal wird gesendet, wenn sich die Verbindung geändert hat.
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
     * @brief Manager des konfigurierten Netzwerkes um Informationen zu dem
     * aktuellen Typ zu erhalten.
     */
    QNetworkConfigurationManager *m_networkConfigurationManager = nullptr;
  };
}
