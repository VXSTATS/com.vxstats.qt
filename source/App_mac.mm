/*
 * Copyright (C) 10/01/2020 VX STATS <sales@vxstats.com>
 *
 * This document is property of VX STATS. It is strictly prohibited
 * to modify, sell or publish it in any way. In case you have access
 * to this document, you are obligated to ensure its nondisclosure.
 * Noncompliances will be prosecuted.
 *
 * Diese Datei ist Eigentum der VX STATS. Jegliche Änderung, Verkauf
 * oder andere Verbreitung und Veröffentlichung ist strikt untersagt.
 * Falls Sie Zugang zu dieser Datei haben, sind Sie verpflichtet,
 * alles in Ihrer Macht stehende für deren Geheimhaltung zu tun.
 * Zuwiderhandlungen werden strafrechtlich verfolgt.
 */

/* openssl header */
#include <openssl/err.h>
#include <openssl/pkcs7.h>
#include <openssl/objects.h>
#include <openssl/sha.h>
#include <openssl/x509.h>

/* qt header */
#include <QFile>

/* objc header */
#include <Carbon/Carbon.h>

/* local header */
#include "App.h"

namespace vxstats {

  /**
   * @~english
   * @brief The App class for macOS.
   * General information about the running application including validation of
   * fair use.
   *
   * @~german
   * @brief Die Klasse App für macOS.
   * Bietet allgemeine Informationen über die Anwendung und eine Überprüfung
   * der fairen Verwendung.
   */
  class App_mac : public App {

  public:
    [[nodiscard]] bool fairUse() const final;
  };

  App &App::instance() {

    static App_mac app;
    return app;
  }

  bool App_mac::fairUse() const {

    NSBundle *mainBundle = [NSBundle mainBundle];
    NSURL *receiptURL = [mainBundle appStoreReceiptURL];
    NSError *receiptError = nullptr;
    BOOL isPresent = [receiptURL checkResourceIsReachableAndReturnError:&receiptError];
    if ( !isPresent ) {

      return false;
    }

    /* Load the receipt file */
    const NSData *receiptData = [NSData dataWithContentsOfURL:receiptURL];

    /* Create a memory buffer to extract the PKCS #7 container */
    std::unique_ptr<BIO, decltype( &BIO_free_all )> receiptBIO( BIO_new( BIO_s_mem() ), &BIO_free_all );
    BIO_write( receiptBIO.get(), [receiptData bytes], static_cast<int>( [receiptData length] ) );
    PKCS7 *receiptPKCS7 = d2i_PKCS7_bio( receiptBIO.get(), nullptr );
    if ( !receiptPKCS7 ) {

      return false;
    }

    /* Check that the container has a signature */
    if ( !PKCS7_type_is_signed( receiptPKCS7 ) ) {

      return false;
    }

    /* Check that the signed container has actual data */
    if ( !PKCS7_type_is_data( receiptPKCS7->d.sign->contents ) ) {

      return false;
    }

    /* Load the Apple Root CA (downloaded from https://www.apple.com/certificateauthority/) */
    QFile file( QString::fromLatin1( ":/AppleIncRootCertificate.cer" ) );
    file.open( QIODevice::ReadOnly );
    NSData *appleRootData = file.readAll().toNSData();
    file.close();

    std::unique_ptr<BIO, decltype( &BIO_free_all )> appleRootBIO( BIO_new( BIO_s_mem() ), &BIO_free_all );
    BIO_write( appleRootBIO.get(), static_cast<const void *>( [appleRootData bytes] ), static_cast<int>( [appleRootData length] ) );
    X509 *appleRootX509 = d2i_X509_bio( appleRootBIO.get(), nullptr );

    /* Create a certificate store */
    std::unique_ptr<X509_STORE, decltype( &X509_STORE_free )> store( X509_STORE_new(), &X509_STORE_free );
    X509_STORE_add_cert( store.get(), appleRootX509 );

    /* Be sure to load the digests before the verification */
    OpenSSL_add_all_digests();

    /* Check the signature */
    int result = PKCS7_verify( receiptPKCS7, nullptr, store.get(), nullptr, nullptr, 0 );
    return result == 1;
  }
}
