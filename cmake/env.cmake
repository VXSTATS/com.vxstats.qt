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

# for debugging of build steps
option(CMAKE_VERBOSE_MAKEFILE "Show the complete build commands" OFF)

# build with or without Qt Gui dependency
option(GUI "Build with or without Qt Gui dependency" ON)

# enable iOS build
option(IOS "Enable iOS build" OFF)

# Pathes
if(NOT DEV)
  get_filename_component(DEV "${CMAKE_CURRENT_LIST_DIR}/.." ABSOLUTE)
endif()

set(CMAKE ${DEV}/cmake)

# Force C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Warning flags
# Case insensitive match
if(CMAKE_CXX_COMPILER_ID MATCHES "[cC][lL][aA][nN][gG]")
  include(${CMAKE}/clang_warnings.cmake)

  set(WARNING_FLAGS_SPACED "")
  foreach(WARNING_FLAG ${WARNING_FLAGS})
    set(WARNING_FLAGS_SPACED "${WARNING_FLAGS_SPACED} ${WARNING_FLAG}")
  endforeach()

  if(NOT CMAKE_CXX_COMPILER_ID MATCHES "[aA][pP][pP][lL][eE][cC][lL][aA][nN][gG]" AND CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL "13")
    foreach(WARNING_FLAG ${WARNING_FLAGS_VERSION13})
      set(WARNING_FLAGS_SPACED "${WARNING_FLAGS_SPACED} ${WARNING_FLAG}")
    endforeach()
  endif()

  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Weverything -Werror")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${WARNING_FLAGS_SPACED}")

elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  include(${CMAKE}/gcc_warnings.cmake)

  set(WARNING_FLAGS_SPACED "")
  foreach(WARNING_FLAG ${WARNING_FLAGS})
    set(WARNING_FLAGS_SPACED "${WARNING_FLAGS_SPACED} ${WARNING_FLAG}")
  endforeach()

  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -Wextra")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${WARNING_FLAGS_SPACED}")

elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  include(${CMAKE}/msvc_warnings.cmake)

  set(WARNING_FLAGS_SPACED "")
  foreach(WARNING_FLAG ${WARNING_FLAGS})
    set(WARNING_FLAGS_SPACED "${WARNING_FLAGS_SPACED} ${WARNING_FLAG}")
  endforeach()

  # Force to always compile with W4
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4 /WX")
  if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
    string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
  endif()

  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${WARNING_FLAGS_SPACED}")
endif()

add_compile_options("$<$<CONFIG:DEBUG>:-DDEBUG>")

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Project modules/variables
set(CMAKE_MODULE_PATH ${CMAKE}/modules)

# Includes
include(${CMAKE}/find_package.cmake)
