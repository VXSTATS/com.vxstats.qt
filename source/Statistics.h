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

#pragma once

/* qt header */
#include <QString>
#include <QObject>
#include <QUrlQuery>

/* local header */
#include "Device.h"

/* qt class */
class QNetworkAccessManager;
class QNetworkReply;

/**
 * @~english
 * @brief The VX namespace.
 *
 * @~german
 * @brief Der VX Namensbereich.
 */
namespace VX {
  /**
   * @~english
   * @brief The Statistics class.
   *
   * @~german
   * @brief Die Statistics Klasse.
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
     * @brief Delete move constructor.
     *
     * @~german
     * @brief Entfernt den verschobenen Konstruktor.
     */
    Statistics( Statistics && ) = delete;

    /**
     * @~english
     * @brief Delete copy assign.
     * @return Nothing.
     *
     * @~german
     * @brief Entfernt die kopierte Zuweisung.
     * @return Keine Rückgabe.
     */
    Statistics &operator=( Statistics const & ) = delete;

    /**
     * @~english
     * @brief Delete move assign.
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
     *
     * @~german
     * @brief Definiert den Pfad und Namen zum Statistikserver.
     *
     * @~english @b Example: @~german @b Beispiel:
     * @~english <br>for the HTTPS address www.vxapps.com and the folder u/stats.php.
     * @~german <br>Für die HTTPS Adresse www.vxapps.com und dem Verzeichnis u/stats.php.
     * @~
     * @code
     * Statistics::instance().setServerFilePath( "https://www.vxapps.com/u/stats.php" );
     * @endcode
     *
     * @~
     * @param _serverFilePath   @~english The file name to the statistics server. @~german Der Dateiname zum Statistikserver.
     */
    void setServerFilePath( const QString &_serverFilePath );

    /**
     * @~german
     * @brief Aufruf einer Seite mit dem Namen pageName um es an den Statistikserver zu übermitteln.
     *
     * @~english
     * @brief Request a page with the name pageName in order to transfer it to the statistics server.
     *
     * @~
     * @param _page   @~english The name of the requested page. @~german Der Name der aufgerufenen Seite.
     * @note @~english Limited to 255 characters. @~german Auf 255 Zeichen begrentzt.
     */
    void page( const QString &_page );

    /**
     * @~english
     * @brief When you would like to request a page with dynamic content please use this function.
     *
     * @~german
     * @brief Wenn Sie eine Seite aufrufen möchten mit dynamischem Inhalt verwenden Sie diese Funktion.
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
     * @~english @n Open browser with URL. @~german @n Browser mit URL.
     * @~
     * @code
     * Statistics::instance().event( "open", urlOrName );
     * Statistics::instance().event( "open", "https://www.vxapps.com" );
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
     * Statistics::instance().event( "play", "https://www.vxapps.com/movie.m4v" );
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
     * @~english @n Shake the device. @~german @n Das Gerät schütteln.
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
     *
     * @~
     * @param _event   @~english The event. @~german Das Event.
     * @~
     * @param _value   @~english The value for the event. @~german Der Wert für das Event.
     */
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Woverloaded-virtual"
#endif
    void event( const QString &_event, const QString &_value = QString() );
#ifdef __clang__
#pragma clang diagnostic pop
#endif

    /**
     * @~english
     * @brief To capture ads - correspondingly the shown ad.
     *
     * @~german
     * @brief Für das Erfassen von Werbeeinblendungen - entsprechend die angezeigte Werbung.
     *
     * @~
     * @see Statistics#event:withValue:
     *
     * @code
     * Statistics::instance().event( "ads", campaign );
     * @endcode
     *
     * @~
     * @param _campaign   @~english The displayed ad. Limited to 255 characters. @~german Die angezeigte Werbung. Auf 255 Zeichen begrentzt.
     */
    void ads( const QString &_campaign );

    /**
     * @~english
     * @brief To capture map shifts - correspondingly the new center.
     *
     * @~german
     * @brief Für die Erfassung von Kartenverschiebungen - entsprechend das neue Zentrum.
     *
     * @~
     * @see event
     *
     * @code
     * Statistics::instance().event( "move", latitude,longitude);
     * @endcode
     *
     * @~
     * @param _latitude   @~english Latitude of center. @~german Latitude des Zentrums.
     * @~
     * @param _longitude   @~english Longitude of center. @~german Longitude des Zentrums.
     */
    void move( double _latitude, double _longitude );

    /**
     * @~english
     * @brief To capture open websites or documents including the information which page or document has been requested.
     *
     * @~german
     * @brief Für das Erfassen von geöffneten Webseiten oder Dokumenten mit der Information, welche Seite, bzw.
     * welches Dokument aufgerufen wurde.
     *
     * @~
     * @see event
     *
     * @code
     * Statistics::instance().event( "open", urlOrName );
     * @endcode
     *
     * @~
     * @param _urlOrName   @~english The displayed website/document. Limited to 255 characters. @~german Die angezeigte Webseite/das angezeigte Dokument. Auf 255 Zeichen begrentzt.
     */
    void open( const QString &_urlOrName );

