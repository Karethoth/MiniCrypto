IF (WIN32)
  file(TO_CMAKE_PATH "${CMAKE_CURRENT_LIST_DIR}/../3rdparty/openssl" OPENSSL_ROOT_DIR)
  set(OPENSSL_INCLUDE_DIR "${OPENSSL_ROOT_DIR}/include")
  set(OPENSSL_LIBRARIES "${OPENSSL_ROOT_DIR}/lib")
ENDIF()

find_package(OpenSSL REQUIRED)

if (NOT OPENSSL_FOUND)
  message(FATAL_ERROR "OpenSSL not found!")
else()
  message(STATUS "OpenSSL found: ${OPENSSL_VERSION}")
  message(STATUS "OpenSSL include dir: ${OPENSSL_INCLUDE_DIR}")
  message(STATUS "OpenSSL libraries: ${OPENSSL_LIBRARIES}")
endif()

