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
The macOS build will do a fair use check of the app, so it is essential to have OpenSSL installed. You can find OpenSSL using the OPENSSL_ROOT_DIR set. The easiest way is to install OpenSSL from brew (https://brew.sh/).
```bash
brew install openssl
```
Then you will find an OpenSSL installation here: /usr/local/Cellar/openssl@1.1/1.1.1g

# Installation
Use a Qt Creator to load CMakeList.txt from the checked-out folder, run the cmake enviroment, generate makefiles, and build it.
```bash
cmake ..
make -j`nproc`
```

## User-defined build variables
Inside cmake/env.cmake you will find some user-defined build variables for particular purposes.

### Debugging
You can log the verbose build output.
```cmake
# Debugging of build steps
set(CMAKE_VERBOSE_MAKEFILE ON)
```

### Gui dependencies
You can disable Qt Gui dependencies.
```cmake
# Build with or without Qt Gui dependency
set(GUI OFF)
```

### iOS
If you want to build for iOS or build with an iOS specified cmake toolchain (https://github.com/leetal/ios-cmake). You will find complete documentation here and can generate native XCode builds.
```cmake
# Enable iOS build
set(IOS ON)
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
- *6.2 (not officially released)*
- 6.1 (will miss Apple M1 support)
- *6.0 (will not be supported)*
- 5.15
- 5.14
- 5.13
- 5.12
- 5.11
- 5.10
- 5.9 (better version support)
- 5.8 (support ended)
- 5.7 (support ended)
- 5.6 (support ended)

## Windows
- Windows 10
- Windows 8.1
- Windows 8 (support ended, extended support ends 01/10/2023)
- Windows 7 (support ended)

## macOS
- macOS 11.0 (arm64 not yet supported by Qt)
- macOS 10.15
- macOS 10.14
- macOS 10.13
- macOS 10.12
- macOS 10.11
- macOS 10.10

## Linux
- Red Hat 8
- Ubuntu 20.04
- Ubuntu 18.04
- Ubuntu 16.04 (support ended, extended support ends 04/30/2021)
- Every combination of supported Qt Version on a Linux system should work nicely

## iOS
- iOS 14
- iOS 13
- iOS 12
- iOS 11
- iOS 10
- iOS 9
- iOS 8

## Android
- API 16 4.1.1 and above

# Known Issues
## Windows
1. If you change the display ratio to a new value - e.g. from 100% to 150% - the change will take effect after re-login.

# Pending Issues
## App Store
We have already successfully passed the review process. If you have issues with the review process, please let us know and ask for support@vxstats.com to solve your needs.
