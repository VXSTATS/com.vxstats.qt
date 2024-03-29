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

/* windows header */
#ifdef _WIN32
  #include <windows.h>
#endif

/* qt header */
#include <QAuthenticator>
#include <QDateTime>
#include <QDebug>
#include <QMetaEnum>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QSettings>
#include <QtGlobal>

#if QT_VERSION >= QT_VERSION_CHECK( 5, 9, 0 )
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

namespace vxstats {

  /** Base length. */
  constexpr int baseLength = 255;

  /** Default field width. */
  constexpr int defaultFieldWidth = 8;

  /** Default base. */
  constexpr int defaultBase = 10;

  Statistics::Statistics( QObject *_parent )
    : QObject( _parent ) {

    m_networkAccessManager = QSharedPointer<QNetworkAccessManager>( new QNetworkAccessManager( this ) );
    m_networkAccessManager->setStrictTransportSecurityEnabled( true );
#if QT_VERSION >= QT_VERSION_CHECK( 5, 11, 0 )
    connect( m_networkAccessManager.get(), &QNetworkAccessManager::authenticationRequired, this, &Statistics::slotAuthenticationRequired );
    connect( m_networkAccessManager.get(), &QNetworkAccessManager::finished, this, &Statistics::slotFinished );
#else
    connect( m_networkAccessManager.data(), &QNetworkAccessManager::authenticationRequired, this, &Statistics::slotAuthenticationRequired );
    connect( m_networkAccessManager.data(), &QNetworkAccessManager::finished, this, &Statistics::slotFinished );
#endif

    auto reachability = QSharedPointer<Reachability>( new Reachability( this ) );
#if QT_VERSION >= QT_VERSION_CHECK( 5, 11, 0 )
    connect( reachability.get(), &Reachability::reachabilityChanged, this, &Statistics::slotReachabilityChanged );
#else
    connect( reachability.data(), &Reachability::reachabilityChanged, this, &Statistics::slotReachabilityChanged );
#endif
#if QT_VERSION >= QT_VERSION_CHECK( 6, 1, 0 )
    if ( QNetworkInformation::instance() ) {

      reachability->slotUpdateReachability( QNetworkInformation::instance()->reachability() );
    }
#else
    reachability->slotUpdateReachability();
#endif
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

    Device::instance().initialize();
    /* device block */
    core.addQueryItem( QStringLiteral( "uuid" ), Device::instance().uniqueIdentifier() );
#if QT_VERSION >= QT_VERSION_CHECK( 5, 9, 0 )
    const QOperatingSystemVersion current = QOperatingSystemVersion::current();
    QString operatingSystem = current.name();
    if ( current.type() == QOperatingSystemVersion::IOS || current.type() == QOperatingSystemVersion::MacOS || current.type() == QOperatingSystemVersion::TvOS || current.type() == QOperatingSystemVersion::WatchOS ) {

      operatingSystem[ 0 ] = operatingSystem[ 0 ].toLower();
    }
    else if ( current.type() == QOperatingSystemVersion::Unknown ) {

      operatingSystem = QSysInfo::productType();
      operatingSystem[ 0 ] = operatingSystem[ 0 ].toUpper();
    }
    core.addQueryItem( QStringLiteral( "os" ), operatingSystem );
#else
#ifdef Q_OS_OSX
    core.addQueryItem( QStringLiteral( "os" ), QStringLiteral( "macOS" ) );
#elif defined Q_OS_IOS
    core.addQueryItem( QStringLiteral( "os" ), QStringLiteral( "iOS" ) );
#else
    QString operatingSystem = QSysInfo::productType();
    operatingSystem[ 0 ] = operatingSystem[ 0 ].toUpper();
    core.addQueryItem( QStringLiteral( "os" ), operatingSystem );
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
    const QStringList locale = QLocale::system().name().split( '_' );
    core.addQueryItem( QStringLiteral( "language" ), locale.at( 0 ) );
    core.addQueryItem( QStringLiteral( "country" ), locale.at( 1 ) );

    /* connection - 'Bluetooth','Ethernet','Offline','Unknown','Wifi','WWAN' */
    if ( m_connection != Device::Connection::Unknown ) {

      const QMetaObject &metaObjectConnection = Device::staticMetaObject;
      const int index = metaObjectConnection.indexOfEnumerator( "Connection" );
      const QMetaEnum metaEnumConnection = metaObjectConnection.enumerator( index );

      core.addQueryItem( QStringLiteral( "connection" ), QString::fromLatin1( metaEnumConnection.valueToKey( static_cast<int>( m_connection ) ) ) );
    }

    /* radio - 'None','GPRS','Edge','WCDMA','HSDPA','HSUPA','CDMA1x','CDMAEVDORev0','CDMAEVDORevA','CDMAEVDORevB','HRPD','LTE','2G','3G','4G','5G','WiMAX' */
    const QMetaObject &etaObjectRadio = Device::staticMetaObject;
    const int index = etaObjectRadio.indexOfEnumerator( "Radio" );
    const QMetaEnum metaEnumRadio = etaObjectRadio.enumerator( index );

    QString radio = QString::fromLatin1( metaEnumRadio.valueToKey( static_cast<int>( m_radio ) ) );
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
    /* is this device uses dark mode? */
    if ( Device::instance().useDarkMode() ) {

      core.addQueryItem( QStringLiteral( "dark" ), QStringLiteral( "1" ) );
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
    const QScreen *screen = QGuiApplication::primaryScreen();
    /* Screen Resolution */
    core.addQueryItem( QStringLiteral( "width" ), QStringLiteral( "%1" ).arg( screen->size().width() ) );
    core.addQueryItem( QStringLiteral( "height" ), QStringLiteral( "%1" ).arg( screen->size().height() ) );

#ifdef _WIN32
    /* NOTE: This is last value, without not logged out! See known problems. */
    HDC deviceContext = GetDC( 0 );
    int dpiX = GetDeviceCaps( deviceContext, LOGPIXELSX );
    ReleaseDC( 0, deviceContext );
    const double ratio = dpiX / 96.0;
#else
    const double ratio = screen->devicePixelRatio();
#endif
    /* Device Pixel Ratio - dpr */
    if ( ratio != 1.0 ) {

      core.addQueryItem( QStringLiteral( "dpr" ), QString( QStringLiteral( "%1" ) ).arg( ratio ) );
    }
#endif

    /* created time block */
#if QT_VERSION >= QT_VERSION_CHECK( 5, 8, 0 )
    core.addQueryItem( QStringLiteral( "created" ), QString( QStringLiteral( "%1" ) ).arg( QDateTime::currentDateTimeUtc().toSecsSinceEpoch() ) );
#else
    core.addQueryItem( QStringLiteral( "created" ), QString( QStringLiteral( "%1" ) ).arg( QDateTime::currentDateTimeUtc().toTime_t() ) );
#endif
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

      qDebug() << Q_FUNC_INFO << __LINE__ << "Bad implementation - 'serverFilePath' is empty - using: 'https://sandbox.vxstats.com'";
      m_serverFilePath = QStringLiteral( "https://sandbox.vxstats.com" );
    }

#ifdef DEBUG
    qDebug() << m_serverFilePath << _message.query( QUrl::FullyEncoded ).toUtf8();
#endif

    QSettings settings;
    settings.beginGroup( QStringLiteral( "statistics" ) );
    if ( settings.value( QStringLiteral( "tracking" ), true ).toBool() ) {

      m_lastMessage = _message;

      QNetworkRequest request( m_serverFilePath );
#if QT_VERSION >= QT_VERSION_CHECK( 5, 15, 0 )
      request.setAttribute( QNetworkRequest::Http2AllowedAttribute, true );
#else
      request.setAttribute( QNetworkRequest::HTTP2AllowedAttribute, true );
#endif
      request.setHeader( QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded" );

      /* we need to make a challenge first */
      if ( !m_realm.isEmpty() && !m_domain.isEmpty() && !m_nonce.isEmpty() && !m_cnonce.isEmpty() ) {

        m_cnonce = QString::fromLatin1( QCryptographicHash::hash( QString( QStringLiteral( "%1" ) ).arg( m_cnonce ).toUtf8(), QCryptographicHash::Md5 ).toHex() );

        const QByteArray hash1 = QCryptographicHash::hash( QString( QStringLiteral( "%1:%2:%3" ) ).arg( m_username, m_realm, m_password ).toUtf8(), QCryptographicHash::Md5 ).toHex();
        const QByteArray hash2 = QCryptographicHash::hash( QString( QStringLiteral( "POST:%1" ) ).arg( m_domain ).toUtf8(), QCryptographicHash::Md5 ).toHex();
        const QByteArray response = QCryptographicHash::hash( QString( QStringLiteral( "%1:%2:%3:%4:%5:%6" ) ).arg( hash1, m_nonce ).arg( m_requestCounter, defaultFieldWidth, defaultBase, QLatin1Char( '0' ) ).arg( m_cnonce, "auth", hash2 ).toUtf8(), QCryptographicHash::Md5 ).toHex();

        const QString digest = QString( QStringLiteral( "Digest username=\"%1\", realm=\"%2\", nonce=\"%3\", uri=\"%4\", response=\"%5\", algorithm=MD5, qop=auth, nc=%6, cnonce=\"%7\"" ) ).arg( m_username, m_realm, m_nonce, m_domain, response ).arg( m_requestCounter, defaultFieldWidth, defaultBase, QLatin1Char( '0' ) ).arg( m_cnonce );
        request.setRawHeader( "Authorization", digest.toUtf8() );

        m_requestCounter++;
      }

      m_networkAccessManager->post( request, _message.query( QUrl::FullyEncoded ).toUtf8() );
    }
    else {

      addOutstandingMessage( _message );
    }
  }

  void Statistics::addOutstandingMessage( const QUrlQuery &_message ) const {

#if defined Q_OS_OSX || defined Q_OS_IOS
    Device::instance().addOutstandingMessage( QString::fromUtf8( _message.query( QUrl::FullyEncoded ).toUtf8() ) );
#else
    QSettings settings( QStringLiteral( "group.com.vxstats.statistics" ) );
    QStringList messages = settings.value( QStringLiteral( "offline" ) ).toStringList();
    messages.append( QString::fromUtf8( _message.query( QUrl::FullyEncoded ).toUtf8() ) );
    settings.setValue( QStringLiteral( "offline" ), messages );
    settings.sync();
#endif
  }

  void Statistics::sendOutstandingMessages() {

#if defined Q_OS_OSX || defined Q_OS_IOS
    QVector<QString> messages = Device::instance().sendOutstandingMessages();
#else
    QSettings settings( QStringLiteral( "group.com.vxstats.statistics" ) );
    const QStringList messages = settings.value( QStringLiteral( "offline" ) ).toStringList();
    settings.remove( QStringLiteral( "offline" ) );
    settings.sync();
#endif
    for ( const QString &message : messages ) {

      sendMessage( QUrlQuery( message ) );
    }
  }

  void Statistics::slotFinished( const QNetworkReply *_reply ) {

    if ( _reply->error() == QNetworkReply::NoError ) {

#if QT_VERSION < QT_VERSION_CHECK( 6, 0, 0 )
#ifdef DEBUG
      if ( _reply->hasRawHeader( "Authentication-Info" ) ) {

        QString authSession = QString::fromLatin1( _reply->rawHeader( "Authentication-Info" ) );
        authSession.remove( QStringLiteral( " " ) );
        authSession.remove( QStringLiteral( "\"" ) );

        QUrlQuery authData;
        authData.setQueryDelimiters( '=', ',' );
        authData.setQuery( authSession );

        if ( authData.queryItemValue( QStringLiteral( "nc" ) ).toInt() == m_requestCounter - 1 ) {

          qDebug() << "Request vaild:" << m_requestCounter;
        }
        else {

          qDebug() << "ERROR: Request NOT vaild:" << m_requestCounter;
        }
        if ( authData.queryItemValue( QStringLiteral( "cnonce" ) ) == m_cnonce ) {

          qDebug() << "Nonce vaild:" << m_cnonce;
        }
        else {

          qDebug() << "ERROR: Nonce NOT vaild:" << m_cnonce;
        }
        const QByteArray hash1 = QCryptographicHash::hash( QString( QStringLiteral( "%1:%2:%3" ) ).arg( m_username, m_realm, m_password ).toUtf8(), QCryptographicHash::Md5 ).toHex();
        const QByteArray hash2 = QCryptographicHash::hash( QString( QStringLiteral( ":%1" ) ).arg( m_domain ).toUtf8(), QCryptographicHash::Md5 ).toHex();
        const QString response = QString::fromLatin1( QCryptographicHash::hash( QString( QStringLiteral( "%1:%2:%3:%4:%5:%6" ) ).arg( hash1, m_nonce ).arg( m_requestCounter - 1, defaultFieldWidth, defaultBase, QLatin1Char( '0' ) ).arg( m_cnonce, "auth", hash2 ).toUtf8(), QCryptographicHash::Md5 ).toHex() );
        if ( authData.queryItemValue( QStringLiteral( "rspauth" ) ) == response ) {

          qDebug() << "Response is vailid:" << response;
        }
        else {

          qDebug() << "ERROR: Response NOT vailid:" << response;
        }
      }
#endif
#endif
      return;
    }

#ifdef DEBUG
    qDebug() << Q_FUNC_INFO << __LINE__ << "Network Error:" << _reply->error();
#endif

    /* Digest Challenge */
    if ( _reply->error() == QNetworkReply::AuthenticationRequiredError && _reply->hasRawHeader( "WWW-Authenticate" ) ) {

      const QString digestPrefix( QStringLiteral( "Digest" ) );
      QString authSession = QString::fromLatin1( _reply->rawHeader( "WWW-Authenticate" ) );
      if ( authSession.startsWith( digestPrefix ) ) {

        m_requestCounter = 1;

        authSession = authSession.right( authSession.size() - digestPrefix.length() );
        authSession.remove( QStringLiteral( " " ) );
        authSession.remove( QStringLiteral( "\"" ) );

        QUrlQuery authData;
        authData.setQueryDelimiters( '=', ',' );
        authData.setQuery( authSession );

        m_realm = authData.queryItemValue( QStringLiteral( "realm" ) );
        m_domain = authData.queryItemValue( QStringLiteral( "domain" ) );
        m_nonce = authData.queryItemValue( QStringLiteral( "nonce" ) );
        m_cnonce = QString::fromLatin1( QCryptographicHash::hash( QString( QStringLiteral( "%1" ) ).arg( m_nonce ).toUtf8(), QCryptographicHash::Md5 ).toHex() );

        const QByteArray hash1 = QCryptographicHash::hash( QString( QStringLiteral( "%1:%2:%3" ) ).arg( m_username, m_realm, m_password ).toUtf8(), QCryptographicHash::Md5 ).toHex();
        const QByteArray hash2 = QCryptographicHash::hash( QString( QStringLiteral( "POST:%1" ) ).arg( m_domain ).toUtf8(), QCryptographicHash::Md5 ).toHex();
        const QByteArray response = QCryptographicHash::hash( QString( QStringLiteral( "%1:%2:%3:%4:%5:%6" ) ).arg( hash1, m_nonce ).arg( m_requestCounter, defaultFieldWidth, defaultBase, QLatin1Char( '0' ) ).arg( m_cnonce, QStringLiteral( "auth" ), hash2 ).toUtf8(), QCryptographicHash::Md5 ).toHex();

        const QString digest = QString( QStringLiteral( "Digest username=\"%1\", realm=\"%2\", nonce=\"%3\", uri=\"%4\", response=\"%5\", algorithm=MD5, qop=auth, nc=%6, cnonce=\"%7\"" ) ).arg( m_username, m_realm, m_nonce, m_domain, response ).arg( m_requestCounter, defaultFieldWidth, defaultBase, QLatin1Char( '0' ) ).arg( m_cnonce );

        QNetworkRequest request( m_serverFilePath );
#if QT_VERSION >= QT_VERSION_CHECK( 5, 15, 0 )
        request.setAttribute( QNetworkRequest::Http2AllowedAttribute, true );
#else
        request.setAttribute( QNetworkRequest::HTTP2AllowedAttribute, true );
#endif
        request.setHeader( QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded" );
        request.setRawHeader( "Authorization", digest.toUtf8() );

        m_networkAccessManager->post( request, m_lastMessage.query( QUrl::FullyEncoded ).toUtf8() );

        m_requestCounter++;
        return;
      }
    }
    addOutstandingMessage( m_lastMessage );
  }

  void Statistics::slotAuthenticationRequired( [[maybe_unused]] const QNetworkReply *_reply,
                                               QAuthenticator *_authenticator ) const {

    if ( !m_username.isEmpty() && !m_password.isEmpty() ) {

      _authenticator->setUser( m_username );
      _authenticator->setPassword( m_password );
    }
  }

  void Statistics::slotReachabilityChanged( Device::Connection _connection,
                                            Device::Radio _radio ) {

    m_radio = _radio;
    m_connection = _connection;

#ifdef DEBUG
    const QMetaObject &metaObject = Device::staticMetaObject;
    const int indexRadio = metaObject.indexOfEnumerator( "Radio" );
    const QMetaEnum radioEnum = metaObject.enumerator( indexRadio );
    const int indexConnection = metaObject.indexOfEnumerator( "Connection" );
    const QMetaEnum connectionEnum = metaObject.enumerator( indexConnection );
    qDebug() << Q_FUNC_INFO << __LINE__ << "Network Changed" << connectionEnum.valueToKey( static_cast<int>( m_connection ) ) << radioEnum.valueToKey( static_cast<int>( m_radio ) );
#endif
  }
}
