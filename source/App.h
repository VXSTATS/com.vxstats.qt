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
   * @brief The App class.
   * Global information about the running app including validation of fair use.
   *
   * @~german
   * @brief Die App Klasse.
   * Bietet allgemeine Informationen über die Anwendung und einer Überprüfung
   * der fairen Verwendung.
   */
  class App {

  public:
    /**
     * @~english
     * @brief Instance of App. C++11 Singleton thread-safe.
     * @return Singleton of App.
     *
     * @~german
     * @brief Instanz von App. C++11 Singleton thread-safe.
     * @return Einzige Instanz von App.
     */
    static App &instance();

    /**
     * @~english
     * @brief Delete move constructor.
     *
     * @~german
     * @brief Entfernt den verschobenen Konstruktor.
     */
    App( App && ) = delete;

    /**
     * @~english
     * @brief Delete copy assign.
     * @return Nothing.
     *
     * @~german
     * @brief Entfernt die kopierte Zuweisung.
     * @return Keine Rückgabe.
     */
    App &operator=( App const & ) = delete;

    /**
     * @~english
     * @brief Delete move assign.
     * @return Nothing.
     *
     * @~german
     * @brief Entfernt die verschobene Zuweisung.
     * @return Keine Rückgabe.
     */
    App &operator=( App && ) = delete;

    /**
     * @~english
     * @brief Return true, if the app is fairly used.
     * @return True, if the app is fairly used - otherwise false.
     *
     * @~german
     * @brief Gibt whar zurück, wenn die Anwendung fair verwendet wird.
     * @return Wahr, wenn die Anwendung fair verwendet wird - sonst falsch.
     */
    virtual bool fairUse() const;

    /**
     * @~english
     * @brief Return the identifier of application. E.g. com.app.name
     * @return Identifier of application.
     *
     * @~german
     * @brief Gibt die Id der Anwendung zurück. Bsp.: de.anwendung.name
     * @return Die Id der Anwendug.
     *
     * @~
     * @see setIdentifier
     */
    QString identifier() const;

    /**
     * @~english
     * @brief Set the application identifier, if not set
     * <a href="https://doc.qt.io/qt-5/qcoreapplication.html#applicationName-prop">QCoreApplication::applicationName()</a>
     * property is used. E.g. com.app.name
     * @param _identifier   Application identifier to set.
     *
     * @~german
     * @brief Setzt die Id der Anwendung, sonst wird
     * <a href="https://doc.qt.io/qt-5/qcoreapplication.html#applicationName-prop">QCoreApplication::applicationName()</a>
     * verwendet. Bsp.: de.anwendung.name
     * @param _identifier   Id der Anwendung.
     *
     * @~
     * @see identifier
     */
    void setIdentifier( const QString &_identifier );

    /**
     * @~english
     * @brief Return the version of application. E.g. 1.0
     * @return Version of application.
     *
     * @~german
     * @brief Gibt die Version der Anwendung zurück. Bsp.: 1.0
     * @return Version der Anwendung.
     *
     * @~
     * @see setVersion
     */
    QString version() const;

    /**
     * @~english
     * @brief Set the application version, if not set
     * <a href="https://doc.qt.io/qt-5/qcoreapplication.html#applicationVersion-prop">QCoreApplication::applicationVersion()</a>
     * property is used. E.g. 1.0
     * @param _version   Application version to set.
     *
     * @~german
     * @brief Setzt die Version der Anwendung, sonst wird
     * <a href="https://doc.qt.io/qt-5/qcoreapplication.html#applicationVersion-prop">QCoreApplication::applicationVersion()</a>
     * verwendet. Bsp.: 1.0
     * @param _version   Version der Anwendung.
     *
     * @~
     * @see version
     */
    void setVersion( const QString &_version );

    /**
     * @~english
     * @brief Return the build of application. E.g. 100, 3A4E
     * @return Build of application.
     *
     * @~german
     * @brief Gibt den Build der Anwendung zurück. Bsp.: 100, 3A4E
     * @return Build der Anwendung.
     *
     * @~
     * @see setBuild
     */
    inline QString build() const { return m_build; }

    /**
     * @~english
     * @brief Set the application build.
     * @param _build   Application build to set. E.g. 100, 3A4E
     *
     * @~german
     * @brief Setzt die Build der Anwendung. Bsp.: 100, 3A4E
     * @param _build   Build der Anwendung.
     *
     * @~
     * @see build
     */
    void setBuild( const QString &_build );

  protected:
    /**
     * @~english
     * @brief Default constructor for App.
     *
     * @~german
     * @brief Standardkonstruktur für App.
     */
    App() = default;

    /**
     * @~english
     * @brief Default destructor for App.
     *
     * @~german
     * @brief Standarddestruktor für App.
     */
    virtual ~App() = default;

  private:
    /**
     * @~english
     * @brief The application identifier.
     *
     * @~german
     * @brief Id der Anwendung.
     */
    QString m_identifier = QString();

    /**
     * @~english
     * @brief The application version.
     *
     * @~german
     * @brief Version der Anwendung.
     */
    QString m_version = QString();

    /**
     * @~english
     * @brief The application build.
     *
     * @~german
     * @brief Build der Anwendung.
     */
    QString m_build = QString();
  };
}
