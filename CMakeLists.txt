cmake_minimum_required(VERSION 3.20)
project(blockchain CXX)

set(CMAKE_CXX_STANDARD 20)

set(OPENSSL_ROOT_DIR "C:/Program Files/OpenSSL-Win64")

set(OPENSSL_INCLUDE_DIR "${OPENSSL_ROOT_DIR}/include")
set(OPENSSL_CRYPTO_LIBRARY "${OPENSSL_ROOT_DIR}/lib/VC/x64/MD/libcrypto.lib")

add_executable(blockchain main.cpp BlockChain.cpp tests.cpp)

target_include_directories(blockchain PRIVATE ${OPENSSL_INCLUDE_DIR})
target_link_libraries(blockchain PRIVATE ${OPENSSL_CRYPTO_LIBRARY})
