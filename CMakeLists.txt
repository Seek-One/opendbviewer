# CMake file

cmake_minimum_required(VERSION "2.8.9")

set (CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/CMakeModules")

# Define project data 
set (APPLICATION_VENDOR_NAME "jet1oeil")
set (APPLICATION_VENDOR_DOMAIN "www.jet1oeil.com")
set (APPLICATION_NAME "OpenDBViewer")
set (APPLICATION_VERSION_MAJOR 1)
set (APPLICATION_VERSION_MINOR 0)
set (APPLICATION_VERSION_PATCH 0)
if(NOT DEFINED APPLICATION_VERSION_BUILD)
	set(APPLICATION_VERSION_BUILD "")
endif()
if(CMAKE_BUILD_TYPE MATCHES Debug)
	set(APPLICATION_VERSION_BUILD ${APPLICATION_VERSION_BUILD}dbg)
endif()
set (APPLICATION_VERSION ${APPLICATION_VERSION_MAJOR}.${APPLICATION_VERSION_MINOR}.${APPLICATION_VERSION_PATCH}${APPLICATION_VERSION_BUILD})
set (APPLICATION_CONTACTMAIL "opendbviewer")
set (APPLICATION_CONTACT "Contact Jet1oeil <${APPLICATION_CONTACTMAIL}>")
set (APPLICATION_PACKAGE_VENDOR "jet1oeil")
set (APPLICATION_PACKAGE_NAME "opendbviewer")
set (APPLICATION_EXE_NAME opendbviewer)

project (opendbviewer CXX C)

#set (CMAKE_VERBOSE_MAKEFILE OFF)

if(CMAKE_HOST_SYSTEM MATCHES Windows)
	set(HOST_WIN32 TRUE)
else()
	set(HOST_WIN32 FALSE)
endif()

# Define install paths
if(WIN32)
	set(INSTALL_PATH_BIN ".")
	set(INSTALL_PATH_SHARE ".")
	set(INSTALL_PATH_DOC ".")
	set(INSTALL_PATH_APPLICATIONS ".")
	set(INSTALL_PATH_ICONS ".")
else(WIN32)
	set(INSTALL_PATH_BIN "bin/")
	set(INSTALL_PATH_SHARE "share/${APPLICATION_PACKAGE_NAME}")
	set(INSTALL_PATH_DOC "share/${APPLICATION_PACKAGE_NAME}/doc")
	set(INSTALL_PATH_APPLICATIONS "share/applications")
	set(INSTALL_PATH_ICONS "share/icons/")
endif(WIN32)

##################
# Define options
##################

if(NOT MSVC)
	set (CMAKE_C_FLAGS -std=gnu89)
	set (CMAKE_CXX_FLAGS -std=gnu++98)
	add_definitions (-Wall)
endif(NOT MSVC)

# Platform check from standard C
include (CheckIncludeFiles)
CHECK_INCLUDE_FILES (stdlib.h HAVE_STDLIB_H)
CHECK_INCLUDE_FILES (stdio.h HAVE_STDIO_H)
CHECK_INCLUDE_FILES (stddef.h HAVE_STDDEF_H)
CHECK_INCLUDE_FILES (string.h HAVE_STRING_H)
CHECK_INCLUDE_FILES (strings.h HAVE_STRINGS_H)
CHECK_INCLUDE_FILES (math.h HAVE_MATH_H)
CHECK_INCLUDE_FILES (regex.h HAVE_REGEX_H)
CHECK_INCLUDE_FILES (byteswap.h HAVE_BYTESWAP_H)
CHECK_INCLUDE_FILES (sys/time.h HAVE_SYS_TIME_H)

if (HAVE_STRING_H AND HAVE_STDLIB_H AND HAVE_STDIO_H AND HAVE_STDDEF_H AND HAVE_MATH_H)
  message(STATUS "All required headers are found")
else()
  message(FATAL_ERROR "Some required headers are not found")
endif()

include (CheckFunctionExists)
CHECK_FUNCTION_EXISTS (strdup HAVE_STRDUP)
CHECK_FUNCTION_EXISTS (strcasestr HAVE_STRCASESTR)
CHECK_FUNCTION_EXISTS (strcasecmp HAVE_STRCASECMP)
CHECK_FUNCTION_EXISTS (strncasecmp HAVE_STRNCASECMP)
CHECK_FUNCTION_EXISTS (_stricmp HAVE_STRICMP)
CHECK_FUNCTION_EXISTS (_strnicmp HAVE_STRNICMP)
CHECK_FUNCTION_EXISTS (strndup HAVE_STRNDUP)
CHECK_FUNCTION_EXISTS (strtok_r HAVE_STRTOK_R)
CHECK_FUNCTION_EXISTS (abs HAVE_ABS)
#CHECK_FUNCTION_EXISTS (fabs HAVE_FABS)

if (HAVE_STRDUP AND (HAVE_STRCASESTR OR HAVE_STRICMP) AND (HAVE_STRNCASECMP OR HAVE_STRNICMP) AND HAVE_ABS)
  message(STATUS "All required functions are found")
else()
  message(FATAL_ERROR "Some required functions are not found")
endif()

####################
# Library checking
####################

# Compiler
set(WITH_COMPILER_LIBRARIES ON CACHE BOOL "Enable build with compiler library")
set(WITH_PTHREAD ON CACHE BOOL "Enable build with pthread library")
# GUI
set(WITH_QT ON CACHE BOOL "Enable build with qt library")
#set(QT_USE_VERSION 5)
set(QT4_COMPONENTS QtCore QtGui QtNetwork QtSql QtTest)
set(QT5_COMPONENTS Core Gui Widgets Network Sql Test)
set(QT_IMAGEFORMATS_COMPONENTS qjpeg)

# Define output variable
set (PACKAGE_DEPS)

include (CMakeModules/IncludeLibraries.cmake)

####################
# Config file
####################

# Configure a header file to pass some of the CMake settings
# to the source code
configure_file (
  "${PROJECT_SOURCE_DIR}/config.h.in"
  "${PROJECT_BINARY_DIR}/config.h"
  )
add_definitions(-DHAVE_CONFIG_H)
include_directories (${PROJECT_BINARY_DIR})

####################
# Add sources
####################

include_directories (${PROJECT_SOURCE_DIR}/src)

####################
# Build test suite
####################

SET (APPLICATION_SRC 
    src/main.cpp
	src/GUI/QWindowMain.cpp
	src/GUI/QWindowMain.h
	src/GUI/QDatabaseTableView.cpp
	src/GUI/QDatabaseTableView.h
	src/GUI/QDatabaseConnectionView.cpp
	src/GUI/QDatabaseConnectionView.h
	src/GUI/QDatabaseWorksheetView.cpp
	src/GUI/QDatabaseWorksheetView.h
	src/GUI/QDatabaseSelectionView.cpp
	src/GUI/QDatabaseSelectionView.h
	src/GUIController/QWindowMainController.cpp
	src/GUIController/QWindowMainController.h
	src/GUIController/QDatabaseConnectionViewController.cpp
	src/GUIController/QDatabaseConnectionViewController.h
	src/GUIController/QDatabaseWorksheetViewController.cpp
	src/GUIController/QDatabaseWorksheetViewController.h
	src/GUIController/QDatabaseSelectionViewController.cpp
	src/GUIController/QDatabaseSelectionViewController.h
	src/Database/DatabaseController.cpp
	src/Database/DatabaseController.h
)
    
# Creating MOC file for sources file using "slots" or Q_OBJECT macro
set(APPLICATION_MOC_HEADERS
    src/GUI/QWindowMain.h
	src/GUIController/QWindowMainController.h
	src/GUIController/QDatabaseConnectionViewController.h
	src/GUIController/QDatabaseSelectionViewController.h
)
qt_wrap_cpp(
  APPLICATION_MOC_OUTFILES
  ${APPLICATION_MOC_HEADERS}
)

set (APPLICATION_EXE ${APPLICATION_EXE_NAME})
add_executable (${APPLICATION_EXE} ${APPLICATION_SRC}
    ${APPLICATION_MOC_OUTFILES}
)
target_link_libraries (${APPLICATION_EXE}
    ${QT_LIBRARIES}
)
#install(TARGETS ${APPLICATION_EXE_NAME} DESTINATION ${APPLICATION_PATH_BIN})
