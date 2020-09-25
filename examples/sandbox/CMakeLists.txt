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

project(sandbox)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

add_executable(${PROJECT_NAME}
  main.cpp
)

if(GUI)
  set(${PROJECT_NAME}_libs Qt5::Gui)
endif()

target_link_libraries(${PROJECT_NAME} PRIVATE com.vxstats.qt Qt5::Core ${${PROJECT_NAME}_libs})
