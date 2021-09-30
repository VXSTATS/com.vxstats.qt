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
#include <QCryptographicHash>
#include <QNetworkInterface>
#include <QSettings>
#include <QUuid>

#if QT_VERSION >= QT_VERSION_CHECK( 5, 9, 0 )
  #include <QOperatingSystemVersion>
#endif

#ifdef QT_GUI_LIB
  #if QT_VERSION >= QT_VERSION_CHECK( 6, 0, 0 )
    #include <QInputDevice>
  #else
    #include <QTouchDevice>
  #endif
#endif

/* local header */
#include "Device.h"

namespace vxstats {

  /* uuid struct: time-version-clock_seq_hi-clock_seq_lo-node */

  /** uuid: time start. */
  constexpr int timeStart = 0;
  /** uuid: time length. */
  constexpr int timeLength = 8;

  /** uuid: version start. */
  constexpr int versionStart = 8;
  /** uuid: version length. */
  constexpr int versionLength = 4;

  /** uuid: clock sequence high start. */
  constexpr int clockSequenceHighStart = 12;
  /** uuid: clock sequence high length. */
  constexpr int clockSequenceHighLength = 4;

  /** uuid: clock sequence low start. */
  constexpr int clockSequenceLowStart = 16;
  /** uuid: clock sequence low length. */
  constexpr int clockSequenceLowLength = 4;

  /** uuid: node start. */
  constexpr int nodeStart = 20;
  /** uuid: node length. */
  constexpr int nodeLength = 12;

  void Device::initialize() {

    if ( m_initialized ) {

      return;
    }

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
      if ( interface.hardwareAddress() == QStringLiteral( "00:00:00:00:00:00" ) ) {

        continue;
      }
      /* iOS will print that value above 7.0, which is not helpful */
      if ( interface.hardwareAddress() == QStringLiteral( "02:00:00:00:00:00" ) ) {

        continue;
      }
      if ( interface.humanReadableName().startsWith( QStringLiteral( "vm" ) ) ) {

        continue;
      }
      if ( !isPhysical( interface.hardwareAddress() ) ) {

        continue;
      }
      hardwareAddress = interface.hardwareAddress().toLower();
      break;
    }

    if ( hardwareAddress.isEmpty() ) {

#if defined Q_OS_OSX || defined Q_OS_IOS
      m_uniqueIdentifier = uniqueId();
#else
      QSettings settings( QStringLiteral( "group.com.vxstats.statistics" ) );
      if ( !settings.contains( QStringLiteral( "uuid" ) ) ) {

        QUuid uuid = QUuid::createUuid();
        settings.setValue( QStringLiteral( "uuid" ), uuid.toString().remove( '{' ).remove( '}' ) );
        settings.sync();
      }
      m_uniqueIdentifier = settings.value( QStringLiteral( "uuid" ) ).toString();
#endif
    }
    else {

#ifdef DEBUG
      qDebug() << Q_FUNC_INFO << __LINE__ << "Hardware Address" << hardwareAddress;
#endif
      QString hash = QString::fromLatin1( QCryptographicHash::hash( hardwareAddress.toUtf8(), QCryptographicHash::Md5 ).toHex() );
      /* UUID struct: time-version-clock_seq_hi-clock_seq_lo-node */
      m_uniqueIdentifier = QString( QStringLiteral( "%1-%2-%3-%4-%5" ) ).arg( hash.mid( timeStart, timeLength ), hash.mid( versionStart, versionLength ), hash.mid( clockSequenceHighStart, clockSequenceHighLength ), hash.mid( clockSequenceLowStart, clockSequenceLowLength ), hash.mid( nodeStart, nodeLength ) );
    }
    m_initialized = true;
  }

  bool Device::useDarkMode() const {

    return false;
  }

  bool Device::isJailbroken() const {

    return false;
  }

  bool Device::isTabletMode() const {

    return false;
  }

  bool Device::hasTouchScreen() const {

#ifdef QT_GUI_LIB
#if QT_VERSION >= QT_VERSION_CHECK( 6, 0, 0 )
    const QList<const QInputDevice *> devices = QInputDevice::devices();
    return std::any_of( devices.begin(), devices.end(), []( const auto *_device ) {

      return _device->type() == QInputDevice::DeviceType::TouchScreen;
    } );
#else
    return !QTouchDevice::devices().empty();
#endif
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
#if QT_VERSION >= QT_VERSION_CHECK( 5, 9, 0 )
    QOperatingSystemVersion current = QOperatingSystemVersion::current();
    if ( current.majorVersion() != -1 ) {

      version = QString::number( current.majorVersion() );
      if ( current.minorVersion() != -1 ) {

        version += QStringLiteral( "." );
        version += QString::number( current.minorVersion() );
        if ( current.microVersion() != -1 ) {

          version += QStringLiteral( "." );
          version += QString::number( current.microVersion() );
        }
      }
    }
#endif
    return version;
  }

  Device::Connection Device::typeOfNetwork( [[maybe_unused]] const QString &_interface ) {

    return Device::Connection::Unknown;
  }

  void Device::addOutstandingMessage( [[maybe_unused]] const QString &_message ) const {

    /* Virtual function */
  }

  QStringList Device::sendOutstandingMessages() const {

    return {};
  }

  void Device::tryToSplitVersionFromModel() {

    /* Do we find space and only numbers from behind - this could be the version */
    auto pos = model().lastIndexOf( QChar::Space );
    if ( pos != -1 ) {

      bool isVersion = true;
      QString maybeVersion = model().mid( pos + 1, model().size() - ( pos + 1 ) );
      for ( QChar chr : maybeVersion ) {

        if ( !chr.isDigit() && !chr.isPunct() ) {

          isVersion = false;
          break;
        }
      }
      if ( isVersion ) {

        setModel( model().left( pos ) );
        setVersion( maybeVersion );
      }
    }
  }

  bool Device::isPhysical( [[maybe_unused]] const QString &_hardwareAddress ) const {

    return true;
  }

  QString Device::uniqueId() const {

    return {};
  }
}
