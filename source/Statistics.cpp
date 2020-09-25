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

/* windows header */
#ifdef _WIN32
  #include <windows.h>
#endif

/* qt header */
#include <QDateTime>
#include <QDebug>
#include <QMetaEnum>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QNetworkSession>
#include <QSettings>
#include <QtGlobal>

#if QT_VERSION >= 0x050900
  #include <QOperatingSystemVersion>
#endif

#ifdef QT_GUI_LIB
  #include <QAccessible>
  #include <QGuiApplication>
  #include <QScreen>
#endif

/* local header */
#include "App.h"
#include "Reachability.h"
#include "Statistics.h"

namespace VX {

  constexpr int baseLength = 255;

  Statistics::Statistics( QObject *_parent )
    : QObject( _parent ) {

    m_networkAccessManager = new QNetworkAccessManager( this );
    connect( m_networkAccessManager, &QNetworkAccessManager::finished, this, &Statistics::slotFinished );

    Reachability *reachability = new Reachability( this );
    connect( reachability, &Reachability::reachabilityChanged, this, &Statistics::slotReachabilityChanged );
    reachability->slotUpdateReachability();
  }

  Statistics &Statistics::instance() {

    static Statistics statistics;
    return statistics;
  }

  void Statistics::setServerFilePath( const QString &_serverFilePath ) {

    if ( m_serverFilePath == _serverFilePath ) {

      return;
    }
    m_serverFilePath = _serverFilePath;
  }

  void Statistics::setUsername( const QString &_username ) {

    if ( m_username == _username ) {

      return;
    }
    m_username = _username;
  }

  void Statistics::setPassword( const QString &_password ) {

    if ( m_password == _password ) {

      return;
    }
    m_password = _password;
  }

  void Statistics::page( const QString &_page ) {

    QString page = _page;
    if ( page.isEmpty() ) {

      qDebug() << Q_FUNC_INFO << __LINE__ << "Bad implementation - page with empty 'page'";
      return;
    }

    if ( page.length() > baseLength ) {

      qDebug() << Q_FUNC_INFO << __LINE__ << "Bad implementation - 'page':" << page << "is larger than" << baseLength << "signs";
      page = page.left( baseLength );
    }
    m_lastPage = page;
    event( {} );
  }

  void Statistics::event( const QString &_event, const QString &_value ) {

    if ( m_lastPage.isEmpty() ) {

      qDebug() << Q_FUNC_INFO << __LINE__ << "Bad implementation - 'event':" << _event << "with empty 'page'";
    }

    QString event = _event;
    if ( event.length() > baseLength ) {

      qDebug() << Q_FUNC_INFO << __LINE__ << "Bad implementation - 'event':" << event << "is larger than" << baseLength << "signs";
      event = event.left( baseLength );
    }

    QString value = _value;
    if ( value.length() > baseLength ) {

      qDebug() << Q_FUNC_INFO << __LINE__ << "Bad implementation - 'value':" << value << "is larger than" << baseLength << "signs";
      value = value.left( baseLength );
    }

    QUrlQuery message = coreMessage();
    if ( !event.isEmpty() ) {

      message.addQueryItem( QStringLiteral( "action" ), event );
    }
    if ( !value.isEmpty() ) {

      message.addQueryItem( QStringLiteral( "value" ), value );
    }
    sendMessage( message );
  }

  void Statistics::ads( const QString &_campaign ) {

    QString campaign = _campaign;
    if ( campaign.isEmpty() ) {

      qDebug() << Q_FUNC_INFO << __LINE__ << "Bad implementation - 'ads' with empty 'campaign' name, 'page':" << m_lastPage;
    }
    else if ( campaign.length() > baseLength ) {

      qDebug() << Q_FUNC_INFO << __LINE__ << "Bad implementation - 'campaign':" << campaign << "is larger than" << baseLength << "signs";
      campaign = campaign.left( baseLength );
    }
    event( QStringLiteral( "ads" ), campaign );
  }

  void Statistics::move( double _latitude, double _longitude ) {

    if ( _latitude == 0.0 || _longitude == 0.0 ) {

      qDebug() << Q_FUNC_INFO << __LINE__ << "Bad implementation - 'move' with empty 'latitude' or 'longitude'";
    }
    event( QStringLiteral( "move" ), QString( QStringLiteral( "%1,%2" ) ).arg( _latitude ).arg( _longitude ) );
  }

