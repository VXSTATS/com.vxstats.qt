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
#ifdef QT_GUI_LIB
  #include <QGuiApplication>
#else
  #include <QCoreApplication>
#endif
#include <QTimer>

/* com.vxstats.qt header */
#include <App.h>
#include <Device.h>
#include <Statistics.h>

/* com.vxstats.qt namespace */
using namespace vxstats;

constexpr int createPageMilliseconds = 1000;

int main( int argc, char *argv[] ) {

#ifdef QT_GUI_LIB
  QGuiApplication app( argc, argv );
#else
  QCoreApplication app( argc, argv );
#endif
  QCoreApplication::setApplicationName( QStringLiteral( "com.vxstats.sandbox" ) );
  QCoreApplication::setApplicationVersion( QStringLiteral( "1.0" ) );

  App::instance().setBuild( QStringLiteral( "100" ) );

  Statistics::instance().setUsername( QStringLiteral( "sandbox" ) );
  Statistics::instance().setPassword( QStringLiteral( "sandbox" ) );
  Statistics::instance().setServerFilePath( QStringLiteral( "https://sandbox.vxstats.com/" ) );

  Statistics::instance().page( QStringLiteral( "Start" ) );

  auto timer = new QTimer;
  QObject::connect( timer, &QTimer::timeout, []() {

    Statistics::instance().page( QStringLiteral( "Demo" ) );
  } );
  timer->start( createPageMilliseconds );

#ifdef QT_GUI_LIB
  return QGuiApplication::exec();
#else
  return QCoreApplication::exec();
#endif
}
