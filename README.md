![integration](https://github.com/VXSTATS/com.vxstats.qt/workflows/integration/badge.svg)

* [Preparation](#preparation)
   * [macOS](#macos)
* [Installation](#installation)
   * [User defined build variables](#user-defined-build-variables)
      * [Debugging](#debugging)
      * [Gui dependencies](#gui-dependencies)
      * [iOS](#ios)
* [Implementation](#implementation)
   * [Pre-Setup](#pre-setup)
   * [Setup](#setup)
   * [Page](#page)
   * [Event](#event)
      * [Ads](#ads)
      * [Move](#move)
      * [Open](#open)
      * [Play](#play)
      * [Search](#search)
      * [Shake](#shake)
      * [Touch](#touch)
* [Compatiblity](#compatiblity)
   * [Qt Versions](#qt-versions)
   * [Windows](#windows)
   * [macOS](#macos-1)
   * [Linux](#linux)
   * [iOS](#ios-1)
   * [Android](#android)
* [Known Issues](#known-issues)
   * [Windows](#windows-1)
* [Pending Issues](#pending-issues)
   * [App Store](#app-store)

# Preparation
Checkout and create the environment.
```bash
git clone --depth 1 --branch 1.0 --single-branch https://github.com/VXSTATS/com.vxstats.qt.git
cd com.vxstats.qt
mkdir build
cd build
```

## macOS
The macOS build will do a fair use check of the app, so it is essential to have OpenSSL 1.1 or OpenSSL 3 installed. You can find OpenSSL using the OPENSSL_ROOT_DIR set. The easiest way is to install OpenSSL from brew (https://brew.sh/).
```bash
brew install openssl@1.1
brew list openssl@1.1
```
```bash
brew install openssl@3
brew list openssl@3
```
Then brew will show you the OpenSSL installation path of latest version. Normally you can use /usr/local/opt/openssl@1.1 or /usr/local/opt/openssl@3.

# Installation
Use a Qt Creator to load CMakeList.txt from the checked-out folder, run the cmake enviroment, generate makefiles, and build it.
```bash
cmake ..
make -j`nproc`
```

## User-defined build variables
Inside cmake/env.cmake you will find some user-defined build options for particular purposes.

### Debugging
You can log the verbose build output.
```cmake
# for debugging of build steps
option(CMAKE_VERBOSE_MAKEFILE "Show the complete build commands" OFF)
```

### Gui dependencies
You can disable Qt Gui dependencies.
```cmake
# build with or without Qt Gui dependency
option(GUI "Build with or without Qt Gui dependency" ON)
```

### iOS
If you want to build for iOS or build with an iOS specified cmake toolchain (https://github.com/leetal/ios-cmake). You will find complete documentation here and can generate native XCode builds.
```cmake
# enable iOS build
option(IOS "Enable iOS build" OFF)
```

# Implementation
```c++
using namespace vxstats;
```

## Pre-Setup
Qt application name and version.
```c++
QCoreApplication::setApplicationName( "$appid" );
QCoreApplication::setApplicationVersion( "$version" );
```

Alternatively, use direct setup without Qt meta system.
```c++
App::instance().setIdentifier( "$appid" );
App::instance().setVersion( "$version" );
```

Setting up your build id is only possible directly; Qt will not support such a value directly.
```c++
App::instance().setBuild( "$build" );
```

## Setup
Set up your environment with your credentials. Please put your username, password and url here. For obfuscation, please follow our best practice documentation.
```c++
Statistics::instance().setUsername( "sandbox" );
Statistics::instance().setPassword( "sandbox" );
Statistics::instance().setServerFilePath( "https://sandbox.vxstats.com/" );
```

## Page
This is the global context that you are currently in your application. Just give it a simple name with logical app structure to identify where the user stays.
```c++
Statistics::instance().page( "Main" );
```

## Event
If you want to request a page with dynamic content, please use this function.
```c++
Statistics::instance().event( "$action", "$value" );
```

### Ads
To capture ads - correspondingly the shown ad.
```c++
Statistics::instance().ads( "$campain" );
```

### Move
To capture map shifts - correspondingly the new center.
```c++
Statistics::instance().move( "$latitude", "$longitude" );
```

### Open
To capture open websites or documents including the information which page or form has been requested.
```c++
Statistics::instance().open( "$urlOrName" );
```

### Play
To capture played files including the information which file/action has been played.
```c++
Statistics::instance().play( "$urlOrName" );
```

### Search
To capture searches including the information for what has been searched.
```c++
Statistics::instance().search( "$search" );
```

### Shake
To capture when the device was shaken.
```c++
Statistics::instance().shake();
```

### Touch
To capture typed/touched actions.
```c++
Statistics::instance().touch( "$action" );
```

# Compatiblity
## Qt Versions
- 6.4
- 6.3
- 6.2
- 6.1 (arm64 build not supported - Apple Silicon and Windows on ARM)
- 5.15

## Windows
- Windows 11
- Windows 10

## macOS
- macOS 12
- macOS 11
- macOS 10.15
- macOS 10.14
- macOS 10.13
- macOS 10.12

## Linux
- Red Hat 9
- Red Hat 8
- Ubuntu 22.04
- Ubuntu 20.04
- Ubuntu 18.04
- Every combination of supported Qt Version on a Linux system should work nicely

## iOS
- iOS 16
- iOS 15
- iOS 14
- iOS 13
- iOS 12

## Android
- API 16 4.1.1 and above

# Known Issues
## Windows
1. If you change the display ratio to a new value - e.g. from 100% to 150% - the change will take effect after re-login.

# Pending Issues
## App Store
We have already successfully passed the review process. If you have issues with the review process, please let us know and ask for support@vxstats.com to solve your needs.