    /**
     * @~english
     * @brief To capture played files including the information which file/action has been played.
     *
     * @~german
     * @brief Für das Erfassen von abgespielten Dateien mit der Info, welche Datei/Aktion abgespielt wurde.
     *
     * @~
     * @see Statistics#event:withValue:
     *
     * @code
     * Statistics::instance().event( "play", urlOrName );
     * @endcode
     *
     * @~
     * @param _urlOrName   @~english The played file. Limited to 255 characters. @~german Die abgespielte Datei. Auf 255 Zeichen begrentzt.
     */
    void play( const QString &_urlOrName );

    /**
     * @~english
     * @brief To capture searches including the information for which has been searched.
     *
     * @~german
     * @brief Für die Erfassung von Suchen mit der Info, nach was gesucht wurde.
     *
     * @~
     * @see Statistics#event:withValue:
     *
     * @code
     * Statistics::instance().event( "search", text );
     * @endcode
     *
     * @~
     * @param _text   @~german Der gesuchte Text. Auf 255 Zeichen begrentzt. @~english The searched text. Limited to 255 characters.
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
     *
     * @~german
     * @brief Für die Erfassung von getippten/gedrückten Aktionen.
     *
     * @~
     * @see Statistics#event:withValue:
     *
     * @code
     * Statistics::instance().event( "touch", action );
     * @endcode
     *
     * @~
     * @param _action   @~english The name of the touched action. Limited to 255 characters. @~german Der Name der getippten/gedrückten Aktion. Auf 255 Zeichen begrentzt.
     */
    void touch( const QString &_action );

  protected:
    /**
     * @~english
     * @brief Default constructor for Statistics.
     * @param _parent   Parent object.
     *
     * @~german
     * @brief Standardkonstruktur für Statistics.
     * @param _parent   Übergeordnetes Objekt.
     */
    Statistics( QObject *_parent = nullptr );

  private:
    /**
     * @~english
     * @brief Internal server path to connect to.
     *
     * @~german
     * @brief Interner Server Pfad für das Signalling.
     */
    QString m_serverFilePath = QString();

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
     * @brief Aktueller Status einer Funkverbindung.
     */
    Device::Radio m_radio = Device::Radio::None;

    /**
     * @~english
     * @brief Last entered page name.
     *
     * @~german
     * @brief Zuletzt besuchte Seite.
     */
    QString m_lastPage = QString();

    /**
     * @~english
     * @brief Last message to send.
     *
     * @~german
     * @brief Zuletzt versendete Nachricht.
     */
    QUrlQuery m_lastMessage = QUrlQuery();

    /**
     * @~english
     * @brief Network access manager to post the events.
     *
     * @~german
     * @brief Netzwerkverbindungsmanager um Aktivitäten zu verschicken.
     */
    QNetworkAccessManager *m_networkAccessManager = nullptr;

    /**
     * @~english
     * @brief Define core information for every block you need to send.
     * @return Core information block to send.
     *
     * @~german
     * @brief Allgemeine Informationen zu jedem speziellen Block.
     * @return Allgemeine Informationen einer zu versendenden Nachricht.
     */
    QUrlQuery coreMessage() const;

    /**
     * @~english
     * @brief Send the complete message.
     * @param _message   Event to send to the database.
     *
     * @~german
     * @brief Versenden der vollständigen Nachricht.
     * @param _message   Aktivität um es in die Datenbank zu versenden.
     */
    void sendMessage( const QUrlQuery &_message );

    /**
     * @~english
     * @brief Add an outstanding message, if we cannot send it directly to the
     * server.
     * @param _message   The message to send.
     *
     * @~german
     * @brief Hinzufügen von nicht direkt versendbaren Nachrichten um es später
     * weiter zu senden.
     * @param _message   Die zu verschickende Nachricht.
     */
    void addOutstandingMessage( const QUrlQuery &_message );

    /**
     * @~english
     * @brief Send all offline and not direct sended messages.
     *
     * @~german
     * @brief Nachträgliches versenden aller aktuell zwischengespeicherten
     * Nachrichten.
     */
    void sendOutstandingMessages();

  private Q_SLOTS:
    /**
     * @~english
     * @brief Called, when a reply has finished, to be complete or to add the
     * message to outstanding messages.
     * @param _reply   The received network reply.
     *
     * @~german
     * @brief Wird aufgerufen, wenn Daten nach dem Versand erhalten wurden um
     * den Erhalt zu bestätigen oder die nicht versendbare Nachricht
     * zwischenzuspeichern.
     * @param _reply   Das erhaltene Ergebnis des Versandes.
     */
    void slotFinished( QNetworkReply *_reply );

    /**
     * @~english
     * @brief Changes on Reachability in any case. Online/Offline, Status, Radio.
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