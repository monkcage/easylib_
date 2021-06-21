cmake_minimum_required(VERSION 3.5)
project(easy)

list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake_modules/)

set(EASY_MAJOR_VERSION 1)
set(EASY_MINOR_VERSION 4)
set(EASY_PATCH_VERSION 1)
set(EASY_VERSION
    ${EASY_MAJOR_VERSION}.${EASY_MINOR_VERSION}.${EASY_PATCH_VERSION})

# Offer the user the choice of overriding the installation directories
set(INSTALL_LIB_DIR lib CACHE PATH "Installation directory for libraries")
set(INSTALL_INCLUDE_DIR
    include
    CACHE PATH "Installation directory for header files")
set(DEF_INSTALL_EASY_CMAKE_DIR lib/cmake/EASY)
set(INSTALL_EASY_CMAKE_DIR
    ${DEF_INSTALL_EASY_CMAKE_DIR}
    CACHE PATH "Installation directory for cmake files")

if(BUILD_EASY_SHARED)
  set(CMAKE_POSITION_INDEPENDENT_CODE TRUE)
  list(FIND CMAKE_PLATFORM_IMPLICIT_LINK_DIRECTORIES
            "${CMAKE_INSTALL_PREFIX}/${INSTALL_LIB_DIR}" isSystemDir)
  if("${isSystemDir}" STREQUAL "-1")
    set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/${INSTALL_LIB_DIR}")
  endif("${isSystemDir}" STREQUAL "-1")
  add_library(${PROJECT_NAME} SHARED)
  set_target_properties(${PROJECT_NAME} PROPERTIES
      VERSION ${EASY_VERSION}
      SOVERSION ${EASY_MAJOR_VERSION})
  if(CMAKE_CXX_COMPILER_ID MATCHES MSVC)
    # Ignore MSVC C4251 and C4275 warning of exporting std objects with no dll export
    # We export class to facilitate maintenance, thus if you compile
    # drogon on windows as a shared library, you will need to use
    # exact same compiler for drogon and your app.
    target_compile_options(${PROJECT_NAME} PUBLIC /wd4251 /wd4275)
  endif()
else(BUILD_EASY_SHARED)
  add_library(${PROJECT_NAME} STATIC)
endif(BUILD_EASY_SHARED)

if (CMAKE_CXX_COMPILER_ID MATCHES GNU)
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Werror)
endif()

include(GenerateExportHeader)
generate_export_header(${PROJECT_NAME} EXPORT_FILE_NAME ${CMAKE_CURRENT_BINARY_DIR}/exports/easy/exports.h)

# include directories
target_include_directories(
  ${PROJECT_NAME}
  PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>
         $<INSTALL_INTERFACE:${INSTALL_INCLUDE_DIR}>
         $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/exports>
  PRIVATE ${PROJECT_SOURCE_DIR}
          ${PROJECT_SOURCE_DIR}/easy/utils
          ${PROJECT_SOURCE_DIR}/easy/net
          ${PROJECT_SOURCE_DIR}/easy/net/inner
          $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/third_party/wepoll>)

if(MINGW)
  target_compile_definitions(
    ${PROJECT_NAME}
    PUBLIC -D_WIN32_WINNT=0x0601)
endif(MINGW)

set(EASY_SOURCES
    easy/utils/AsyncFileLogger.cc
    easy/utils/ConcurrentTaskQueue.cc
    easy/utils/Date.cc
    easy/utils/LogStream.cc
    easy/utils/Logger.cc
    easy/utils/MsgBuffer.cc
    easy/utils/SerialTaskQueue.cc
    easy/utils/TimingWheel.cc
    easy/net/EventLoop.cc
    easy/net/EventLoopThread.cc
    easy/net/EventLoopThreadPool.cc
    easy/net/InetAddress.cc
    easy/net/TcpClient.cc
    easy/net/TcpServer.cc
    easy/net/Channel.cc
    easy/net/inner/Acceptor.cc
    easy/net/inner/Connector.cc
    easy/net/inner/Poller.cc
    easy/net/inner/Socket.cc
    easy/net/inner/TcpConnectionImpl.cc
    easy/net/inner/Timer.cc
    easy/net/inner/TimerQueue.cc
    easy/net/inner/poller/EpollPoller.cc
    easy/net/inner/poller/KQueue.cc)

if(WIN32)
  set(EASY_SOURCES ${EASY_SOURCES} easy/utils/WindowsSupport.cc)
  set(EASY_SOURCES ${EASY_SOURCES} third_party/wepoll/Wepoll.c)
