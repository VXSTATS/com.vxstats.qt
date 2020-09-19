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
#include <QFile>

/* local header */
#include "Device.h"

namespace VX {

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

      QString vendor = vendorFile.readAll();
      vendor = vendor.simplified();
      setVendor( vendor );
      vendorFile.close();
    }

    QFile modelFile( QStringLiteral( "/sys/class/dmi/id/product_name" ) );
    if ( modelFile.exists() && modelFile.open( QIODevice::ReadOnly ) ) {

      QString model = modelFile.readAll();
      model = model.simplified();
      setModel( model );
      modelFile.close();
    }

    QFile versionFile( QStringLiteral( "/sys/class/dmi/id/product_version" ) );
    if ( versionFile.exists() && versionFile.open( QIODevice::ReadOnly ) ) {

      QString version = versionFile.readAll();
      version = version.simplified();
      if ( version != QStringLiteral( "None" ) ) {

        setVersion( version );
      }
      versionFile.close();
    }

    if ( version().isEmpty() ) {

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
  }
}
