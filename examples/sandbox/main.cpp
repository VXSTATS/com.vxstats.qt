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
using vxstats::App;
using vxstats::Statistics;

constexpr int createPageMilliseconds = 1000;

int main( int argc, char *argv[] ) {

#ifdef QT_GUI_LIB
  const QGuiApplication app( argc, argv );
#else
  const QCoreApplication app( argc, argv );
#endif
  QCoreApplication::setApplicationName( QStringLiteral( "com.vxstats.sandbox" ) );
  QCoreApplication::setApplicationVersion( QStringLiteral( "1.0" ) );

  App::instance().setBuild( QStringLiteral( "100" ) );

  Statistics::instance().setUsername( QStringLiteral( "sandbox" ) );
  Statistics::instance().setPassword( QStringLiteral( "sandbox" ) );
  Statistics::instance().setServerFilePath( QStringLiteral( "https://sandbox.vxstats.com/" ) );

  Statistics::instance().page( QStringLiteral( "Start" ) );

  auto timer = QSharedPointer<QTimer>( new QTimer );
#if QT_VERSION >= QT_VERSION_CHECK( 5, 11, 0 )
  QObject::connect( timer.get(), &QTimer::timeout, []() {

    Statistics::instance().page( QStringLiteral( "Demo" ) );
  } );
#else
  QObject::connect( timer.data(), &QTimer::timeout, []() {

    Statistics::instance().page( QStringLiteral( "Demo" ) );
  } );
#endif
  timer->start( createPageMilliseconds );

#ifdef QT_GUI_LIB
  return QGuiApplication::exec();
#else
  return QCoreApplication::exec();
#endif
}