endif(WIN32)

find_package(OpenSSL)
if(OpenSSL_FOUND)
  target_link_libraries(${PROJECT_NAME} PRIVATE OpenSSL::SSL OpenSSL::Crypto)
  target_compile_definitions(${PROJECT_NAME} PRIVATE USE_OPENSSL)
endif()

find_package(c-ares)
if(c-ares_FOUND)
  message(STATUS "c-ares found!")
  target_link_libraries(${PROJECT_NAME} PRIVATE c-ares_lib)
  set(EASY_SOURCES ${EASY_SOURCES} easy/net/inner/AresResolver.cc)
else()
  set(EASY_SOURCES ${EASY_SOURCES} easy/net/inner/NormalResolver.cc)
endif()
target_sources(${PROJECT_NAME} PRIVATE ${EASY_SOURCES})

find_package(Threads)
target_link_libraries(${PROJECT_NAME} PUBLIC Threads::Threads)
if(WIN32)
  target_link_libraries(${PROJECT_NAME} PRIVATE ws2_32 Rpcrt4)
  if(OpenSSL_FOUND)
    target_link_libraries(${PROJECT_NAME} PRIVATE Crypt32 Secur32)
  endif(OpenSSL_FOUND)
endif(WIN32)

set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 14)
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD_REQUIRED ON)
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_EXTENSIONS OFF)
set_target_properties(${PROJECT_NAME} PROPERTIES EXPORT_NAME easy)

if(BUILD_TESTING)
  add_subdirectory(easy/tests)
  find_package(GTest)
  if(GTest_FOUND)
    enable_testing()
    add_subdirectory(easy/unittests)
  endif()
endif()

set(public_net_headers
    easy/net/EventLoop.h
    easy/net/EventLoopThread.h
    easy/net/EventLoopThreadPool.h
    easy/net/InetAddress.h
    easy/net/TcpClient.h
    easy/net/TcpConnection.h
    easy/net/TcpServer.h
    easy/net/callbacks.h
    easy/net/Resolver.h
    easy/net/Channel.h)

set(public_utils_headers
    easy/utils/AsyncFileLogger.h
    easy/utils/ConcurrentTaskQueue.h
    easy/utils/Date.h
    easy/utils/Funcs.h
    easy/utils/LockFreeQueue.h
    easy/utils/LogStream.h
    easy/utils/Logger.h
    easy/utils/MsgBuffer.h
    easy/utils/NonCopyable.h
    easy/utils/ObjectPool.h
    easy/utils/SerialTaskQueue.h
    easy/utils/TaskQueue.h
    easy/utils/TimingWheel.h)

source_group("Public API"
             FILES
             ${public_net_headers}
             ${public_utils_headers})

install(TARGETS easy
                # IMPORTANT: Add the easy library to the "export-set"
        EXPORT EasyTargets
        RUNTIME DESTINATION "${INSTALL_BIN_DIR}" COMPONENT bin
        ARCHIVE DESTINATION "${INSTALL_LIB_DIR}" COMPONENT lib
        LIBRARY DESTINATION "${INSTALL_LIB_DIR}" COMPONENT lib)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/exports/easy/exports.h
        DESTINATION ${INSTALL_INCLUDE_DIR}/easy)
install(FILES ${public_net_headers}
        DESTINATION ${INSTALL_INCLUDE_DIR}/easy/net)
install(FILES ${public_utils_headers}
        DESTINATION ${INSTALL_INCLUDE_DIR}/easy/utils)

include(CMakePackageConfigHelpers)
# ... for the install tree
configure_package_config_file(
  cmake/templates/EasyConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/EasyConfig.cmake
  INSTALL_DESTINATION
  ${INSTALL_EASY_CMAKE_DIR})

# version
write_basic_package_version_file(
  ${CMAKE_CURRENT_BINARY_DIR}/EasyConfigVersion.cmake
  VERSION ${EASY_VERSION}
  COMPATIBILITY SameMajorVersion)

# Install the EasyConfig.cmake and EasyConfigVersion.cmake
install(
  FILES
    "${CMAKE_CURRENT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/EasyConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/EasyConfigVersion.cmake"
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake_modules/Findc-ares.cmake"
  DESTINATION "${INSTALL_EASY_CMAKE_DIR}"
  COMPONENT dev)

# Install the export set for use with the install-tree
install(EXPORT EasyTargets
        DESTINATION "${INSTALL_EASY_CMAKE_DIR}"
        NAMESPACE Easy::
        COMPONENT dev)
