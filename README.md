* [Preparation](#preparation)
* [Installation](#installation)
   * [Android](#android)
   * [macOS](#macos)
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
   * [Linux](#linux)
   * [Android](#android-1)
   * [macOS](#macos-1)
   * [iOS](#ios-1)
* [Known Issues](#known-issues)
   * [Windows](#windows-1)
* [Pending Issues](#pending-issues)
   * [App Store](#app-store)

# Preparation

# Installation
Write down all installation rules and dependencies

## Android
## macOS
## iOS

# Implementation
## Pre-Setup
Qt application name and version.
```c++
QCoreApplication::setApplicationName( "$appid" );
QCoreApplication::setApplicationVersion( "$version" );
```

Alternatially use direct setup without Qt meta system.
```c++
VX::App::instance().setIdentifier( "$appid" );
VX::App::instance().setVersion( "$version" );
```

Setup your build id is only directly possible, Qt will not support such a value directly.
```c++
VX::App::instance().setBuild( "$build" );
```

## Setup
Setup your environment with your credentials. Please insert your username, password and url here. For defuscation please follow our best practice documentation.
```c++
VX::Statistics::instance().setServerFilePath( "https://$username:$password@$url/" );
```

## Page
This is the global context, where you are currently on in your application. Just name it easy and with logical app structure to identify where the user stays.
```c++
VX::Statistics::instance().page( "Main" );
```

## Event
When you would like to request a page with dynamic content please use this function.
```c++
VX::Statistics::instance().event( "$action", "$value" );
```

### Ads
To capture ads - correspondingly the shown ad.
```c++
VX::Statistics::instance().ads( "$campain" );
```

### Move
To capture map shifts - correspondingly the new center.
```c++
VX::Statistics::instance().move( "$latitude", "$longitude" );
```

### Open
To capture open websites or documents including the information which page or document has been requested.
```c++
VX::Statistics::instance().open( "$urlOrName" );
```

### Play
To capture played files including the information which file/action has been played.
```c++
VX::Statistics::instance().play( "$urlOrName" );
```

### Search
To capture searches including the information for which has been searched.
```c++
VX::Statistics::instance().play( "$search" );
```

### Shake
To capture when the device has been shaken.
```c++
VX::Statistics::instance().shake();
```

### Touch
To capture typed/touched actions.
```c++
VX::Statistics::instance().touch( "$action" );
```

# Compatiblity
## Qt Versions
- *6 (not yet supported)*
- 5.15
- 5.14
- 5.13
- 5.12
- 5.11
- 5.10
- 5.9 (better version support, we recommend to use a version above 5.9)
- 5.8
- 5.7
- 5.6

## Windows
- Windows 10
- Windows 8.1
- Windows 8 (extended support ends 10.01.2023)
- Windows 7 (support ended)

## Linux
- Red Hat 8.2
- Ubuntu 20.04
- Ubuntu 18.04
- Ubuntu 16.04
- Every combination of supported Qt Version on a linux system should work nicely

## Android
- API 16 4.1.1 and above

## macOS
- *macOS 10.16  - (not yet supported)*
- macOS 10.15
- macOS 10.14
- macOS 10.13
- macOS 10.12
- macOS 10.11
- macOS 10.10
- macOS 10.9

## iOS
- *iOS 14 - (not yet supported)*
- iOS 13
- iOS 12
- iOS 11
- iOS 10
- iOS 9
- iOS 8
- iOS 7

# Known Issues
## Windows
1. If you change the display ratio to a new value - maybe from 100% to 150% - the change will first take effect after relogin.

# Pending Issues
## App Store
Maybe it is not possible for a possible App Store review with every Qt Version, because we are using some settings not in regular app context. So this has to be changed, if you have issues of the review process, please let us know and ask for support@vxapps.com to solve your needs.