  void Statistics::open( const QString &_urlOrName ) {

    QString urlOrName = _urlOrName;
    if ( urlOrName.isEmpty() ) {

      qDebug() << Q_FUNC_INFO << __LINE__ << "Bad implementation - 'open' with empty 'urlOrName', 'page':" << m_lastPage;
    }
    else if ( urlOrName.length() > baseLength ) {

      qDebug() << Q_FUNC_INFO << __LINE__ << "Bad implementation - 'urlOrName':" << urlOrName << "is larger than" << baseLength << "signs";
      urlOrName = urlOrName.left( baseLength );
    }
    event( QStringLiteral( "open" ), urlOrName );
  }

  void Statistics::play( const QString &_urlOrName ) {

    QString urlOrName = _urlOrName;
    if ( urlOrName.isEmpty() ) {

      qDebug() << Q_FUNC_INFO << __LINE__ << "Bad implementation - 'play' with empty 'urlOrName', 'page':" << m_lastPage;
    }
    else if ( urlOrName.length() > baseLength ) {

      qDebug() << Q_FUNC_INFO << __LINE__ << "Bad implementation - 'urlOrName':" << urlOrName << "is larger than" << baseLength << "signs";
      urlOrName = urlOrName.left( baseLength );
    }
    event( QStringLiteral( "play" ), urlOrName );
  }

  void Statistics::search( const QString &_text ) {

    QString text = _text;
    if ( text.isEmpty() ) {

      qDebug() << Q_FUNC_INFO << __LINE__ << "Bad implementation - 'search' with empty 'text', 'page':" << m_lastPage;
    }
    else if ( text.length() > baseLength ) {

      qDebug() << Q_FUNC_INFO << __LINE__ << "Bad implementation - 'text':" << text << "is larger than" << baseLength << "signs";
      text = text.left( baseLength );
    }
    event( QStringLiteral( "search" ), text );
  }

  void Statistics::shake() { event( QStringLiteral( "shake" ) ); }

  void Statistics::touch( const QString &_action ) {

    QString action = _action;
    if ( action.isEmpty() ) {

      qDebug() << Q_FUNC_INFO << __LINE__ << "Bad implementation - 'touch' with empty 'action', 'page':" << m_lastPage;
    }
    else if ( action.length() > baseLength ) {

      qDebug() << Q_FUNC_INFO << __LINE__ << "Bad implementation - 'action':" << action << "is larger than" << baseLength << "signs";
      action = action.left( baseLength );
    }
    event( QStringLiteral( "touch" ), action );
  }

