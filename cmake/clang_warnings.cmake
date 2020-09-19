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

set(WARNING_FLAGS

  # From Xcode default project
#  -Wno-sign-conversion
#  -Wno-exit-time-destructors
#  -Wno-conversion

  # Own parameter
  -Wno-c++98-compat # C++11
  -Wno-padded

  # Qt global.h
  -Wno-redundant-parens

  # For doxygen documentation, because we handle two languages #1
  -Wno-documentation
  -Wno-documentation-unknown-command

  # QRC and static instance
  -Wno-exit-time-destructors

  # QRC
  -Wno-global-constructors

  # Deprecated QNetwork* Qt 5.15
  -Wno-deprecated-declarations

  # Only Apple
  -Wno-old-style-cast
)
