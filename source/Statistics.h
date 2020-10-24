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

#pragma once

/* qt header */
#include <QString>
#include <QObject>
#include <QUrlQuery>

/* local header */
#include "Device.h"

/* qt class */
class QAuthenticator;
class QNetworkAccessManager;
class QNetworkReply;

/**
 * @~english
 * @brief The vxstats namespace.
 *
 * @~german
 * @brief Der vxstats Namensraum.
 */
namespace vxstats {

  /**
   * @~english
   * @brief The Statistics class.
   *
   * @~german
   * @brief Die Klasse Statistics.
   */
  class Statistics : public QObject {

    Q_OBJECT

  public:
    /**
     * @~english
     * @brief Instance of Statistics. C++11 Singleton thread-safe.
     * @return Singleton of Statistics.
     *
     * @~german
     * @brief Instanz von Statistics. C++11 Singleton thread-safe.
     * @return Einzige Instanz von Statistics.
     */
    static Statistics &instance();

    /**
     * @~english
     * @brief Deletes move constructor.
     *
     * @~german
     * @brief Entfernt den verschobenen Konstruktor.
     */
    Statistics( Statistics && ) = delete;

    /**
     * @~english
     * @brief Deletes copy assign.
     * @return Nothing.
     *
     * @~german
     * @brief Entfernt die kopierte Zuweisung.
     * @return Keine Rückgabe.
     */
    Statistics &operator=( Statistics const & ) = delete;

    /**
     * @~english
     * @brief Deletes move assign.
     * @return Nothing.
     *
     * @~german
     * @brief Entfernt die verschobene Zuweisung.
     * @return Keine Rückgabe.
     */
    Statistics &operator=( Statistics && ) = delete;

    /**
     * @~english
     * @brief Defines the path and name to the statistics server.
     * @param _serverFilePath   The file name of the statistics server.
     *
     * @~german
     * @brief Definiert den Pfad und Namen zum Statistikserver.
     * @param _serverFilePath   Der Dateiname zum Statistikserver.
     *
     * @~english @b Example: @~german @b Beispiel:
     * @~english <br>for the HTTPS address sandbox.vxstats.com and the folder /.
     * @~german <br>Für die HTTPS Adresse sandbox.vxstats.com und das Verzeichnis /.
     * @~
     * @code
     * Statistics::instance().setServerFilePath( "https://sandbox.vxtats.com/" );
     * @endcode
     */
    void setServerFilePath( const QString &_serverFilePath );

    /**
     * @~english
     * @brief Defines the username to the statistics server.
     * @param _username   The username to the statistics server.
     *
     * @~german
     * @brief Definiert den Benutzernamen zum Statistikserver.
     * @param _username   Der Benutzername zum Statistikserver.
     *
     * @~english @b Example: @~german @b Beispiel:
     * @~
     * @code
     * Statistics instance().setUsername( "sandbox" );
     * @endcode
     */
    void setUsername( const QString &_username );

    /**
     * @~english
     * @brief Defines the password to the statistics server.
     * @param _password   The password to the statistics server.
     *
     * @~german
     * @brief Definiert das Passwort zum Statistikserver.
     * @param _password   Das Passwoort zum Statistikserver.
     *
     * @~english @b Example: @~german @b Beispiel:
     * @~
     * @code
     * Statistics instance().setPassword( "sandbox" );
     * @endcode
     */
    void setPassword( const QString &_password );

    /**
     * @~english
     * @brief Request a page with the name pageName in order to transfer it to the statistics server.
     * @param _page   The name of the requested page.
     * @note Limited to 255 characters.
     *
     * @~german
     * @brief Aufruf einer Seite mit dem Namen pageName um es an den Statistikserver zu übermitteln.
     * @param _page   Der Name der aufgerufenen Seite.
     * @note Auf 255 Zeichen begrenzt.
     */
    void page( const QString &_page );

