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
#include <QDebug>
#include <QFile>

/* system header */
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/sysctl.h>
#include <ifaddrs.h>
#include <net/if.h>
#include <netdb.h>

/* objc header */
#include <UIKit/UIKit.h>

/* local header */
#include "Device.h"

namespace VX {

  class Device_ios : public Device {

  public:
    Device_ios();
    bool isJailbroken() const final;
    bool isTabletMode() const final;
    bool hasTouchScreen() const final;
    bool isVoiceOverActive() const final;
#if QT_VERSION < 0x050900
    QString osVersion() const final;
#endif
    Connection typeOfNetwork( const QString &_interface ) final;
  };

  Device &Device::instance() {

    static Device_ios device;
    return device;
  }

  Device_ios::Device_ios() {

    setVendor( QStringLiteral( "Apple Inc." ) );

    size_t len = 0;
    ::sysctlbyname( "hw.machine", nullptr, &len, nullptr, 0 );

    char *model = ( char * )malloc( len + 1 ); // TODO: new char[ len + 1 ];
    ::memset( model, 0, len + 1 );

    ::sysctlbyname( "hw.machine", model, &len, nullptr, 0 );
    QString hwmodel = model;
    free( model );

    if ( hwmodel == QStringLiteral( "x86_64" ) ) {

      setVersion( hwmodel );
      hwmodel = QStringLiteral( "iOS Simulator" );
    }
    else {

      int versionBegin = hwmodel.indexOf( "," );
      if ( versionBegin > 1 && hwmodel.at( versionBegin - 1 ).isDigit() ) {

        --versionBegin;
      }
      if ( versionBegin > 1 && hwmodel.at( versionBegin - 1 ).isDigit() ) {

        --versionBegin;
      }

      setVersion( hwmodel.right( hwmodel.length() - versionBegin ) );
      setModel( hwmodel.left( versionBegin ) );
    }
  }

  bool Device_ios::isJailbroken() const {

    bool jailbroken = false;
    if ( QFile::exists( QStringLiteral( "/Applications/Cydia.app" ) ) ) {

      jailbroken = true;
    }
    else if ( QFile::exists( QStringLiteral( "/Library/MobileSubstrate/MobileSubstrate.dylib" ) ) ) {

      jailbroken = true;
    }
    else if ( QFile::exists( QStringLiteral( "/bin/bash" ) ) ) {

      jailbroken = true;
    }
    else if ( QFile::exists( QStringLiteral( "/usr/sbin/sshd" ) ) ) {

      jailbroken = true;
    }
    else if ( QFile::exists( QStringLiteral( "/etc/apt" ) ) ) {

      jailbroken = true;
    }

    QFile file( QStringLiteral( "/private/jailbreak.txt" ) );
    if ( file.open( QIODevice::WriteOnly ) ) {

      jailbroken = true;
      file.close();
    }
    return jailbroken;
  }

  bool Device_ios::isTabletMode() const {

    return true;
  }

  bool Device_ios::hasTouchScreen() const {

    return true;
  }

  bool Device_ios::isVoiceOverActive() const {

    return UIAccessibilityIsVoiceOverRunning();
  }

#if QT_VERSION < 0x050900
  QString Device_ios::osVersion() const {

    NSOperatingSystemVersion version = [[NSProcessInfo processInfo] operatingSystemVersion];
    return QString::fromNSString( [NSString stringWithFormat:@"%zd.%zd.%zd", version.majorVersion, version.minorVersion, version.patchVersion] );
  }
#endif

  Device::Connection Device_ios::typeOfNetwork( const QString & ) {

    Device::Connection result = Device::Connection::Unknown;
    struct ifaddrs *allInterfaces = nullptr;

    /* Get list of all interfaces on the local machine: */
    if ( getifaddrs( &allInterfaces ) == 0 ) {

      struct ifaddrs *interface = nullptr;

      /* For each interface ... */
      for ( interface = allInterfaces; interface != nullptr; interface = interface->ifa_next ) {

        unsigned int flags = interface->ifa_flags;
        struct sockaddr *addr = interface->ifa_addr;

        /* Check for running IPv4, IPv6 interfaces. Skip the loopback interface. */
        if ( ( flags & ( IFF_UP | IFF_RUNNING | IFF_LOOPBACK ) ) == ( IFF_UP | IFF_RUNNING ) ) {

          if ( addr->sa_family == AF_INET || addr->sa_family == AF_INET6 ) {

            /* Convert interface address to a human readable string: */
            char host[ NI_MAXHOST ];
            getnameinfo( addr, addr->sa_len, host, sizeof( host ), nullptr, 0, NI_NUMERICHOST );

#ifdef DEBUG
            qDebug() << interface->ifa_name << host;
#endif
          }
        }
      }
      freeifaddrs( allInterfaces );
    }
    return result;
  }
}
