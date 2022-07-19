
## Find ssl library
find_library(OPENSSL_SSL_LIBRARY
  NAMES
    ssl-1_1-x64
    ssl-1_1-x32
    ssl
  PATHS
    ${OPENSSL_ROOT_DIR}
    ${OPENSSL_ROOT_DIR}/bin
    ${OPENSSL_ROOT_DIR}/lib
  NO_DEFAULT_PATH
)

## Find crypto library
find_library(OPENSSL_CRYPTO_LIBRARY
  NAMES
    crypto-1_1-x64
    crypto-1_1-x32
    crypto
  PATHS
    ${OPENSSL_ROOT_DIR}
    ${OPENSSL_ROOT_DIR}/bin
    ${OPENSSL_ROOT_DIR}/lib
  NO_DEFAULT_PATH
)

set(OPENSSL_LIBRARIES ${OPENSSL_SSL_LIBRARY} ${OPENSSL_CRYPTO_LIBRARY})

## Find include directory
find_path(OPENSSL_INCLUDE_DIR openssl/opensslv.h
  PATHS
    ${OPENSSL_ROOT_DIR}
    ${OPENSSL_ROOT_DIR}/include
  NO_DEFAULT_PATH
)

set(OPENSSL_FOUND TRUE)

mark_as_advanced(
  OPENSSL_FOUND
  OPENSSL_INCLUDE_DIR
  OPENSSL_CRYPTO_LIBRARY
  OPENSSL_SSL_LIBRARY
  OPENSSL_LIBRARIES
)