    /**
     * @~english
     * @brief If you want to request a page with dynamic content please use this function.
     * @param _event   The event.
     * @param _value   The value for the event.
     *
     * @~german
     * @brief Wenn Sie eine Seite mit dynamischem Inhalt aufrufen möchten, verwenden Sie diese Funktion.
     * @param _event   Das Event.
     * @param _value   Der Wert für das Event.
     *
     * @~english @b Example: @~german @b Beispiel:
     * @~english @n Page with ads. @~german @n Seite mit Werbung.
     * @~
     * @code
     * Statistics::instance().event( "ads", campaign);
     * Statistics::instance().event( "ads", "Apple" );
     * @endcode
     *
     * @see Statistics#ads:
     *
     * @code
     * Statistics::instance().ads( campaign );
     * @endcode
     *
     * @~english @b Example: @~german @b Beispiel:
     * @~english @n Move map to geo position. @~german @n Karte auf Geoposition verschieben.
     * @~
     * @code
     * Statistics::instance().event( "move", latitude,longitude );
     * Statistics::instance().event( "move", "52.523405,13.411400" );
     * @endcode
     *
     * @see Statistics#move:
     *
     * @code
     * Statistics::instance().move( latitude longitude:longitude );
     * @endcode
     *
     * @~english @b Example: @~german @b Beispiel:
     * @~english @n Open browser with URL. @~german @n Browser mit URL öffnen.
     * @~
     * @code
     * Statistics::instance().event( "open", urlOrName );
     * Statistics::instance().event( "open", "https://www.vxstats.com" );
     * @endcode
     *
     * @see Statistics#open:
     *
     * @code
     * Statistics::instance().open( urlOrName );
     * @endcode
     *
     * @~english @b Example: @~german @b Beispiel:
     * @~english @n Play video. @n @~german Video abspielen.
     * @~
     * @code
     * Statistics::instance().event( "play", urlOrName );
     * Statistics::instance().event( "play", "https://www.vxstats.com/movie.m4v" );
     * @endcode
     *
     * @see Statistics#play:
     *
     * @code
     * Statistics::instance().play( urlOrName );
     * @endcode
     *
     * @~english @b Example: @~german @b Beispiel:
     * @~english @n Search for 'asdf'. @~german @n Suchen nach 'asdf'.
     * @~
     * @code
     * Statistics::instance().event( "search", text );
     * Statistics::instance().event( "search", "asdf" );
     * @endcode
     *
     * @see Statistics#search:
     *
     * @code
     * Statistics::instance().search( text );
     * @endcode
     *
     * @~english @b Example: @~german @b Beispiel:
     * @~english @n Shake the device. @~german @n Gerät schütteln.
     * @~
     * @code
     * Statistics::instance().event( "shake", "" );
     * @endcode
     *
     * @see Statistics#shake
     *
     * @code
     * Statistics::instance().shake();
     * @endcode
     *
     * @~english @b Example: @~german @b Beispiel:
     * @~english @n Touch the button for navigation. @~german @n Button für Navitation drücken.
     * @~
     * @code
     * Statistics::instance().event( "touch", action );
     * Statistics::instance().event( "touch", "Navigation" );
     * @endcode
     *
     * @see Statistics#touch:
     *
     * @code
     * Statistics::instance().touch( action );
     * @endcode
     */
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Woverloaded-virtual"
#endif
    void event( const QString &_event, const QString &_value = {} );
#ifdef __clang__
#pragma clang diagnostic pop
#endif

    /**
     * @~english
     * @brief To capture ads - correspondingly the shown ad.
     * @param _campaign   The displayed ad.
     * @note Limited to 255 characters.
     *
     * @~german
     * @brief Für das Erfassen von Werbeeinblendungen - entsprechend die angezeigte Werbung.
     * @param _campaign   Die angezeigte Werbung.
     * @note Auf 255 Zeichen begrenzt.
     *
     * @~
     * @see Statistics#event:withValue:
     *
     * @code
     * Statistics::instance().event( "ads", campaign );
     * @endcode
     */
    void ads( const QString &_campaign );

    /**
     * @~english
     * @brief To capture map shifts - correspondingly the new center.
     * @param _latitude   Latitude of center.
     * @param _longitude   Longitude of center.
     *
     * @~german
     * @brief Für die Erfassung von Kartenverschiebungen - entsprechend das neue Zentrum.
     * @param _latitude   Latitude des Zentrums.
     * @param _longitude   Longitude des Zentrums.
     *
     * @~
     * @see event
     *
     * @code
     * Statistics::instance().event( "move", latitude,longitude);
     * @endcode
     */
    void move( double _latitude, double _longitude );

