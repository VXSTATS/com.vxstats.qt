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
#include <QtAndroidExtras>

#ifdef QT_GUI_LIB
  #include <QTouchDevice>
#endif

/* local header */
#include "Device.h"

namespace vxstats {

  /**
   * @~english
   * @brief The Device class for Android.
   * Delivers information about the current device.
   *
   * @~german
   * @brief Die Device Klasse für Android.
   * Liefert Informationen zum aktuellen Gerät.
   */
  class Device_android : public Device {

  public:
    Device_android();

  private:
    [[nodiscard]] bool hasTouchScreen() const final;
  };

  Device &Device::instance() {

    static Device_android device;
    return device;
  }

  Device_android::Device_android() {

    QAndroidJniObject nativeVendor = QAndroidJniObject::getStaticObjectField("android.os.Build", "MANUFACTURER", "Ljava/lang/String;");
    QString vendor = nativeVendor.toString();
    if ( vendor == "unknown" )
      vendor = "Android Simulator";
    setVendor( vendor );

    QAndroidJniObject nativeModel = QAndroidJniObject::getStaticObjectField("android.os.Build", "MODEL", "Ljava/lang/String;");
    QString model = nativeModel.toString();
    model.remove( vendor );
    model = model.trimmed();
    setModel( model );

    QString version = nativeModel.toString();
    version.remove( model );
    version = version.trimmed();
    setVersion( version );
  }

  bool Device_android::hasTouchScreen() const {

#ifdef QT_GUI_LIB
    return QTouchDevice::devices().size() > 0;
#else
    return true;
#endif
  }
}
