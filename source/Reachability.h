/*
 * Copyright (C) 10/01/2020 VX STATS <sales@vxstats.com>
 *
 * This document is property of VX STATS. It is strictly prohibited
 * to modify, sell or publish it in any way. In case you have access
 * to this document, you are obligated to ensure its nondisclosure.
 * Noncompliances will be prosecuted.
 *
 * Diese Datei ist Eigentum der VX STATS. Jegliche Änderung, Verkauf
 * oder andere Verbreitung und Veröffentlichung ist strikt untersagt.
 * Falls Sie Zugang zu dieser Datei haben, sind Sie verpflichtet,
 * alles in Ihrer Macht stehende für deren Geheimhaltung zu tun.
 * Zuwiderhandlungen werden strafrechtlich verfolgt.
 */

#pragma once

/* stl header */
#include <memory>

/* qt header */
#include <QObject>
#if QT_VERSION >= QT_VERSION_CHECK( 6, 1, 0 )
  #include <QNetworkInformation>
#endif
#if QT_VERSION < QT_VERSION_CHECK( 6, 0, 0 )
  #include <QNetworkConfigurationManager>
#endif

/* local header */
#include "Device.h"

/**
 * @~english
 * @brief The vxstats namespace.
 *
 * @~german
 * @brief Der vxstats Namensraum.
 */
namespace vxstats {

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
    explicit Reachability( QObject *_parent = nullptr );

    /**
     * @~english
     * @brief Any changes on Reachability status. Online/Offline, Status, Radio.
     * @param _newReachability   Changed connection.
     *
     * @~german
     * @brief Änderungen im Netzwerk zu jedem Status. Online/Offline, Status,
     * @param _newReachability   Geänderte Verbindungsart.
     * Funk.
     */
#if QT_VERSION >= QT_VERSION_CHECK( 6, 1, 0 )
    void slotUpdateReachability( QNetworkInformation::Reachability _newReachability );
#else
    void slotUpdateReachability();
#endif

  Q_SIGNALS:
    /**
     * @~english
     * @brief Signal is emitted, if the reachability is changed.
     * @param _connection   Changed connection.
     * @param _radio   Changed radio.
     *
     * @~german
     * @brief Signal wird gesendet, wenn sich die Verbindung geändert hat.
     * @param _connection   Geänderte Verbindungsart.
     * @param _radio   Geänderte Funkverbindung.
     */
    void reachabilityChanged( Device::Connection _connection,
                              Device::Radio _radio );

#if QT_VERSION < QT_VERSION_CHECK( 6, 0, 0 )
  private:
    /**
     * @~english
     * @brief Network configuration manager to bear the correct network type.
     *
     * @~german
     * @brief Manager des konfigurierten Netzwerkes um Informationen zu dem
     * aktuellen Typ zu erhalten.
     */
    std::unique_ptr<QNetworkConfigurationManager> m_networkConfigurationManager = nullptr;
#endif
  };
}
