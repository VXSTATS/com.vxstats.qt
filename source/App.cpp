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