    /**
     * @~english
     * @brief To capture open websites or documents including the information which page or document has been requested.
     * @param _urlOrName   @~english The displayed website/document.
     * @note Limited to 255 characters.
     *
     * @~german
     * @brief Für das Erfassen von geöffneten Webseiten oder Dokumenten mit der Information, welche Seite bzw.
     * welches Dokument aufgerufen wurde.
     * @param _urlOrName   Die angezeigte Webseite/das angezeigte Dokument.
     * @note Auf 255 Zeichen begrenzt.
     *
     * @~
     * @see event
     *
     * @code
     * Statistics::instance().event( "open", urlOrName );
     * @endcode
     */
    void open( const QString &_urlOrName );

    /**
     * @~english
     * @brief To capture played files including the information which file/action has been played.
     * @param _urlOrName   The played file.
     * @note Limited to 255 characters.
     *
     * @~german
     * @brief Für das Erfassen von abgespielten Dateien mit der Info, welche Datei/Aktion abgespielt wurde.
     * @param _urlOrName   Die abgespielte Datei.
     * @note Auf 255 Zeichen begrentzt.
     *
     * @~
     * @see Statistics#event:withValue:
     *
     * @code
     * Statistics::instance().event( "play", urlOrName );
     * @endcode
     */
    void play( const QString &_urlOrName );

    /**
     * @~english
     * @brief To capture searches including the information for which has been searched.
     * @param _text   The searched text.
     * @note Limited to 255 characters.
     *
     * @~german
     * @brief Für die Erfassung von Suchen mit der Information, nach was gesucht wurde.
     * @param _text   Der gesuchte Text.
     * @note Auf 255 Zeichen begrenzt.
     *
     * @~
     * @see Statistics#event:withValue:
     *
     * @code
     * Statistics::instance().event( "search", text );
     * @endcode
     */
    void search( const QString &_text );

    /**
     * @~english
     * @brief To capture when the device has been shaken.
     *
     * @~german
     * @brief Für das Erfassen, wann das Gerät geschüttelt wurde.
     *
     * @~
     * @see Statistics#event:withValue:
     *
     * @code
     * Statistics::instance().event( "shake", "" );
     * @endcode
     */
    void shake();

    /**
     * @~english
     * @brief To capture typed/touched actions.
     * @param _action   The name of the touched action.
     * @note Limited to 255 characters.
     *
     * @~german
     * @brief Für die Erfassung von getippten/gedrückten Aktionen.
     * @param _action   Der Name der getippten/gedrückten Aktion.
     * @note Auf 255 Zeichen begrenzt.
     *
     * @~
     * @see Statistics#event:withValue:
     *
     * @code
     * Statistics::instance().event( "touch", action );
     * @endcode
     */
    void touch( const QString &_action );

  protected:
    /**
     * @~english
     * @brief Default constructor for Statistics.
     * @param _parent   Parent object.
     *
     * @~german
     * @brief Standardkonstruktor für Statistics.
     * @param _parent   Übergeordnetes Objekt.
     */
    explicit Statistics( QObject *_parent = nullptr );

  private:
    /**
     * @~english
     * @brief Session request counter.
     *
     * @~german
     * @brief Hochzählender Wert der Session.
     */
    int m_requestCounter = 1;

    /**
     * @~english
     * @brief Internal server path to connect to.
     *
     * @~german
     * @brief Interner Serverpfad für das Signalling.
     */
    QString m_serverFilePath = {};

    /**
     * @~english
     * @brief Username for authentication.
     *
     * @~german
     * @brief Benutzername für die Authorisierung.
     */
    QString m_username = {};

    /**
     * @~english
     * @brief Password for authentication.
     *
     * @~german
     * @brief Passwort für die Authorisierung.
     */
    QString m_password = {};

    /**
     * @~english
     * @brief Domain for authentication.
     *
     * @~german
     * @brief Domain für die Authorisierung.
     */
    QString m_domain = {};

    /**
     * @~english
     * @brief Realm for authentication.
     *
     * @~german
     * @brief Realm für die Authorisierung.
     */
    QString m_realm = {};

