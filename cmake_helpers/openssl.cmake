IF (WIN32)
  file(TO_CMAKE_PATH ../3rdparty/openssl OPENSSL_ROOT_DIR)
ENDIF()

find_package(OpenSSL)
