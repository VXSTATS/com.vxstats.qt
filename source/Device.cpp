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

/* qt header */
#include <QCryptographicHash>
#include <QNetworkInterface>
#include <QSettings>
#include <QUuid>

#if QT_VERSION >= 0x050900
  #include <QOperatingSystemVersion>
#endif

#ifdef QT_GUI_LIB
  #include <QTouchDevice>
#endif

/* local header */
#include "Device.h"

namespace VX {

  /* uuid struct: time-version-clock_seq_hi-clock_seq_lo-node */
  constexpr int timeStart = 0;
  constexpr int timeLength = 8;

  constexpr int versionStart = 8;
  constexpr int versionLength = 4;

  constexpr int clockSequenceHighStart = 12;
  constexpr int clockSequenceHighLength = 4;

  constexpr int clockSequenceLowStart = 16;
  constexpr int clockSequenceLowLength = 4;

  constexpr int nodeStart = 20;
  constexpr int nodeLength = 12;

  Device::Device() {

    QString hardwareAddress;
    const QList<QNetworkInterface> interfaces = QNetworkInterface::allInterfaces();
    for ( const QNetworkInterface &interface : interfaces ) {

      if ( interface.flags() & QNetworkInterface::IsLoopBack ) {

        continue;
      }
      if ( interface.flags() & QNetworkInterface::IsPointToPoint ) {

        continue;
      }
      if ( interface.hardwareAddress().isEmpty() ) {

        continue;
      }
      if ( interface.hardwareAddress() == QLatin1String( "00:00:00:00:00:00" ) ) {

        continue;
      }
      /* iOS will print that value above 7.0, which is not helpful */
      if ( interface.hardwareAddress() == QLatin1String( "02:00:00:00:00:00" ) ) {

        continue;
      }
      if ( interface.humanReadableName().startsWith( QLatin1String( "vm" ) ) ) {

        continue;
      }
      if ( !isPhysical( interface.hardwareAddress() ) ) {

        continue;
      }
      hardwareAddress = interface.hardwareAddress();
      break;
    }

    if ( hardwareAddress.isEmpty() ) {

      QSettings settings( QStringLiteral( "com.vxstats" ), QStringLiteral( "statistics" ) );
      settings.beginGroup( QStringLiteral( "statistics" ) );
      if ( !settings.contains( QStringLiteral( "uuid" ) ) ) {

        QUuid uuid = QUuid::createUuid();
        settings.setValue( QStringLiteral( "uuid" ), uuid.toString().remove( '{' ).remove( '}' ) );
        settings.sync();
      }
      m_uniqueIdentifier = settings.value( QStringLiteral( "uuid" ) ).toString();
    }
    else {

#ifdef DEBUG
      qDebug() << Q_FUNC_INFO << __LINE__ << "Hardware Address" << hardwareAddress;
#endif
      QString hash = QString::fromLatin1( QCryptographicHash::hash( hardwareAddress.toUtf8(), QCryptographicHash::Md5 ) .toHex() );
      /* UUID struct: time-version-clock_seq_hi-clock_seq_lo-node */
      m_uniqueIdentifier = QString( "%1-%2-%3-%4-%5" ).arg( hash.mid( timeStart, timeLength ), hash.mid( versionStart, versionLength ), hash.mid( clockSequenceHighStart, clockSequenceHighLength ), hash.mid( clockSequenceLowStart, clockSequenceLowLength ), hash.mid( nodeStart, nodeLength ) );
    }
  }

  bool Device::isJailbroken() const {

    return false;
  }

  bool Device::isTabletMode() const {

    return false;
  }

  bool Device::hasTouchScreen() const {

#ifdef QT_GUI_LIB
    return QTouchDevice::devices().size() > 0;
#else
    return false;
#endif
  }

  bool Device::isVoiceOverActive() const {

    return false;
  }

  void Device::setModel( const QString &_model ) {

    if ( m_model == _model ) {

      return;
    }
    m_model = _model;
  }

  void Device::setVendor( const QString &_vendor ) {

    if ( m_vendor == _vendor ) {

      return;
    }
    m_vendor = _vendor;
  }

  void Device::setVersion( const QString &_version ) {

    if ( m_version == _version ) {

      return;
    }
    m_version = _version;
  }

  QString Device::osVersion() const {

    QString version = QSysInfo::productVersion();
#if QT_VERSION >= 0x050900
    QOperatingSystemVersion current = QOperatingSystemVersion::current();
    if ( current.majorVersion() != -1 ) {

      version = QString::number( current.majorVersion() );
      if ( current.minorVersion() != -1 ) {

        version += QLatin1String( "." );
        version += QString::number( current.minorVersion() );
        if ( current.microVersion() != -1 ) {

          version += QLatin1String( "." );
          version += QString::number( current.microVersion() );
        }
      }
    }
#endif
    return version;
  }

  Device::Connection Device::typeOfNetwork( const QString &_interface ) {

    Q_UNUSED( _interface )
    return Device::Connection::Unknown;
  }

  bool Device::isPhysical( const QString &_hardwareAddress ) const {

    Q_UNUSED( _hardwareAddress )
    return true;
  }
}
