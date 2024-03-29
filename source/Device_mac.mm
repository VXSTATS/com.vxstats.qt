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

/* c header */
#include <cstdio>

/* system header */
#include <ifaddrs.h>
#include <net/if.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/sysctl.h>
#include <sys/types.h>

/* stl header */
#include <array>

/* objc header */
#include <CoreFoundation/CoreFoundation.h>
#include <Foundation/Foundation.h>
#include <SystemConfiguration/SystemConfiguration.h>

/* local header */
#include "Device.h"

namespace vxstats {

  /**
   * @~english
   * @brief The Device class for macOS.
   * Delivers information about the current device.
   *
   * @~german
   * @brief Die Device Klasse für macOS.
   * Liefert Informationen zum aktuellen Gerät.
   */
  class Device_mac : public Device {

  public:
    Device_mac();

  private:
    [[nodiscard]] bool useDarkMode() const final;
    [[nodiscard]] bool isVoiceOverActive() const final;
#if QT_VERSION < QT_VERSION_CHECK( 5, 9, 0 )
    QString osVersion() const final;
#endif
    Connection typeOfNetwork( const QString &_interface ) final;
    void addOutstandingMessage( const QString &_message ) const final;
    [[nodiscard]] QVector<QString> sendOutstandingMessages() const final;
    [[nodiscard]] QString uniqueId() const final;
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

    auto versionBegin = hwmodel.indexOf( ',' );
    if ( versionBegin > 1 && hwmodel.at( versionBegin - 1 ).isDigit() ) {

      --versionBegin;
    }
    if ( versionBegin > 1 && hwmodel.at( versionBegin - 1 ).isDigit() ) {

      --versionBegin;
    }

    setVersion( hwmodel.right( hwmodel.length() - versionBegin ) );
    setModel( hwmodel.left( versionBegin ) );
  }

#if QT_VERSION < QT_VERSION_CHECK( 5, 9, 0 )
  QString Device_mac::osVersion() const {

    NSOperatingSystemVersion version = [[NSProcessInfo processInfo] operatingSystemVersion];
    return QString::fromNSString( [NSString stringWithFormat:@"%zd.%zd.%zd", version.majorVersion, version.minorVersion, version.patchVersion] );
  }
#endif

  bool Device_mac::useDarkMode() const {

    return [[[NSUserDefaults standardUserDefaults] stringForKey:@"AppleInterfaceStyle"] length] > 0;
  }

  bool Device_mac::isVoiceOverActive() const {

    CFStringRef voiceOverOnOffKey = CFStringCreateWithCString( nullptr, "voiceOverOnOffKey", kCFStringEncodingASCII );
    CFStringRef universalaccess = CFStringCreateWithCString( nullptr, "com.apple.universalaccess", kCFStringEncodingASCII );

    Boolean value = false;
    Boolean result = CFPreferencesGetAppBooleanValue( voiceOverOnOffKey, universalaccess, &value );

    CFRelease( universalaccess );
    CFRelease( voiceOverOnOffKey );

    return value ? result : value;
  }

  Device::Connection Device_mac::typeOfNetwork( [[maybe_unused]] const QString &_interface ) {

    QVector<QString> allUpDevices;
    struct ifaddrs *allInterfaces = nullptr;

    /* Get list of all interfaces on the local machine: */
    if ( getifaddrs( &allInterfaces ) == 0 ) {

      /* For each interface ... */
      for ( struct ifaddrs *interface = allInterfaces; interface != nullptr; interface = interface->ifa_next ) {

        unsigned int flags = interface->ifa_flags;
        const struct sockaddr *addr = interface->ifa_addr;

        /* Check for running IPv4, IPv6 interfaces. Skip the loopback interface. */
        if ( ( flags & static_cast<unsigned int>( IFF_UP | IFF_RUNNING | IFF_LOOPBACK ) ) == static_cast<unsigned int>( IFF_UP | IFF_RUNNING ) ) {

          if ( addr->sa_family == AF_INET || addr->sa_family == AF_INET6 ) {

            /* Convert interface address to a human readable string: */
            std::array<char, NI_MAXHOST> host {};
            getnameinfo( addr, addr->sa_len, host.data(), host.size(), nullptr, 0, NI_NUMERICHOST );

            QString interfaceName = interface->ifa_name;
            if ( interfaceName.startsWith( QStringLiteral( "en" ) ) ) {

              allUpDevices << interface->ifa_name;
            }
#ifdef DEBUG
            qDebug() << "Interface:" << interface->ifa_name << host.data();
#endif
          }
        }
      }
      freeifaddrs( allInterfaces );
    }

    Device::Connection result = Device::Connection::Unknown;
    CFArrayRef interfaceArray = SCNetworkInterfaceCopyAll();
    CFStringRef displayName = nullptr;
    CFStringRef bsdName = nullptr;
    CFStringRef type = nullptr;

    for ( CFIndex x = 0; x < CFArrayGetCount( interfaceArray ); x++ ) {

      const auto *interface = static_cast<SCNetworkInterfaceRef>( CFArrayGetValueAtIndex( interfaceArray, x ) );

      displayName = SCNetworkInterfaceGetLocalizedDisplayName( interface );
      bsdName = SCNetworkInterfaceGetBSDName( interface );
      type = SCNetworkInterfaceGetInterfaceType( interface );
      if ( allUpDevices.contains( QString::fromCFString( bsdName ) ) ) {

#ifdef DEBUG
        qDebug() << "Type/Name:" << QString::fromCFString( type ) << QString::fromCFString( bsdName ) << QString::fromCFString( displayName );
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

  void Device_mac::addOutstandingMessage( const QString &_message ) const {

    if ( _message.isEmpty() ) {

      return;
    }

    NSUserDefaults *userDefaults = [[NSUserDefaults alloc] initWithSuiteName:@"group.com.vxstats.statistics"];
    /* add to queue */
    NSArray *existingMessages = [userDefaults objectForKey:@"offline"];
    NSMutableArray *messages = [NSMutableArray alloc];
    if ( existingMessages != nullptr ) {

      [messages addObjectsFromArray:existingMessages];
    }
    const NSString *message = _message.toNSString();
    [messages addObject:message];
    [userDefaults setObject:messages forKey:@"offline"];
    [userDefaults synchronize];
  }

  QVector<QString> Device_mac::sendOutstandingMessages() const {

    QVector<QString> result;
    NSUserDefaults *userDefaults = [[NSUserDefaults alloc] initWithSuiteName:@"group.com.vxstats.statistics"];
    NSArray *messages = [userDefaults objectForKey:@"offline"];
    [userDefaults removeObjectForKey:@"offline"];
    [userDefaults synchronize];
    for ( NSUInteger x = 0; x < [messages count]; ++x ) {

      result += QString::fromNSString( [messages objectAtIndex:x] );
    }
    return result;
  }

  QString Device_mac::uniqueId() const {

    NSUserDefaults *userDefaults = [[NSUserDefaults alloc] initWithSuiteName:@"group.com.vxstat.statistics"];
    NSString *uuid = [userDefaults objectForKey:@"uuid"];
    if ( uuid == nullptr ) {

      uuid = [[NSUUID UUID] UUIDString];
      [userDefaults setObject:uuid forKey:@"uuid"];
      [userDefaults synchronize];
    }
    return QString::fromNSString( uuid );
  }
}
