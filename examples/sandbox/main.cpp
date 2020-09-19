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
#ifdef QT_GUI_LIB
  #include <QGuiApplication>
#else
  #include <QCoreApplication>
#endif

/* local header */
#include "App.h"
#include "Device.h"
#include "Statistics.h"

int main( int argc, char *argv[] ) {

#ifdef QT_GUI_LIB
  QGuiApplication app( argc, argv );
#else
  QCoreApplication app( argc, argv );
#endif
  QCoreApplication::setApplicationName( QStringLiteral( "com.vxstats.sandbox" ) );
  QCoreApplication::setApplicationVersion( QStringLiteral( "1.0" ) );

  VX::App::instance().setBuild( QStringLiteral( "100" ) );

  VX::Statistics::instance().setServerFilePath( QStringLiteral( "https://sandbox.vxstats.com/" ) );
  VX::Statistics::instance().page( QStringLiteral( "Demo" ) );

#ifdef QT_GUI_LIB
  return QGuiApplication::exec();
#else
  return QCoreApplication::exec();
#endif
}
