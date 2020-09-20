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

namespace VX {

  class Device_win : public Device {

  public:
    Device_win();
    bool isTabletMode() const final;
    bool hasTouchScreen() const final;
    bool isVoiceOverActive() const final;
    Connection typeOfNetwork( const QString &_interface ) final;

  private:
    bool isPhysical( const QString &_hardwareAddress ) const final;
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

    setVersion( QString() );
    /* Do we find space and only numbers from behind - this could be the version */
    int pos = model().lastIndexOf( QChar::Space );
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
  
  bool Device_win::isTabletMode() const {

    QSettings settings( QStringLiteral( "HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\ImmersiveShell" ), QSettings::NativeFormat );
    return settings.value( QStringLiteral( "TabletMode" ) ).toInt();
  }

  bool Device_win::hasTouchScreen() const {

    return GetSystemMetrics( SM_MAXIMUMTOUCHES );
  }

  bool Device_win::isVoiceOverActive() const {

    bool bActive = false;
    bool bReturn = SystemParametersInfo( SPI_GETSCREENREADER, 0, &bActive, 0 );
    return bReturn && bActive;
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

    PIP_ADAPTER_INFO pAdapterInfo;
    PIP_ADAPTER_INFO pAdapter = nullptr;
    DWORD dwRetVal = 0;
    UINT i;

    ULONG ulOutBufLen = sizeof( IP_ADAPTER_INFO );
    pAdapterInfo = ( IP_ADAPTER_INFO * ) MALLOC( sizeof( IP_ADAPTER_INFO ) );
    if ( pAdapterInfo == nullptr ) {

      qDebug() << Q_FUNC_INFO << __LINE__ << "Error allocating memory needed to call GetAdaptersinfo";
      return false;
    }

    if ( GetAdaptersInfo( pAdapterInfo, &ulOutBufLen ) == ERROR_BUFFER_OVERFLOW ) {

      FREE( pAdapterInfo );
      pAdapterInfo = ( IP_ADAPTER_INFO * ) MALLOC( ulOutBufLen );
      if ( pAdapterInfo == nullptr ) {

        qDebug() << Q_FUNC_INFO << __LINE__ << "Error allocating memory needed to call GetAdaptersinfo";
        return false;
      }
    }

    if ( ( dwRetVal = GetAdaptersInfo( pAdapterInfo, &ulOutBufLen ) ) == NO_ERROR ) {

      pAdapter = pAdapterInfo;
      while ( pAdapter ) {

        QString description = pAdapter->Description;
        QString hardwareAddress;
        for ( i = 0; i < pAdapter->AddressLength; i++ ) {

          if ( !hardwareAddress.isEmpty() ) {

            hardwareAddress += ":";
          }
          hardwareAddress += QString( "%1" ).arg( pAdapter->Address[i], 2, 16, QLatin1Char( '0' ) );
        }
        hardwareAddress = hardwareAddress.toUpper();
        if ( hardwareAddress == _hardwareAddress ) {

          if ( pAdapterInfo ) {

            FREE( pAdapterInfo );
          }

          if ( description.contains( QStringLiteral( "Virtual Adapter" ) ) ) {

            return false;
          }
          return true;
        }
        pAdapter = pAdapter->Next;
      }
    }
    else {

      qDebug() << Q_FUNC_INFO << __LINE__ << "GetAdaptersInfo failed with error:" << dwRetVal;
    }

    if ( pAdapterInfo ) {

      FREE( pAdapterInfo );
    }
    return false;
  }
}
