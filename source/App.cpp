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
#include <QCoreApplication>

/* local header */
#include "App.h"

namespace VX {

  bool App::fairUse() const {

    return false;
  }

  QString App::identifier() const {

    return m_identifier.isEmpty() ? QCoreApplication::applicationName() : m_identifier;
  }

  void App::setIdentifier( const QString &_identifier ) {

    if ( m_identifier == _identifier ) {

      return;
    }
    m_identifier = _identifier;
  }

  QString App::version() const {

    return m_version.isEmpty() ? QCoreApplication::applicationVersion() : m_version;
  }

  void App::setVersion( const QString &_version ) {

    if ( m_version == _version ) {

      return;
    }
    m_version = _version;
  }

  void App::setBuild( const QString &_build ) {

    if ( m_build == _build ) {

      return;
    }
    m_build = _build;
  }
}
