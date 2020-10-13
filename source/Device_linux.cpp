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
#include <QDir>
#include <QFile>
#include <QTextStream>

#ifdef QT_GUI_LIB
  #include <QTouchDevice>
#endif

/* local header */
#include "Device.h"

namespace vxstats {

  /**
   * @~english
   * @brief The Device class for Linux.
   * Delivers information about the current device.
   *
   * @~german
   * @brief Die Device Klasse für Linux.
   * Liefert Informationen zum aktuellen Gerät.
   */
  class Device_linux : public Device {

  public:
    Device_linux();

  private:
    [[nodiscard]] bool hasTouchScreen() const final;
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

    /* Get a vendor name, if no dmi is present */
    if ( vendor().isEmpty() ) {

      QDir dir( QStringLiteral( "/sys/bus/pci/devices" ) );
      QStringList folders = dir.entryList( {}, QDir::NoDot | QDir::NoDotDot | QDir::Dirs );
      QFile vendorFileAlternative( QStringLiteral( "/sys/bus/pci/devices/" ) + folders.at( 0 ) + QStringLiteral( "/vendor" ) );
      if ( vendorFileAlternative.exists() && vendorFileAlternative.open( QIODevice::ReadOnly ) ) {

        QString hexVendor = vendorFileAlternative.readAll();
        hexVendor = hexVendor.simplified();
        hexVendor = hexVendor.right( hexVendor.size() - 2 );
        vendorFileAlternative.close();

        /* Identify vendor */
        QStringList idsFiles;
        idsFiles << QStringLiteral( "/pci.ids" ) << QStringLiteral( "/usr/share/lshw/pci.ids" ) << QStringLiteral( "/usr/local/share/pci.ids" );
        idsFiles << QStringLiteral( "/usr/share/pci.ids" ) << QStringLiteral( "/etc/pci.ids" ) << QStringLiteral( "/usr/share/hwdata/pci.ids" );
        idsFiles << QStringLiteral( "/usr/share/misc/pci.ids" );
        for ( const QString &idsFile : idsFiles ) {

          QFile dataFile( idsFile );
          if ( dataFile.exists() && dataFile.open( QIODevice::ReadOnly ) ) {

            QTextStream in( &dataFile );
            while ( !in.atEnd() ) {

              QString line = in.readLine();
              line = line.simplified();
              if ( line.startsWith( hexVendor ) ) {

                line.remove( hexVendor );
                line = line.simplified();
                setVendor( line );
                break;
              }
            }
            dataFile.close();
          }
        }
      }
    }

    QFile modelFile( QStringLiteral( "/sys/class/dmi/id/product_name" ) );
    if ( modelFile.exists() && modelFile.open( QIODevice::ReadOnly ) ) {

      QString model = QString::fromLatin1( modelFile.readAll() );
      model = model.simplified();
      setModel( model );
      modelFile.close();
    }

    /* Get a model name, if no dmi is present */
    if ( model().isEmpty() ) {

      QFile modelFileAlternative( QStringLiteral( "/sys/firmware/devicetree/base/model" ) );
      if ( modelFileAlternative.exists() && modelFileAlternative.open( QIODevice::ReadOnly ) ) {

        QString model = QString::fromLatin1( modelFileAlternative.readAll() );
        model = model.simplified();
        setModel( model );
        modelFileAlternative.close();
      }
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

  bool Device_linux::hasTouchScreen() const {

#ifdef QT_GUI_LIB
    return QTouchDevice::devices().size() > 0;
#else
    return false;
#endif
  }
}
