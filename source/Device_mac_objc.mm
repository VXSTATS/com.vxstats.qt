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
#ifdef DEBUG
  #include <QDebug>
#endif

/* system header */
#include <cstdio>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/sysctl.h>
#include <ifaddrs.h>
#include <net/if.h>
#include <netdb.h>

/* objc header */
#include <Foundation/Foundation.h>
#include <CoreFoundation/CoreFoundation.h>
#include <SystemConfiguration/SystemConfiguration.h>

/* local header */
#include "Device.h"

namespace VX {

  class Device_mac : public Device {

  public:
    Device_mac();
    bool isVoiceOverActive() const final;
#if QT_VERSION < 0x050900
    QString osVersion() const final;
#endif
    Connection typeOfNetwork( const QString &_interface ) final;
  };

  Device &Device::instance() {

    static Device_mac device;
    return device;
  }

  Device_mac::Device_mac() {

    setVendor( QStringLiteral( "Apple Inc." ) );

    std::size_t len = 0;
    ::sysctlbyname( "hw.model", nullptr, &len, nullptr, 0 );

    std::vector<char> model;
    model.resize( len );

    ::sysctlbyname( "hw.model", model.data(), &len, nullptr, 0 );
    QString hwmodel = model.data();

    int versionBegin = hwmodel.indexOf( QStringLiteral( "," ) );
    if ( versionBegin > 1 && hwmodel.at( versionBegin - 1 ).isDigit() ) {

      --versionBegin;
    }
    if ( versionBegin > 1 && hwmodel.at( versionBegin - 1 ).isDigit() ) {

      --versionBegin;
    }

    setVersion( hwmodel.right( hwmodel.length() - versionBegin ) );
    hwmodel = hwmodel.left( versionBegin );
    if ( hwmodel.endsWith( QStringLiteral( "Pro" ) ) ) {

      hwmodel.replace( QStringLiteral( "Pro" ), QStringLiteral( " Pro" ) );
    }
    else if ( hwmodel.endsWith( QStringLiteral( "Air" ) ) ) {

      hwmodel.replace( QStringLiteral( "Air" ), QStringLiteral( " Air" ) );
    }
    else if ( hwmodel.endsWith( QStringLiteral( "TV" ) ) ) {

      hwmodel.replace( QStringLiteral( "TV" ), QStringLiteral( " TV" ) );
    }
    else if ( hwmodel.endsWith( QStringLiteral( "mini" ) ) ) {

      hwmodel.replace( QStringLiteral( "mini" ), QStringLiteral( " mini" ) );
    }
    setModel( hwmodel );
  }

#if QT_VERSION < 0x050900
  QString Device_mac::osVersion() const {

    NSOperatingSystemVersion version = [[NSProcessInfo processInfo] operatingSystemVersion];
    return QString::fromNSString( [NSString stringWithFormat:@"%zd.%zd.%zd", version.majorVersion, version.minorVersion, version.patchVersion] );
  }
#endif

  bool Device_mac::isVoiceOverActive() const {

    Boolean value = false;
    Boolean result = CFPreferencesGetAppBooleanValue( CFSTR( "voiceOverOnOffKey" ), CFSTR( "com.apple.universalaccess" ), &value );
    return value ? result : value;
  }

  Device::Connection Device_mac::typeOfNetwork( const QString & /*_interface*/ ) {

    QStringList allUpDevices;
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
            std::array<char, NI_MAXHOST> host{};
            getnameinfo( addr, addr->sa_len, host.data(), host.size(), nullptr, 0, NI_NUMERICHOST );

            QString interfaceName = interface->ifa_name;
            if ( interfaceName.startsWith( QStringLiteral( "en" ) ) ) {

              allUpDevices << interface->ifa_name;
            }
#ifdef DEBUG
            qDebug() << interface->ifa_name << host.data();
#endif
          }
        }
      }
      freeifaddrs( allInterfaces );
    }

    Device::Connection result = Device::Connection::Unknown;
    CFArrayRef interfaceArray = SCNetworkInterfaceCopyAll();
    NSArray *networkInterfaces = CFBridgingRelease( interfaceArray );
    CFStringRef displayName;
    CFStringRef bsdName;
    CFStringRef type;

    for ( NSUInteger x = 0; x < networkInterfaces.count; x++ ) {

      SCNetworkInterfaceRef interface = ( __bridge SCNetworkInterfaceRef )( networkInterfaces[ x ] );

      displayName = SCNetworkInterfaceGetLocalizedDisplayName( interface );
      bsdName = SCNetworkInterfaceGetBSDName( interface );
      type = SCNetworkInterfaceGetInterfaceType( interface );
      if ( allUpDevices.contains( QString::fromCFString( bsdName ) ) ) {

#ifdef DEBUG
        qDebug() << QString::fromCFString( type ) << QString::fromCFString( bsdName ) << QString::fromCFString( displayName );
#endif
        bool end = false;
        if ( type != nullptr ) {

          if ( CFEqual( type, kSCNetworkInterfaceTypeBluetooth ) ) {

            result = Device::Connection::Bluetooth;
            end = true;
          }
          else if ( CFEqual( type, kSCNetworkInterfaceTypeEthernet ) ) {

            result = Device::Connection::Ethernet;
            end = true;
          }
          else if ( CFEqual( type, kSCNetworkInterfaceTypeIEEE80211 ) ) {

            result = Device::Connection::Wifi;
            end = true;
          }
        }
        if ( end ) {

          break;
        }
      }
    }
    CFRelease( interfaceArray );
    return result;
  }
}