  QUrlQuery Statistics::coreMessage() const {

    QUrlQuery core;

    /* device block */
    core.addQueryItem( QStringLiteral( "uuid" ), Device::instance().uniqueIdentifier() );
#if QT_VERSION >= 0x050900
    QOperatingSystemVersion current = QOperatingSystemVersion::current();
    QString os = current.name();
    if ( current.type() == QOperatingSystemVersion::IOS || current.type() == QOperatingSystemVersion::MacOS || current.type() == QOperatingSystemVersion::TvOS || current.type() == QOperatingSystemVersion::WatchOS ) {

      os[ 0 ] = os[ 0 ].toLower();
    }
    else if ( current.type() == QOperatingSystemVersion::Unknown ) {

      os = QSysInfo::productType();
      os[ 0 ] = os[ 0 ].toUpper();
    }
    core.addQueryItem( QStringLiteral( "os" ), os );
#else
#ifdef Q_OS_OSX
    core.addQueryItem( QStringLiteral( "os" ), QStringLiteral( "macOS" ) );
#elif defined Q_OS_IOS
    core.addQueryItem( QStringLiteral( "os" ), QStringLiteral( "iOS" ) );
#else
    QString os = QSysInfo::productType();
    os[ 0 ] = os[ 0 ].toUpper();
    core.addQueryItem( QStringLiteral( "os" ), os );
#endif
#endif
    core.addQueryItem( QStringLiteral( "osversion" ), Device::instance().osVersion() );
    if ( !Device::instance().model().isEmpty() ) {

      core.addQueryItem( QStringLiteral( "model" ), Device::instance().model() );
    }
    if ( !Device::instance().version().isEmpty() ) {

      core.addQueryItem( QStringLiteral( "modelversion" ), Device::instance().version() );
    }
    if ( !Device::instance().vendor().isEmpty() ) {

      core.addQueryItem( QStringLiteral( "vendor" ), Device::instance().vendor() );
    }

    /* locale */
    QStringList locale = QLocale::system().name().split( '_' );
    core.addQueryItem( QStringLiteral( "language" ), locale.at( 0 ) );
    core.addQueryItem( QStringLiteral( "country" ), locale.at( 1 ) );

    /* connection - 'Bluetooth','Ethernet','Offline','Unknown','Wifi','WWAN' */
    if ( m_connection != Device::Connection::Unknown ) {

      const QMetaObject &mo = Device::staticMetaObject;
      int index = mo.indexOfEnumerator( "Connection" );
      QMetaEnum metaEnum = mo.enumerator( index );

      core.addQueryItem( QStringLiteral( "connection" ), QString::fromLatin1( metaEnum.valueToKey( static_cast<int>( m_connection ) ) ) );
    }

    /* radio - 'None','GPRS','Edge','WCDMA','HSDPA','HSUPA','CDMA1x','CDMAEVDORev0','CDMAEVDORevA','CDMAEVDORevB','HRPD','LTE','2G','3G','4G','5G','WiMAX' */
    const QMetaObject &mo = Device::staticMetaObject;
    int index = mo.indexOfEnumerator( "Radio" );
    QMetaEnum metaEnum = mo.enumerator( index );

    QString radio = QString::fromLatin1( metaEnum.valueToKey( static_cast<int>( m_radio ) ) );
    radio.remove( QStringLiteral( "Radio" ) );
    if ( m_radio != Device::Radio::None ) {

      core.addQueryItem( QStringLiteral( "radio" ), radio );
    }

    /* app block */
    core.addQueryItem( QStringLiteral( "appid" ), App::instance().identifier() );
    core.addQueryItem( QStringLiteral( "appversion" ), App::instance().version() );
    if ( !App::instance().build().isEmpty() ) {

      core.addQueryItem( QStringLiteral( "appbuild" ), App::instance().build() );
    }
    /* is this app fairly used? */
    if ( App::instance().fairUse() ) {

      core.addQueryItem( QStringLiteral( "fair" ), QStringLiteral( "1" ) );
    }
    /* is the device jailbroken? */
    if ( Device::instance().isJailbroken() ) {

      core.addQueryItem( QStringLiteral( "free" ), QStringLiteral( "1" ) );
    }
    /* is the device in tabletmode */
    if ( Device::instance().isTabletMode() ) {

      core.addQueryItem( QStringLiteral( "tabletmode" ), QStringLiteral( "1" ) );
    }
    /* is the device with touch screen */
    if ( Device::instance().hasTouchScreen() ) {

      core.addQueryItem( QStringLiteral( "touch" ), QStringLiteral( "1" ) );
    }
    /* does the user use voiceover? */
    bool accessibleIsActive = false;
#ifdef QT_GUI_LIB
    accessibleIsActive = QAccessible::isActive();
#endif
    if ( accessibleIsActive || Device::instance().isVoiceOverActive() ) {

      core.addQueryItem( QStringLiteral( "voiceover" ), QStringLiteral( "1" ) );
    }

#ifdef QT_GUI_LIB
    QScreen *screen = QGuiApplication::primaryScreen();
    /* Screen Resolution */
    core.addQueryItem( QStringLiteral( "width" ), QStringLiteral( "%1" ).arg( screen->size().width() ) );
    core.addQueryItem( QStringLiteral( "height" ), QStringLiteral( "%1" ).arg( screen->size().height() ) );

#ifdef _WIN32
    /* NOTE: This is last value, without not logged out! See known problems. */
    HDC deviceContext = GetDC( 0 );
    int dpiX = GetDeviceCaps( deviceContext, LOGPIXELSX );
    int dpiY = GetDeviceCaps( deviceContext, LOGPIXELSY );
    Q_UNUSED( dpiY );
    ReleaseDC( 0, deviceContext );
    double ratio = dpiX / 96.0;
#else
    double ratio = screen->devicePixelRatio();
#endif
    /* Device Pixel Ratio - dpr */
    if ( ratio != 1.0 ) {

      core.addQueryItem( QStringLiteral( "dpr" ), QString( QStringLiteral( "%1" ) ).arg( ratio ) );
    }
#endif

    /* created time block */
    core.addQueryItem( QStringLiteral( "created" ), QString( QStringLiteral( "%1" ) ).arg( QDateTime::currentDateTimeUtc().toTime_t() ) );
    /* data block */
    core.addQueryItem( QStringLiteral( "page" ), m_lastPage );
    return core;
  }

