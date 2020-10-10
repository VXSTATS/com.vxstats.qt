/*
 * Copyright (C) 10/01/2020 VX APPS <sales@vxapps.com>
 *
 * This document is property of VX APPS. It is strictly prohibited
 * to modify, sell or publish it in any way. In case you have access
 * to this document, you are obligated to ensure its nondisclosure.
 * Noncompliances will be prosecuted.
 *
 * Diese Datei ist Eigentum der VX APPS. Jegliche Änderung, Verkauf
 * oder andere Verbreitung und Veröffentlichung ist strikt untersagt.
 * Falls Sie Zugang zu dieser Datei haben, sind Sie verpflichtet,
 * alles in Ihrer Macht stehende für deren Geheimhaltung zu tun.
 * Zuwiderhandlungen werden strafrechtlich verfolgt.
 */

/* windows header */
#include <windows.h>
#include <stdio.h>
#pragma comment(lib, "user32.lib")
#include <iphlpapi.h>
#include <stdlib.h>
#pragma comment(lib, "IPHLPAPI.lib")

/* qt header */
#include <QDebug>
#include <QSettings>

/* local header */
#include "Device.h"

#define MALLOC(x) HeapAlloc(GetProcessHeap(), 0, (x))
#define FREE(x) HeapFree(GetProcessHeap(), 0, (x))

namespace vxstats {

  class Device_win : public Device {

  public:
    Device_win();

  private:
    [[nodiscard]] bool useDarkMode() const final;
    [[nodiscard]] bool isTabletMode() const final;
    [[nodiscard]] bool hasTouchScreen() const final;
    [[nodiscard]] bool isVoiceOverActive() const final;
    Connection typeOfNetwork( const QString &_interface ) final;
    [[nodiscard]] bool isPhysical( const QString &_hardwareAddress ) const final;
  };

  Device &Device::instance() {

    static Device_win device;
    return device;
  }

  Device_win::Device_win() {

    /* From Windows7: HKEY_LOCAL_MACHINE\\HARDWARE\\DESCRIPTION\\System\\BIOS */
    QSettings settings( "HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Control\\SystemInformation", QSettings::NativeFormat );
    setVendor( settings.value( "SystemManufacturer", {} ).toString() );
    setModel( settings.value( "SystemProductName", {} ).toString() );

    tryToSplitVersionFromModel();
  }

  bool Device_win::useDarkMode() const {

    QSettings settings( QSettings::UserScope, "Microsoft", "Windows" );
    return !settings.value( "CurrentVersion/Themes/Personalize/AppsUseLightTheme", true ).toBool();
  }

  bool Device_win::isTabletMode() const {

    QSettings settings( QStringLiteral( "HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\ImmersiveShell" ), QSettings::NativeFormat );
    return settings.value( QStringLiteral( "TabletMode" ) ).toInt();
  }

  bool Device_win::hasTouchScreen() const {

    return GetSystemMetrics( SM_MAXIMUMTOUCHES );
  }

  bool Device_win::isVoiceOverActive() const {

    std::string narratorMutex = "NarratorRunning";

    /* security attributes are part of windows API for CreateMutex */
    LPSECURITY_ATTRIBUTES securityAttributes = new _SECURITY_ATTRIBUTES();
    securityAttributes->bInheritHandle = false;
    securityAttributes->lpSecurityDescriptor = nullptr;
    securityAttributes->nLength = sizeof( LPSECURITY_ATTRIBUTES );

    /* initialize values */
    bool narratorRunning = false;

    /* CreateMutex returns a windows application HANDLE */
    HANDLE applicationHandle = CreateMutex( securityAttributes, false, narratorMutex.c_str() );

    /* This should never happen */
    if ( applicationHandle == nullptr ) {

      qDebug() << Q_FUNC_INFO << __LINE__ << "Narrator application handle is nullptr";
      narratorRunning = false;
    }
    /* this condition indicates that narrator is running. */
    else if ( GetLastError() == ERROR_ALREADY_EXISTS ) {

      narratorRunning = true;
    }
    delete ( securityAttributes );

    /* Visual Studio 2019, will hate this implementation and report crash around active. */
//    bool active = false;
//    bool result = SystemParametersInfo( SPI_GETSCREENREADER, 0, &active, 0 );
//    return result && active;
    return narratorRunning;
  }

  Device::Connection Device_win::typeOfNetwork( const QString &_interface ) {

    if ( _interface.startsWith( QStringLiteral( "WLAN" ) ) ) {

      return Device::Connection::Wifi;
    }
    else if ( _interface.startsWith( QStringLiteral( "Ethernet" ) ) ) {

      return Device::Connection::Ethernet;
    }
    else if ( _interface.startsWith( QStringLiteral( "Bluetooth" ) ) ) {

      return Device::Connection::Bluetooth;
    }
    return Device::Connection::Unknown;
  }

  bool Device_win::isPhysical( const QString &_hardwareAddress ) const {

    PIP_ADAPTER_INFO adapterInfo;
    PIP_ADAPTER_INFO adapter = nullptr;
    DWORD result = 0;
    UINT i;

    ULONG outLen = sizeof( IP_ADAPTER_INFO );
    adapterInfo = ( IP_ADAPTER_INFO * ) MALLOC( sizeof( IP_ADAPTER_INFO ) );
    if ( adapterInfo == nullptr ) {

      qDebug() << Q_FUNC_INFO << __LINE__ << "Error allocating memory needed to call GetAdaptersinfo";
      return false;
    }

    if ( GetAdaptersInfo( adapterInfo, &outLen ) == ERROR_BUFFER_OVERFLOW ) {

      FREE( adapterInfo );
      adapterInfo = ( IP_ADAPTER_INFO * ) MALLOC( outLen );
      if ( adapterInfo == nullptr ) {

        qDebug() << Q_FUNC_INFO << __LINE__ << "Error allocating memory needed to call GetAdaptersinfo";
        return false;
      }
    }

    if ( ( result = GetAdaptersInfo( adapterInfo, &outLen ) ) == NO_ERROR ) {

      adapter = adapterInfo;
      while ( adapter ) {

        QString description = adapter->Description;
        QString hardwareAddress;
        for ( i = 0; i < adapter->AddressLength; i++ ) {

          if ( !hardwareAddress.isEmpty() ) {

            hardwareAddress += ":";
          }
          hardwareAddress += QString( "%1" ).arg( adapter->Address[i], 2, 16, QLatin1Char( '0' ) );
        }
        hardwareAddress = hardwareAddress.toLower();
        if ( hardwareAddress == _hardwareAddress ) {

          if ( adapterInfo ) {

            FREE( adapterInfo );
          }

          if ( description.contains( QStringLiteral( "Virtual Adapter" ) ) ) {

            return false;
          }
          return true;
        }
        adapter = adapter->Next;
      }
    }
    else {

      qDebug() << Q_FUNC_INFO << __LINE__ << "GetAdaptersInfo failed with error:" << result;
    }

    if ( adapterInfo ) {

      FREE( adapterInfo );
    }
    return false;
  }
}
