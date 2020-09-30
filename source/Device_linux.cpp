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

/* qt header */
#include <QFile>

/* local header */
#include "Device.h"

namespace vxstats {

  class Device_linux : public Device {

  public:
    Device_linux();
  };

  Device &Device::instance() {

    static Device_linux device;
    return device;
  }

  Device_linux::Device_linux() {

    QFile vendorFile( QStringLiteral( "/sys/class/dmi/id/sys_vendor" ) );
    if ( vendorFile.exists() && vendorFile.open( QIODevice::ReadOnly ) ) {

      QString vendor = QString::fromLatin1( vendorFile.readAll() );
      vendor = vendor.simplified();
      setVendor( vendor );
      vendorFile.close();
    }

    QFile modelFile( QStringLiteral( "/sys/class/dmi/id/product_name" ) );
    if ( modelFile.exists() && modelFile.open( QIODevice::ReadOnly ) ) {

      QString model = QString::fromLatin1( modelFile.readAll() );
      model = model.simplified();
      setModel( model );
      modelFile.close();
    }

    QFile versionFile( QStringLiteral( "/sys/class/dmi/id/product_version" ) );
    if ( versionFile.exists() && versionFile.open( QIODevice::ReadOnly ) ) {

      QString version = QString::fromLatin1( versionFile.readAll() );
      version = version.simplified();
      if ( version != QStringLiteral( "None" ) ) {

        setVersion( version );
      }
      versionFile.close();
    }

    if ( version().isEmpty() ) {

      tryToSplitVersionFromModel();
    }
  }
}
