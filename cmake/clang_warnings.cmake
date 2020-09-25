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

set(WARNING_FLAGS

  # From Xcode default project
#  -Wno-sign-conversion
  # +QRC and singleton
  -Wno-exit-time-destructors
#  -Wno-conversion

  # Own parameter
  -Wno-c++98-compat # C++11
  -Wno-padded

  # Qt global.h
  -Wno-redundant-parens

  # For doxygen documentation, because we handle two languages #1
  -Wno-documentation

  # QRC
  -Wno-global-constructors

  # Deprecated QNetwork* Qt 5.15
  -Wno-deprecated-declarations

  # Only Apple
  -Wno-old-style-cast

  # Only Android Linux Build
  -Wno-weak-vtables
)
