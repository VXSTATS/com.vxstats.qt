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
#include <QObject>
#include <QString>
#include <QVector>

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
   * @brief The Device class.
   * Delivers information about the current device.
   *
   * @~german
   * @brief Die Device Klasse.
   * Liefert Informationen zum aktuellen Gerät.
   */
  class Device {

    Q_GADGET

  public:
    /**
     * @~english
     * @brief The Connection enum.
     *
     * @~german
     * @brief Der Connection Enumerator.
     */
    enum class Connection {

      Unknown, /**< @~english Unkown connection. @~german Unbekannte Verbindungsart. */
      Offline, /**< @~english No connection, we are offline. @~german Keine Verbindung, wir sind offline. */
      Ethernet, /**< @~english Ethernet connection. @~german Netzwerkverbindung. */
      Wifi, /**< @~english Wifi connection. @~german Kabellose Verbindung. */
      WWAN, /**< @~english WWAN connection. @~german Funkverbindung. */
      Bluetooth /**< @~english Bluetooth connection. @~german Bluetooth-Verbindung. */
    };
    Q_ENUM( Connection )

    /**
     * @~english
     * @brief The Radio enum.
     *
     * @~german
     * @brief Der Radio Enumerator.
     */
    enum class Radio {

      None, /**< @~english No radio. @~german Keine Funkverbindung. */
      GPRS, /**< @~english GPRS radio. @~german GPRS-Funkverbindung. */
      Edge, /**< @~english Edge radio. @~german Edge-Funkverbindung. */
      WCDMA, /**< @~english WCDMA radio. @~german WCDMA-Funkverbindung. */
      HSDPA, /**< @~english HSDPA radio. @~german HSDPA-Funkverbindung. */
      HSUPA, /**< @~english HSUPA radio. @~german HSUPA-Funkverbindung. */
      CDMA1x, /**< @~english CDMA1x radio. @~german CDMA1x-Funkverbindung. */
      CDMAEVDORev0, /**< @~english CDMAEVDORev0 radio. @~german CDMAEVDORev0-Funkverbindung. */
      CDMAEVDORevA, /**< @~english CDMAEVDORevA radio. @~german CDMAEVDORevA-Funkverbindung. */
      CDMAEVDORevB, /**< @~english CDMAEVDORevB radio. @~german CDMAEVDORevB-Funkverbindung. */
      HRPD, /**< @~english HRPD radio. @~german HRPD-Funkverbindung. */
      LTE, /**< @~english LTE radio. @~german LTE-Funkverbindung. */
      Radio2G, /**< @~english Radio2G radio. @~german Radio2G-Funkverbindung. */
      Radio3G, /**< @~english Radio3G radio. @~german Radio3G-Funkverbindung. */
      Radio4G, /**< @~english Radio4G radio. @~german Radio4G-Funkverbindung. */
      Radio5G, /**< @~english Radio5G radio. @~german Radio5G-Funkverbindung. */
      WiMAX /**< @~english WiMAX radio. @~german WiMAX-Funkverbindung. */
    };
    Q_ENUM( Radio )

    /**
     * @~english
     * @brief Instance of Device. C++11 Singleton thread-safe.
     * @return Singleton of Device.
     *
     * @~german
     * @brief Instanz von Device. C++11 Singleton thread-safe.
     * @return Einzige Instanz von Device.
     */
    static Device &instance();

    /**
     * @~english
     * @brief Deletes copy constructor.
     *
     * @~german
     * @brief Entfernt den kopierenden Konstruktor.
     */
    Device( const Device & ) = delete;

    /**
     * @~english
     * @brief Deletes move constructor.
     *
     * @~german
     * @brief Entfernt den verschobenen Konstruktor.
     */
    Device( Device && ) = delete;

    /**
     * @~english
     * @brief Deletes copy assign.
     * @return Nothing.
     *
     * @~german
     * @brief Entfernt die kopierte Zuweisung.
     * @return Keine Rückgabe.
     */
    Device &operator=( const Device & ) = delete;

    /**
     * @~english
     * @brief Deletes move assign.
     * @return Nothing.
     *
     * @~german
     * @brief Entfernt die verschobene Zuweisung.
     * @return Keine Rückgabe.
     */
    Device &operator=( Device && ) = delete;

    /**
     * @~english
     * @brief Initialize the current device.
     *
     * @~german
     * @brief Initialisieren des aktuellen Geräts.
     */
    void initialize();

    /**
     * @~english
     * @brief Returns true, if the device run in dark mode.
     * @return True, if the device run in dark mode - otherwise false.
     *
     * @~german
     * @brief Gibt wahr zurück, wenn die Plattform den Darkmode verwendet.
     * @return Wahr, wenn die Plattform den Darkmode verwendet - sonst falsch.
     */
    [[nodiscard]] virtual bool useDarkMode() const;

    /**
     * @~english
     * @brief Returns true, if the device is jailbroken - otherwise false.
     * @return True, if the device is jailbroken - otherwise false.
     *
     * @~german
     * @brief Gibt wahr zurück, wenn das Gerät gejailbreakt ist - sonst falsch.
     * @return Wahr, wenn das Gerät gejailbreakt ist - sonst falsch.
     */
    [[nodiscard]] virtual bool isJailbroken() const;

    /**
     * @~english
     * @brief Returns true, if the device is in tablet mode, opens one app a
     * time - otherwise false.
     * @return True, if the device is in tablet mode - otherwise false.
     *
     * @~german
     * @brief Gibt wahr zurück, wenn sich das Gerät im Tabletmodus befindet,
     * eine Anwendung belegt den gesamten Bildschirm - sonst falsch.
     * @return Wahr, wenn sich das Gerät im Tabletmodus befindet - sonst falsch.
     */
    [[nodiscard]] virtual bool isTabletMode() const;

    /**
     * @~english
     * @brief Returns true, if the device contains a touch screen - otherwise
     * false.
     * @return True, if the device contains a touch screen - otherwise false.
     *
     * @~german
     * @brief Gibt wahr zurück, wenn das Gerät mit einem Touch-Bildschirm
     * ausgestattet ist - sonst falsch.
     * @return Wahr, wenn das Gerät mit einem Touch-Bildschirm ausgestattet
     * ist - sonst falsch.
     */
    [[nodiscard]] virtual bool hasTouchScreen() const;

    /**
     * @~english
     * @brief Returns true, if the device has active voice over - otherwise false.
     * @return True, if the device has active voice over - otherwise false.
     *
     * @~german
     * @brief Gibt wahr zurück, wenn auf dem Gerät VoiceOver aktiviert ist -
     * sonst falsch.
     * @return Wahr, wenn auf dem Gerät VoiceOver aktiviert ist - sonst falsch.
     */
    [[nodiscard]] virtual bool isVoiceOverActive() const;

    /**
     * @~english
     * @brief Returns the device model. E.g. Precision
     * @return The device model.
     *
     * @~german
     * @brief Gibt das Gerätemodell zurück. Bsp.: Precision
     * @return Das Gerätemodell.
     */
    [[nodiscard]] inline QString model() const { return m_model; }

    /**
     * @~english
     * @brief Set the device model. E.g. Precision
     * @param _model   The device model.
     *
     * @~german
     * @brief Setzt das Gerätemodell. Bsp.: Precision
     * @param _model   Das Gerätemodell.
     */
    void setModel( const QString &_model );

    /**
     * @~english
     * @brief Returns the device vendor. E.g. Dell Inc.
     * @return The device vendor.
     *
     * @~german
     * @brief Gibt den Gerätehersteller zurück. Bsp.: Dell Inc.
     * @return Der Gerätehersteller.
     */
    [[nodiscard]] inline QString vendor() const { return m_vendor; }

    /**
     * @~english
     * @brief Sets the device vendor. E.g. Dell Inc.
     * @param _vendor   The device vendor.
     *
     * @~german
     * @brief Setzt den Gerätehersteller. Bsp.: Dell Inc.
     * @param _vendor   Der Gerätehersteller.
     */
    void setVendor( const QString &_vendor );

    /**
     * @~english
     * @brief Returns the device version. E.g. 5510
     * @return The device version.
     *
     * @~german
     * @brief Gibt die Geräteversion zurück. Bsp.: 5510
     * @return Die Geräteversion.
     */
    [[nodiscard]] inline QString version() const { return m_version; }

    /**
     * @~english
     * @brief Sets the device version. E.g. 5510
     * @param _version   The device version.
     *
     * @~german
     * @brief Setzt die Geräteversion. Bsp.: 5510
     * @param _version   Die Geräteversion.
     */
    void setVersion( const QString &_version );

    /**
     * @~english
     * @brief The version of the operating system, e.g. 10.11.6
     * @return The version of the operating system.
     *
     * @~german
     * @brief Die Version des Betriebssystems, z.B. 10.11.6
     * @return Die Version des Betriebssystems.
     */
    [[nodiscard]] virtual QString osVersion() const;

    /**
     * @~english
     * @brief Unique identifier for current device.
     * @return The unique identifier for the current device.
     *
     * @~german
     * @brief Eindeutige Id des Geräts.
     * @return Eindeutige Id des Geräts.
     */
    [[nodiscard]] inline QString uniqueIdentifier() const { return m_uniqueIdentifier; }

    /**
     * @~english
     * @brief Some network type detection may not work, so this can be used for
     * a system to set the current connection type.
     * @param _interface   Interface to ask type for.
     * @return Current network type.
     *
     * @~german
     * @brief Manchmal müssen erkannte Netzwerke nachbearbeitet werden, um die
     * aktuelle Verbindung zu bestimmen.
     * @param _interface   Abzufragendes Interface.
     * @return Der aktuelle Netzwerktyp.
     */
    virtual Connection typeOfNetwork( const QString &_interface );

    /**
     * @~english
     * @brief Add a message to the outstanding message stack.
     * @param _message   Message to add.
     *
     * @~german
     * @brief Für den ausstehenden Nachrichten eine Nachricht hinzu.
     * @param _message   Nachricht, die hinzugefügt werden soll.
     */
    virtual void addOutstandingMessage( const QString &_message ) const;

    /**
     * @~english
     * @brief Get outstanding messages.
     * @return A list of outstanding messages.
     *
     * @~german
     * @brief Erhalte alle zwischengespeicherten Nachrichten.
     * @return Die Liste aller ausstehenden Nachrichten.
     */
    [[nodiscard]] virtual QVector<QString> sendOutstandingMessages() const;

  protected:
    /**
     * @~english
     * @brief Default constructor for Device.
     *
     * @~german
     * @brief Standardkonstruktor für Device.
     */
    Device() = default;

    /**
     * @~english
     * @brief Default destructor for Device.
     *
     * @~german
     * @brief Standarddestruktor für Device.
     */
    virtual ~Device() = default;

    /**
     * @~english
     * @brief Try to split a version from the model string.
     *
     * @~german
     * @brief Versuch eine Version aus dem Gerätemodell zu extrahieren.
     */
    void tryToSplitVersionFromModel();

  private:
    /**
     * @~english
     * @brief Is the current device initialized?
     *
     * @~german
     * @brief Wurde das aktuelle Gerät initialisiert?
     */
    bool m_initialized = false;

    /**
     * @~english
     * @brief The device model.
     *
     * @~german
     * @brief Das Gerätemodell.
     */
    QString m_model = {};

    /**
     * @~english
     * @brief The device vendor.
     *
     * @~german
     * @brief Der Gerätehersteller.
     */
    QString m_vendor = {};

    /**
     * @~english
     * @brief The device version.
     *
     * @~german
     * @brief Die Geräteversion.
     */
    QString m_version = {};

    /**
     * @~english
     * @brief The unique device identifier.
     *
     * @~german
     * @brief Die eindeutige Id des Geräts.
     */
    QString m_uniqueIdentifier = {};

    /**
     * @~english
     * @brief Is it a physical device? It is possible that the first device
     * is a virtual adapter or a tunnel adapter.
     * @param _hardwareAddress   Current hardware address to check if the card
     * is physical.
     * @return True, if this is a physical card - otherwise false.
     *
     * @~german
     * @brief Handelt es sich um einen physischen Adapter? Womöglich handelt es
     * sich um einen virtuellen oder einen getunnelten Adapter.
     * @param _hardwareAddress   Zu prüfende Hardwareadresse, um zu sehen, ob es sich um
     * einen physischen Adapter handelt.
     * @return Wahr, wenn es ein physischer Adapter ist - sonst falsch.
     */
    [[nodiscard]] virtual bool isPhysical( const QString &_hardwareAddress ) const;

    /**
     * @~english
     * @brief Returns the unique device identifier.
     * @return The unique device identifier.
     *
     * @~german
     * @brief Gibt die eindeutige Id des Geräts zurück.
     * @return Die eindeutige Id des Geräts.
     */
    [[nodiscard]] virtual QString uniqueId() const;
  };
}