    /**
     * @~english
     * @brief Nonce for authentication.
     *
     * @~german
     * @brief Nonce für die Authorisierung.
     */
    QString m_nonce = {};

    /**
     * @~english
     * @brief Client nonce for authentication.
     *
     * @~german
     * @brief Lokaler nonce für die Authorisierung.
     */
    QString m_cnonce = {};

    /**
     * @~english
     * @brief Current network status.
     *
     * @~german
     * @brief Aktueller Netzwerkstatus.
     */
    Device::Connection m_connection = Device::Connection::Offline;

    /**
     * @~english
     * @brief Current radio status.
     *
     * @~german
     * @brief Aktueller Status der Funkverbindung.
     */
    Device::Radio m_radio = Device::Radio::None;

    /**
     * @~english
     * @brief Last entered page name.
     *
     * @~german
     * @brief Zuletzt besuchte Seite.
     */
    QString m_lastPage = {};

    /**
     * @~english
     * @brief Last message to send.
     *
     * @~german
     * @brief Zuletzt gesendete Nachricht.
     */
    QUrlQuery m_lastMessage = {};

    /**
     * @~english
     * @brief Network access manager to post the events.
     *
     * @~german
     * @brief Netzwerkverbindungsmanager, um Aktivitäten zu verschicken.
     */
    QNetworkAccessManager *m_networkAccessManager = nullptr;

    /**
     * @~english
     * @brief Defines core information for every block you need to send.
     * @return Core information block to send.
     *
     * @~german
     * @brief Allgemeine Informationen zu jedem speziellen Block.
     * @return Allgemeine Informationen einer zu versendenden Nachricht.
     */
    QUrlQuery coreMessage() const;

    /**
     * @~english
     * @brief Sends the complete message.
     * @param _message   Event to send to the database.
     *
     * @~german
     * @brief Versenden der vollständigen Nachricht.
     * @param _message   Aktivität, um die Nachricht in die Datenbank zu senden.
     */
    void sendMessage( const QUrlQuery &_message );

    /**
     * @~english
     * @brief Adds a pending message, if we cannot send it directly to the
     * server.
     * @param _message   The message to be sent.
     *
     * @~german
     * @brief Hinzufügen von nicht direkt versendbaren Nachrichten, um sie später
     * weiterzusenden.
     * @param _message   Die zu verschickende Nachricht.
     */
    void addOutstandingMessage( const QUrlQuery &_message ) const;

    /**
     * @~english
     * @brief Sends all offline and not directly sent messages.
     *
     * @~german
     * @brief Nachträgliches Versenden aller aktuell zwischengespeicherten
     * Nachrichten.
     */
    void sendOutstandingMessages();

    /**
     * @~english
     * @brief Called when a reply has comfirmed complete receipt, or to add the
     * message to outstanding messages.
     * @param _reply   The received network reply.
     *
     * @~german
     * @brief Wird aufgerufen, wenn Daten nach dem Versand erhalten wurden, um
     * den Erhalt zu bestätigen oder die nicht versendbare Nachricht
     * zwischenzuspeichern.
     * @param _reply   Das erhaltene Ergebnis des Versands.
     */
    void slotFinished( const QNetworkReply *_reply );

    /**
     * @~english
     * @brief Called when a reply want authentication.
     * @param _reply   The received network reply.
     * @param _authenticator   The useable authenticator.
     *
     * @~german
     * @brief Wird aufgerufen, wenn eine Authorisierung verlangt wird.
     * @param _reply   Das erhaltene Ergebnis des Versands.
     * @param _authenticator   Zu nutzender Authentifizierer.
     */
    void slotAuthenticationRequired( const QNetworkReply *_reply,
                                     QAuthenticator *_authenticator ) const;

    /**
     * @~english
     * @brief Any changes to Reachability status. Online/Offline, Status, Radio.
     * @param _connection   New connection type.
     * @param _radio   New radio type.
     *
     * @~german
     * @brief Änderungen im Netzwerk zu jedem Status. Online/Offline, Status,
     * Funk.
     * @param _connection   Neuer Verbindungstyp.
     * @param _radio   Neuer Typ der Funkverbindung.
     */
    void slotReachabilityChanged( Device::Connection _connection,
                                  Device::Radio _radio );
  };
}
