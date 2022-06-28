
include(FetchContent)
include(ExternalProject)

include(install-module)


##########################################################################
## cxxopts

if (NOT ${SPY_PACKAGED_BUILD})
  find_package(cxxopts QUIET)

  if (NOT ${cxxopts_FOUND})
    set(INSTALL_CXXOPTS TRUE)
  endif()
endif()

if ((${SPY_PACKAGED_BUILD}) OR (${INSTALL_CXXOPTS}))
  include(install/install-cxxopts)
  install_cxxopts()
  include_cxxopts()
endif()

##########################################################################
## jsoncpp

if (NOT ${SPY_PACKAGED_BUILD})
  find_package(jsoncpp QUIET)

  if (NOT ${jsoncpp_FOUND})
    set(INSTALL_JSONCPP TRUE)
  endif()
endif()

if ((${SPY_PACKAGED_BUILD}) OR (${INSTALL_JSONCPP}))
  include(install/install-jsoncpp)
  install_jsoncpp()
  include_jsoncpp()
endif()

##########################################################################
## plog

if (NOT ${SPY_PACKAGED_BUILD})
  find_package(plog QUIET)

  if (NOT ${plog_FOUND})
    set(INSTALL_PLOG TRUE)
  endif()
endif()
# 
if ((${SPY_PACKAGED_BUILD}) OR (${INSTALL_PLOG}))
  include(install/install-plog)
  install_plog()
  include_plog()
endif()

##########################################################################
## openssl

if (NOT ${SPY_PACKAGED_BUILD})
  find_package(OpenSSL QUIET)

  if (NOT ${OPENSSL_FOUND})
    set(INSTALL_OPENSSL ON)
  endif()
endif()

if ((${SPY_PACKAGED_BUILD}) OR (${INSTALL_OPENSSL}))
  include(install/install-openssl)
  install_openssl()
  include_openssl()
endif()

##########################################################################
## zlib

if (NOT ${SPY_PACKAGED_BUILD})
  find_package(ZLIB QUIET)

  if (NOT ${ZLIB_FOUND})
    set(INSTALL_ZLIB ON)
  endif()
endif()

if ((${SPY_PACKAGED_BUILD}) OR (${INSTALL_ZLIB}))
  include(install/install-zlib)
  install_zlib()
  include_zlib()
endif()

##########################################################################
## gperf

if (NOT ${SPY_PACKAGED_BUILD})
  find_program(GPERF_EXECUTABLE gperf)

  if (NOT ${GPERF_EXECUTABLE})
    set(INSTALL_GPERF ON)
  endif()
endif()

if ((${SPY_PACKAGED_BUILD}) OR (${INSTALL_GPERF}))
  include(install/install-gperf)
  install_gperf()
  include_gperf()
endif()

##########################################################################
## tdlib

if (NOT ${SPY_PACKAGED_BUILD})
  find_package(Td QUIET)

  if (NOT ${Td_FOUND})
    set(INSTALL_TD ON)
  endif()
endif()

if ((${SPY_PACKAGED_BUILD}) OR (${INSTALL_TD}))
  include(install/install-td)
  install_td()
  include_td()
endif()

