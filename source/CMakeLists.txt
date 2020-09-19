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

if(WIN32)
add_library(${PROJECT_NAME}
  ../README.md
  App_default.cpp
  App.cpp
  App.h
  Device_default.cpp
  Device_win.cpp
  Device.cpp
  Device.h
  Reachability.cpp
  Reachability.h
  Statistics.cpp
  Statistics.h
  statistics.qrc
)
endif()

if(APPLE)
add_library(${PROJECT_NAME}
  ../README.md
  App_default.cpp
  App_mac_objc.mm
  App.cpp
  App.h
  Device_default.cpp
  Device_mac_objc.mm
  Device.cpp
  Device.h
  Reachability.cpp
  Reachability.h
  Statistics.cpp
  Statistics.h
  statistics.qrc
)
endif()

if(UNIX AND NOT APPLE)
add_library(${PROJECT_NAME}
  ../README.md
  App_default.cpp
  App.cpp
  App.h
  Device_default.cpp
  Device_linux.cpp
  Device.cpp
  Device.h
  Reachability.cpp
  Reachability.h
  Statistics.cpp
  Statistics.h
  statistics.qrc
)
endif()

target_link_libraries(${PROJECT_NAME} PRIVATE OpenSSL::SSL Qt5::Network)

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR})
