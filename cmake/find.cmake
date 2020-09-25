#
# Copyright (C) 01/10/2020 VX APPS <sales@vxapps.com>
#
# This document is property of VX APPS. It is strictly prohibited
# to modify, sell or publish it in any way. In case you have access
# to this document, you are obligated to ensure its nondisclosure.
# Noncompliances will be prosecuted.
#
# Diese Datei ist Eigentum der VX APPS. Jegliche Änderung, Verkauf
# oder andere Verbreitung und Veröffentlichung ist strikt untersagt.
# Falls Sie Zugang zu dieser Datei haben, sind Sie verpflichtet,
# alles in Ihrer Macht stehende für deren Geheimhaltung zu tun.
# Zuwiderhandlungen werden strafrechtlich verfolgt.
#

if(APPLE AND NOT IOS)
  find_library(FOUNDATION Foundation REQUIRED)
  find_library(SYSTEMCONFIGURATION SystemConfiguration REQUIRED)
  find_package(OpenSSL REQUIRED)
endif()
if(IOS)
  find_library(UIKIT UIKit REQUIRED)
  message(${UIKIT})
endif()
find_package(Qt5 COMPONENTS Network REQUIRED)
find_package(Qt5 COMPONENTS Gui)