  void Statistics::sendMessage( const QUrlQuery &_message ) {

    if ( _message.isEmpty() ) {

      qDebug() << Q_FUNC_INFO << __LINE__ << "Bad implementation - 'message' is empty";
      return;
    }

    if ( m_serverFilePath.isEmpty() ) {

      qDebug() << Q_FUNC_INFO << __LINE__ << "Bad implementation - 'serverFilePath' is empty - using: 'https://sandbox.vxapps.com'";
      m_serverFilePath = "https://sandbox.vxapps.com";
    }

#ifdef DEBUG
    qDebug() << m_serverFilePath << _message.query( QUrl::FullyEncoded ).toUtf8();
#endif

    QSettings settings;
    settings.beginGroup( QStringLiteral( "statistics" ) );
    if ( settings.value( QStringLiteral( "tracking" ), true ).toBool() ) {

      m_lastMessage = _message;

      QNetworkRequest request( m_serverFilePath );
      request.setHeader( QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded" );

      /* we need to make a challenge first */
      if ( !m_realm.isEmpty() && !m_domain.isEmpty() && !m_nonce.isEmpty() && !m_cnonce.isEmpty() ) {

        m_cnonce = QCryptographicHash::hash( QString( "%1" ).arg( m_cnonce ).toUtf8(), QCryptographicHash::Md5 ).toHex();

        QString hash1 = QCryptographicHash::hash( QString( "%1:%2:%3" ).arg( m_username ).arg( m_realm ).arg( m_password ).toUtf8(), QCryptographicHash::Md5 ).toHex();
        QString hash2 = QCryptographicHash::hash( QString( "POST:%1" ).arg( m_domain ).toUtf8(), QCryptographicHash::Md5 ).toHex();
        QString response = QCryptographicHash::hash( QString( "%1:%2:%3:%4:%5:%6" ).arg( hash1 ).arg( m_nonce ).arg( m_requestCounter, 8, 10, QLatin1Char( '0' ) ).arg( m_cnonce ).arg( "auth" ).arg( hash2 ).toUtf8(), QCryptographicHash::Md5 ).toHex();

        QString digest = QString( "Digest username=\"%1\", realm=\"%2\", nonce=\"%3\", uri=\"%4\", response=\"%5\", algorithm=MD5, qop=auth, nc=%6, cnonce=\"%7\"" ).arg( m_username ).arg( m_realm ).arg( m_nonce ).arg( m_domain ).arg( response ).arg( m_requestCounter, 8, 10, QLatin1Char( '0' ) ).arg( m_cnonce );
        qDebug() << "Digest direct " << digest;

        request.setRawHeader( "Authorization", digest.toUtf8() );

        m_requestCounter++;
      }
      m_networkAccessManager->post( request, _message.query( QUrl::FullyEncoded ).toUtf8() );
    }
    else {

      addOutstandingMessage( _message );
    }
  }

  void Statistics::addOutstandingMessage( const QUrlQuery &_message ) {

    QSettings settings( QStringLiteral( "com.vxstats" ), QStringLiteral( "statistics" ) );
    settings.beginGroup( QStringLiteral( "statistics" ) );
    QStringList messages = settings.value( QStringLiteral( "offline" ) ).toStringList();
    messages.append( QString::fromUtf8( _message.query( QUrl::FullyEncoded ).toUtf8() ) );
    settings.setValue( QStringLiteral( "offline" ), messages );
    settings.sync();
  }

  void Statistics::sendOutstandingMessages() {

    QSettings settings( QStringLiteral( "com.vxstats" ), QStringLiteral( "statistics" ) );
    settings.beginGroup( QStringLiteral( "statistics" ) );
    const QStringList messages = settings.value( QStringLiteral( "offline" ) ).toStringList();
    settings.remove( QStringLiteral( "offline" ) );
    settings.sync();
    for ( const QString &message : messages ) {

      sendMessage( QUrlQuery( message ) );
    }
  }

  void Statistics::slotFinished( QNetworkReply *_reply ) {

    if ( _reply->error() == QNetworkReply::NoError ) {

      if ( _reply->hasRawHeader( "Authentication-Info" ) ) {

        QString authSession = _reply->rawHeader( "Authentication-Info" );
        qDebug() << "Auth Sueccess: " << authSession;
        authSession.remove( " " );
        authSession.remove( "\"" );

        QUrlQuery authData;
        authData.setQueryDelimiters( '=', ',' );
        authData.setQuery( authSession );

        if ( authData.queryItemValue( "nc" ).toInt() == m_requestCounter - 1 ) {

          qDebug() << "Request vaild:" << m_requestCounter;
        }
        if ( authData.queryItemValue( "cnonce" ) == m_cnonce ) {

          qDebug() << "Nonce vaild:" << m_cnonce;
        }
        QString hash1 = QCryptographicHash::hash( QString( "%1:%2:%3" ).arg( m_username ).arg( m_realm ).arg( m_password ).toUtf8(), QCryptographicHash::Md5 ).toHex();
        QString hash2 = QCryptographicHash::hash( QString( ":%1" ).arg( m_domain ).toUtf8(), QCryptographicHash::Md5 ).toHex();
        QString response = QCryptographicHash::hash( QString( "%1:%2:%3:%4:%5:%6" ).arg( hash1 ).arg( m_nonce ).arg( m_requestCounter - 1, 8, 10, QLatin1Char( '0' ) ).arg( m_cnonce ).arg( "auth" ).arg( hash2 ).toUtf8(), QCryptographicHash::Md5 ).toHex();
        if ( authData.queryItemValue( "rspauth" ) == response ) {

          qDebug() << "Response is vailid:" << response;
        }
      }
      return;
    }

#ifdef DEBUG
    qDebug() << Q_FUNC_INFO << __LINE__ << "Network Error:" << _reply->error();
#endif

    /* Digest Challenge */
    if ( _reply->error() == QNetworkReply::AuthenticationRequiredError && _reply->hasRawHeader( "WWW-Authenticate" ) ) {

      QString authSession = _reply->rawHeader( "WWW-Authenticate" );
      qDebug() << "Auth: " << authSession;

      if ( authSession.startsWith( "Digest" ) ) {

        m_requestCounter = 1;

        authSession = authSession.right( authSession.size() - 6 );
        authSession.remove( " " );
        authSession.remove( "\"" );

        QUrlQuery authData;
        authData.setQueryDelimiters( '=', ',' );
        authData.setQuery( authSession );

        m_realm = authData.queryItemValue( "realm" );
        m_domain = authData.queryItemValue( "domain" );
        m_nonce = authData.queryItemValue( "nonce" );
        m_cnonce = QCryptographicHash::hash( QString( "%1" ).arg( m_nonce ).toUtf8(), QCryptographicHash::Md5 ).toHex();

        QString hash1 = QCryptographicHash::hash( QString( "%1:%2:%3" ).arg( m_username ).arg( m_realm ).arg( m_password ).toUtf8(), QCryptographicHash::Md5 ).toHex();
        QString hash2 = QCryptographicHash::hash( QString( "POST:%1" ).arg( m_domain ).toUtf8(), QCryptographicHash::Md5 ).toHex();
        QString response = QCryptographicHash::hash( QString( "%1:%2:%3:%4:%5:%6" ).arg( hash1 ).arg( m_nonce ).arg( m_requestCounter, 8, 10, QLatin1Char( '0' ) ).arg( m_cnonce ).arg( "auth" ).arg( hash2 ).toUtf8(), QCryptographicHash::Md5 ).toHex();

        QString digest = QString( "Digest username=\"%1\", realm=\"%2\", nonce=\"%3\", uri=\"%4\", response=\"%5\", algorithm=MD5, qop=auth, nc=%6, cnonce=\"%7\"" ).arg( m_username ).arg( m_realm ).arg( m_nonce ).arg( m_domain ).arg( response ).arg( m_requestCounter, 8, 10, QLatin1Char( '0' ) ).arg( m_cnonce );
        qDebug() << "Digest" << digest;

        QNetworkRequest request( m_serverFilePath );
        request.setHeader( QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded" );
        request.setRawHeader( "Authorization", digest.toUtf8() );

        m_networkAccessManager->post( request, m_lastMessage.query( QUrl::FullyEncoded ).toUtf8() );

        m_requestCounter++;
        return;
      }
    }
    addOutstandingMessage( m_lastMessage );
  }

  void Statistics::slotReachabilityChanged( Device::Connection _connection,
                                            Device::Radio _radio ) {

    m_radio = _radio;
    m_connection = _connection;

#ifdef DEBUG
    const QMetaObject &metaObject = Device::staticMetaObject;
    int indexRadio = metaObject.indexOfEnumerator( "Radio" );
    QMetaEnum radioEnum = metaObject.enumerator( indexRadio );
    int indexConnection = metaObject.indexOfEnumerator( "Connection" );
    QMetaEnum connectionEnum = metaObject.enumerator( indexConnection );
    qDebug() << Q_FUNC_INFO << __LINE__ << "Network Changed" << connectionEnum.valueToKey( static_cast<int>( m_connection ) ) << radioEnum.valueToKey( static_cast<int>( m_radio ) );
#endif
  }
}
