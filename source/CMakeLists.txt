#
# Copyright (C) 10/01/2020 VX STATS <sales@vxstats.com>
#
# This document is property of VX STATS. It is strictly prohibited
# to modify, sell or publish it in any way. In case you have access
# to this document, you are obligated to ensure its nondisclosure.
# Noncompliances will be prosecuted.
#
# Diese Datei ist Eigentum der VX STATS. Jegliche Änderung, Verkauf
# oder andere Verbreitung und Veröffentlichung ist strikt untersagt.
# Falls Sie Zugang zu dieser Datei haben, sind Sie verpflichtet,
# alles in Ihrer Macht stehende für deren Geheimhaltung zu tun.
# Zuwiderhandlungen werden strafrechtlich verfolgt.
#

project(com.vxstats.qt)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(${PROJECT_NAME}_os_src App_default.cpp Device_default.cpp)
if(WIN32)
  set(${PROJECT_NAME}_os_src App_default.cpp Device_win.cpp)
endif()
if(APPLE AND NOT IOS)
  set(${PROJECT_NAME}_os_src App_mac.mm Device_mac.mm statistics.qrc)
endif()
if (IOS)
  set(${PROJECT_NAME}_os_src App_mac.mm Device_ios.mm statistics.qrc)
endif()
if(UNIX AND NOT APPLE AND NOT ANDROID)
  set(${PROJECT_NAME}_os_src App_default.cpp Device_linux.cpp)
endif()
if(ANDROID)
  set(${PROJECT_NAME}_os_src App_default.cpp Device_android.cpp)
endif()

add_library(${PROJECT_NAME}
  ../.github/workflows/integrate.yml
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

if(APPLE AND NOT IOS)
  set(${PROJECT_NAME}_os_libs OpenSSL::SSL ${FOUNDATION} ${SYSTEMCONFIGURATION})
endif()
if(IOS)
  set(${PROJECT_NAME}_os_libs ${UIKIT})
endif()
if(WIN32)
  set(${PROJECT_NAME}_os_libs gdi32.lib)
endif()
if(ANDROID)
  set(${PROJECT_NAME}_os_libs ${QtVERSION}::AndroidExtras)
endif()
if(GUI)
  set(${PROJECT_NAME}_os_libs ${${PROJECT_NAME}_os_libs} ${QtVERSION}::Gui)
endif()
target_link_libraries(${PROJECT_NAME} PRIVATE ${QtVERSION}::Network ${${PROJECT_NAME}_os_libs})

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR})
