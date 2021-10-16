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
#include <QFile>

#ifdef QT_GUI_LIB
  #if QT_VERSION >= QT_VERSION_CHECK( 6, 0, 0 )
    #include <QInputDevice>
  #else
    #include <QTouchDevice>
  #endif
#endif

/* c header */
#include <cstdio>

/* system header */
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/sysctl.h>
#include <ifaddrs.h>
#include <net/if.h>
#include <netdb.h>

/* objc header */
#include <Foundation/Foundation.h>
#include <UIKit/UIKit.h>

/* local header */
#include "Device.h"

namespace vxstats {

  /**
   * @~english
   * @brief The Device class for iOS.
   * Delivers information about the current device.
   *
   * @~german
   * @brief Die Device Klasse für iOS.
   * Liefert Informationen zum aktuellen Gerät.
   */
  class Device_ios : public Device {

  public:
    Device_ios();

  private:
    [[nodiscard]] bool useDarkMode() const final;
    [[nodiscard]] bool isJailbroken() const final;
    [[nodiscard]] bool isTabletMode() const final;
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

    static Device_ios device;
    return device;
  }

  Device_ios::Device_ios() {

    setVendor( QStringLiteral( "Apple Inc." ) );

    std::size_t len = 0;
    ::sysctlbyname( "hw.machine", nullptr, &len, nullptr, 0 );

    std::vector<char> model;
    model.resize( len );

    ::sysctlbyname( "hw.machine", model.data(), &len, nullptr, 0 );
    QString hwmodel = model.data();

    if ( hwmodel == QStringLiteral( "x86_64" ) ) {

      setVersion( hwmodel );
      hwmodel = QStringLiteral( "iOS Simulator" );
    }
    else {

      auto versionBegin = hwmodel.indexOf( "," );
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

  bool Device_ios::useDarkMode() const {

    return [[[NSUserDefaults standardUserDefaults] stringForKey:@"AppleInterfaceStyle"] length] > 0;
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

  bool Device_ios::isVoiceOverActive() const {

    return UIAccessibilityIsVoiceOverRunning();
  }

#if QT_VERSION < QT_VERSION_CHECK( 5, 9, 0 )
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
        const struct sockaddr *addr = interface->ifa_addr;

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

  void Device_ios::addOutstandingMessage( const QString &_message ) const {

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

  QVector<QString> Device_ios::sendOutstandingMessages() const {

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

  QString Device_ios::uniqueId() const {

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
