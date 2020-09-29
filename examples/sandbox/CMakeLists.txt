#
# Copyright (C) 10/01/2020 VX APPS <sales@vxapps.com>
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

project(sandbox LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

#if(ANDROID)
#  set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
#  if (ANDROID_ABI STREQUAL "armeabi-v7a")
#    set(ANDROID_EXTRA_LIBS
#    ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libcrypto.so
#    ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libssl.so)
#  endif()
#endif()

if(ANDROID)
  add_library(${PROJECT_NAME} SHARED
    main.cpp
  )
else()
  add_executable(${PROJECT_NAME}
    main.cpp
  )
endif()

if(GUI)
  set(${PROJECT_NAME}_libs Qt5::Gui)
endif()

target_link_libraries(${PROJECT_NAME} PRIVATE com.vxstats.qt Qt5::Core ${${PROJECT_NAME}_libs})
