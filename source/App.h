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

#pragma once

/* qt header */
#include <QString>

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
   * @brief The App class.
   * General information about the running application including validation of
   * fair use.
   *
   * @~german
   * @brief Die Klasse App.
   * Bietet allgemeine Informationen über die Anwendung und eine Überprüfung
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
     * @brief Deletes move constructor.
     *
     * @~german
     * @brief Entfernt den verschobenen Konstruktor.
     */
    App( App && ) = delete;

    /**
     * @~english
     * @brief Deletes copy assign.
     * @return Nothing.
     *
     * @~german
     * @brief Entfernt die kopierte Zuweisung.
     * @return Keine Rückgabe.
     */
    App &operator=( App const & ) = delete;

    /**
     * @~english
     * @brief Deletes move assign.
     * @return Nothing.
     *
     * @~german
     * @brief Entfernt die verschobene Zuweisung.
     * @return Keine Rückgabe.
     */
    App &operator=( App && ) = delete;

    /**
     * @~english
     * @brief Returns true, if the app is fairly used.
     * @return True, if the app is fairly used - otherwise false.
     *
     * @~german
     * @brief Gibt wahr zurück, wenn die Anwendung fair verwendet wird.
     * @return Wahr, wenn die Anwendung fair verwendet wird - sonst falsch.
     */
    virtual bool fairUse() const;

    /**
     * @~english
     * @brief Returns the identifier of the application. E.g. com.app.name
     * @return Identifier of application.
     *
     * @~german
     * @brief Gibt die Id der Anwendung zurück. Bsp.: de.anwendung.name
     * @return Id der Anwendug.
     *
     * @~
     * @see setIdentifier
     */
    QString identifier() const;

    /**
     * @~english
     * @brief Sets the application identifier, otherwise
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
     * @brief Returns the version of the application. E.g. 1.0
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
     * @brief Sets the application version, otherwise
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
     * @brief Returns the build of application. E.g. 100, 3A4E
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
     * @brief Standardkonstruktor für App.
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
    QString m_identifier = {};

    /**
     * @~english
     * @brief The application version.
     *
     * @~german
     * @brief Version der Anwendung.
     */
    QString m_version = {};

    /**
     * @~english
     * @brief The application build.
     *
     * @~german
     * @brief Build der Anwendung.
     */
    QString m_build = {};
  };
}
