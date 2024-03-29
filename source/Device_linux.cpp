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
#include <QDir>
#include <QFile>
#include <QProcess>
#include <QTextStream>

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
    [[nodiscard]] bool useDarkMode() const final;

    void findVendor();
    void findModel();
    void findVersion();
  };

  Device &Device::instance() {

    static Device_linux device;
    return device;
  }

  Device_linux::Device_linux() {

    findVendor();
    findModel();
    findVersion();
  }

  bool Device_linux::useDarkMode() const {

    const QString program = QStringLiteral( "gsettings" );
    QStringList arguments;
    arguments << QStringLiteral( "get" ) << QStringLiteral( "org.gnome.desktop.interface" ) << QStringLiteral( "gtk-theme" );

    QProcess process;
#if QT_VERSION >= QT_VERSION_CHECK( 6, 0, 0 )
    process.startCommand( program + " " + arguments.join( QStringLiteral( " " ) ) );
#else
    process.start( program, arguments );
#endif
    process.waitForFinished();
    const QString result( process.readAllStandardOutput() );
    return result.contains( QStringLiteral( "-dark" ) );
  }

  void Device_linux::findVendor() {

    QFile vendorFile( QStringLiteral( "/sys/class/dmi/id/sys_vendor" ) );
    if ( vendorFile.exists() && vendorFile.open( QIODevice::ReadOnly ) ) {

      QString vendor = QString::fromLatin1( vendorFile.readAll() );
      vendor = vendor.simplified();
      setVendor( vendor );
      vendorFile.close();
    }

    /* Get a vendor name, if no dmi is present */
    if ( vendor().isEmpty() ) {

      const QDir dir( QStringLiteral( "/sys/bus/pci/devices" ) );
      const QStringList folders = dir.entryList( {}, QDir::NoDot | QDir::NoDotDot | QDir::Dirs );
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
        for ( const QString &idsFile : qAsConst( idsFiles ) ) {

          QFile dataFile( idsFile );
          if ( dataFile.exists() && dataFile.open( QIODevice::ReadOnly ) ) {

            QTextStream istream( &dataFile );
            while ( !istream.atEnd() ) {

              QString line = istream.readLine();
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
  }

  void Device_linux::findModel() {

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
  }

  void Device_linux::findVersion() {

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
