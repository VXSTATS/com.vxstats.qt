#/*
# * Copyright (C) 01/10/2020 VX APPS <sales@vxapps.com>
# *
# * The ownership of this document rests with the VX APPS. It is
# * strictly prohibited to change, sell or publish it in any way. In case
# * you have access to this document, you are obligated to ensure its
# * nondisclosure. Noncompliances will be prosecuted.
# *
# * Diese Datei ist Eigentum der VX APPS. Ändern, verkaufen oder
# * auf eine andere Weise verbreiten und öffentlich machen ist strikt
# * untersagt. Falls Sie Zugang zu dieser Datei haben, sind Sie
# * verpflichtet alles Mögliche für deren Geheimhaltung zu tun.
# * Zuwiderhandlungen werden strafrechtlich verfolgt.
# */

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
