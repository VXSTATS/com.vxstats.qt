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

/* qt header */
#ifdef DEBUG
  #include <QDebug>
#endif
#include <QMetaEnum>
#if QT_VERSION < QT_VERSION_CHECK( 6, 0, 0 )
  #include <QNetworkConfiguration>
  #include <QNetworkConfigurationManager>
#endif

/* local header */
#include "Reachability.h"

namespace vxstats {

  Reachability::Reachability( QObject *_parent )
    : QObject( _parent ) {

#if QT_VERSION >= QT_VERSION_CHECK( 6, 1, 0 )
    QNetworkInformation::load( QNetworkInformation::Feature::Reachability );
    if ( QNetworkInformation::instance() ) {

      connect( QNetworkInformation::instance(), &QNetworkInformation::reachabilityChanged, this, &Reachability::slotUpdateReachability );
    }
#else
    m_networkConfigurationManager = new QNetworkConfigurationManager( this );
    connect( m_networkConfigurationManager, &QNetworkConfigurationManager::configurationChanged, this, &Reachability::slotUpdateReachability );
    connect( m_networkConfigurationManager, &QNetworkConfigurationManager::onlineStateChanged, this, &Reachability::slotUpdateReachability );
    connect( m_networkConfigurationManager, &QNetworkConfigurationManager::updateCompleted, this, &Reachability::slotUpdateReachability );
#endif
  }

#if QT_VERSION >= QT_VERSION_CHECK( 6, 1, 0 )
  void Reachability::slotUpdateReachability( QNetworkInformation::Reachability _newReachability ) {

    if ( _newReachability != QNetworkInformation::Reachability::Online ) {

#ifdef DEBUG
      qDebug() << Q_FUNC_INFO << __LINE__ << "Network Offline";
#endif

      Q_EMIT reachabilityChanged( Device::Connection::Offline, Device::Radio::None );
      return;
    }

    Device::Connection connection = Device::Connection::Wifi;
    Device::Radio radio = Device::Radio::None;
    Q_EMIT reachabilityChanged( connection, radio );
  }
#else
  void Reachability::slotUpdateReachability() {

#ifdef DEBUG
    qDebug() << Q_FUNC_INFO << __LINE__;
#endif

    if ( !m_networkConfigurationManager->isOnline() ) {

#ifdef DEBUG
      qDebug() << Q_FUNC_INFO << __LINE__ << "Network Offline";
#endif

      Q_EMIT reachabilityChanged( Device::Connection::Offline, Device::Radio::None );
      return;
    }

    QNetworkConfiguration configuration = m_networkConfigurationManager->defaultConfiguration();
#ifdef DEBUG
    const QList<QNetworkConfiguration> allConfigurations = m_networkConfigurationManager->allConfigurations( QNetworkConfiguration::Active );
    for ( const QNetworkConfiguration &currentConfiguration : allConfigurations ) {

      qDebug() << "configuration" << configuration.bearerType() << configuration.bearerTypeFamily() << configuration.bearerTypeName() << configuration.identifier() << configuration.name() << configuration.isRoamingAvailable() << configuration.state();
      qDebug() << "currentConfiguration" << currentConfiguration.bearerType() << currentConfiguration.bearerTypeFamily() << currentConfiguration.bearerTypeName() << currentConfiguration.identifier() << currentConfiguration.name() << currentConfiguration.isRoamingAvailable() << currentConfiguration.state();
    }
#endif

    Device::Connection connection = Device::Connection::WWAN;
    Device::Radio radio = Device::Radio::None;
    switch ( configuration.bearerType() ) {

      case QNetworkConfiguration::BearerUnknown:
        connection = Device::Connection::Unknown;
        break;
      case QNetworkConfiguration::BearerBluetooth:
        connection = Device::Connection::Bluetooth;
        break;
      case QNetworkConfiguration::BearerEthernet:
        connection = Device::Connection::Ethernet;
        break;
      case QNetworkConfiguration::BearerWLAN:
        connection = Device::Connection::Wifi;
        break;
      case QNetworkConfiguration::BearerWiMAX:
        radio = Device::Radio::WiMAX;
        break;
      case QNetworkConfiguration::Bearer2G:
        radio = Device::Radio::Radio2G;
        break;
      case QNetworkConfiguration::Bearer3G:
        radio = Device::Radio::Radio3G;
        break;
      case QNetworkConfiguration::Bearer4G:
        radio = Device::Radio::Radio4G;
        break;
      case QNetworkConfiguration::BearerWCDMA:
        radio = Device::Radio::WCDMA;
        break;
      case QNetworkConfiguration::BearerCDMA2000:
        radio = Device::Radio::CDMA1x;
        break;
      case QNetworkConfiguration::BearerHSPA:
        radio = Device::Radio::HSDPA;
        break;
      case QNetworkConfiguration::BearerEVDO:
        radio = Device::Radio::CDMAEVDORev0;
        break;
      case QNetworkConfiguration::BearerLTE:
        radio = Device::Radio::LTE;
        break;
    }
    if ( connection == Device::Connection::Unknown ) {

      /* Connection has just a name, not an type */
#ifdef DEBUG
      qDebug() << "typeOfNetwork" << configuration.name();
#endif
      Device::instance().initialize();
      connection = Device::instance().typeOfNetwork( configuration.name() );
    }
    Q_EMIT reachabilityChanged( connection, radio );
  }
#endif
}
