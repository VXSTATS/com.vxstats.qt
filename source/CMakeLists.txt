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

project(com.vxstats.sdk.qt)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(${PROJECT_NAME}_os_src App_default.cpp Device_default.cpp)
if(WIN32)
  set(${PROJECT_NAME}_os_src App_default.cpp Device_win.cpp)
endif()
if(APPLE AND NOT IOS)
  set(${PROJECT_NAME}_os_src App_mac_objc.mm Device_mac_objc.mm statistics.qrc)
endif()
if (IOS)
  set(${PROJECT_NAME}_os_src App_default.cpp Device_ios_objc.mm statistics.qrc)
endif()
if(UNIX AND NOT APPLE)
  set(${PROJECT_NAME}_os_src App_default.cpp Device_linux.cpp)
endif()

add_library(${PROJECT_NAME}
  ../README.md
  App.cpp
  App.h
  Device.cpp
  Device.h
  Reachability.cpp
  Reachability.h
  Statistics.cpp
  Statistics.h
  ${${PROJECT_NAME}_os_src}
)

set(${PROJECT_NAME}_os_libs)
if(APPLE AND NOT IOS)
  set(${PROJECT_NAME}_os_libs OpenSSL::SSL ${FOUNDATION} ${SYSTEMCONFIGURATION})
endif()
if(IOS)
  set(${PROJECT_NAME}_os_libs ${UIKIT})
endif()
if(WIN32)
  set(${PROJECT_NAME}_os_libs gdi32.lib)
endif()
target_link_libraries(${PROJECT_NAME} PRIVATE Qt5::Network ${${PROJECT_NAME}_os_libs})

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR})
