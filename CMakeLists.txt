# CMake file

cmake_minimum_required(VERSION "2.8.9")

set (CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/CMakeModules")

include (CMakeModules/IncludeProductImage.cmake)

# Define project data 
set (APPLICATION_VENDOR_NAME "jet1oeil")
set (APPLICATION_VENDOR_DOMAIN "www.jet1oeil.com")
set (APPLICATION_NAME "OpenDBViewer")
set (APPLICATION_VERSION_MAJOR 1)
set (APPLICATION_VERSION_MINOR 1)
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

if(WIN32)
	set(AUTO_NEWLINE_STYLE WIN32)
else()
	set(AUTO_NEWLINE_STYLE UNIX)
endif()
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
elseif(APPLE)
	set(INSTALL_PATH_BIN ".")
	set(INSTALL_PATH_SHARE ".")
	set(INSTALL_PATH_FRAMEWORKS "../Frameworks")
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
set(QT_SQL_COMPONENTS qsqlite qsqlmysql qsqlpsql)

# Define output variable
set (PACKAGE_DEPS)

include (CMakeModules/IncludeLibraries.cmake)

# Install qt-mysql plugin
if(QT_FOUND AND WIN32 AND QT_USE_VERSION EQUAL 4)
    INSTALL(FILES
        "${PROJECT_SOURCE_DIR}/3rdparty/qt-mysql/libmysql.dll"
        DESTINATION ${INSTALL_PATH_BIN}
    )
    SET(TMPDLL "${QT_PLUGINS_DIR}/sqldrivers/qsqlmysql${QT_VERSION_MAJOR}.dll")
    if(NOT EXISTS ${TMPDLL} AND NOT MSVC)
        INSTALL(FILES
            "${PROJECT_SOURCE_DIR}/3rdparty/qt-mysql/qsqlmysql${QT_VERSION_MAJOR}.dll"
            DESTINATION ${INSTALL_PATH_BIN}/sqldrivers
            CONFIGURATIONS Release
        )
    endif()
    SET(TMPDLL "${QT_PLUGINS_DIR}/sqldrivers/qsqlmysqld${QT_VERSION_MAJOR}.dll")
    if(NOT EXISTS ${TMPDLL} AND NOT MSVC)
        INSTALL(FILES
            "${PROJECT_SOURCE_DIR}/3rdparty/qt-mysql/qsqlmysqld${QT_VERSION_MAJOR}.dll"
            DESTINATION ${INSTALL_PATH_BIN}/sqldrivers
            CONFIGURATIONS Debug
        )
    endif()
endif(QT_FOUND AND WIN32 AND QT_USE_VERSION EQUAL 4)

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
configure_file (
  "${PROJECT_SOURCE_DIR}/version.h.in"
  "${PROJECT_BINARY_DIR}/version.h"
)
add_definitions(-DHAVE_VERSION_H)
include_directories (${PROJECT_BINARY_DIR})

####################
# Add sources
####################

include_directories (${PROJECT_SOURCE_DIR}/src)

set(APPLICATION_MODEL_SOURCES
	src/Model/HistoryDatabaseList.cpp
	src/Model/HistoryDatabaseList.h
	src/Model/DatabaseModel.cpp
	src/Model/DatabaseModel.h
)

set(APPLICATION_SOURCES
	src/main.cpp
	src/Global/ApplicationSettings.cpp
	src/Global/ApplicationSettings.h
	src/GUI/QAboutDialog.cpp
	src/GUI/QAboutDialog.h
	src/GUI/QDatabaseTableView.cpp
	src/GUI/QDatabaseTableView.h
	src/GUI/QDatabaseConnectionView.cpp
	src/GUI/QDatabaseConnectionView.h
	src/GUI/QDatabaseWorksheetView.cpp
	src/GUI/QDatabaseWorksheetView.h
	src/GUI/QLineNumberAreaView.h
	src/GUI/QOpenDatabaseView.cpp
	src/GUI/QOpenDatabaseView.h
	src/GUI/QOpenHistoryView.cpp
	src/GUI/QOpenHistoryView.h
	src/GUI/QQueryResultView.cpp
	src/GUI/QQueryResultView.h
	src/GUI/QSqlSourceView.cpp
	src/GUI/QSqlSourceView.h
	src/GUI/QWindowMain.cpp
	src/GUI/QWindowMain.h
	src/GUIController/QWindowMainController.cpp
	src/GUIController/QWindowMainController.h
	src/GUIController/QDatabaseConnectionViewController.cpp
	src/GUIController/QDatabaseConnectionViewController.h
	src/GUIController/QDatabaseWorksheetViewController.cpp
	src/GUIController/QDatabaseWorksheetViewController.h
	src/GUIController/QDatabaseTableViewController.cpp
	src/GUIController/QDatabaseTableViewController.h
	src/GUIController/QOpenDatabaseViewController.cpp
	src/GUIController/QOpenDatabaseViewController.h
	src/GUIController/QOpenHistoryViewController.cpp
	src/GUIController/QOpenHistoryViewController.h
	src/GUIController/QSqlHighlighterController.cpp
	src/GUIController/QSqlHighlighterController.h
	src/GUIModel/QHistoryDatabaseItem.cpp
	src/GUIModel/QHistoryDatabaseItem.h
	src/GUIModel/QSqlDisplayQueryModel.cpp
	src/GUIModel/QSqlDisplayQueryModel.h
	src/GUIModel/QSqlDisplayTableModel.cpp
	src/GUIModel/QSqlDisplayTableModel.h
	src/GUIModel/QHistoryDatabaseModel.cpp
	src/GUIModel/QHistoryDatabaseModel.h
	src/GUIModel/HTMLDelegate.cpp
	src/GUIModel/HTMLDelegate.h
	src/Image/QIconThemeFallback.cpp
	src/Image/QIconThemeFallback.h
	src/Database/DatabaseController.cpp
	src/Database/DatabaseController.h
	src/Database/DatabaseControllerMysql.cpp
	src/Database/DatabaseControllerMysql.h
	src/Database/DatabaseControllerSqlite.cpp
	src/Database/DatabaseControllerSqlite.h
	src/Database/DatabaseControllerPostgreSQL.cpp
	src/Database/DatabaseControllerPostgreSQL.h
	src/Settings/QSettingsManager.cpp
	src/Settings/QSettingsManager.h
	src/Widget/QDropAreaWidget.cpp
	src/Widget/QDropAreaWidget.h
   	src/Widget/QFileExplorerWidget.cpp
   	src/Widget/QFileExplorerWidget.h
	src/Widget/QMidClickClosableTabWidget.cpp
	src/Widget/QMidClickClosableTabWidget.h
)

# Creating MOC file for sources file using "slots" or Q_OBJECT macro
set(APPLICATION_MOC_HEADERS
	src/GUI/QAboutDialog.h
	src/GUI/QDatabaseConnectionView.h
	src/GUI/QDatabaseTableView.h
	src/GUI/QDatabaseWorksheetView.h
	src/GUI/QOpenDatabaseView.h
    src/GUI/QOpenHistoryView.h
	src/GUI/QQueryResultView.h
	src/GUI/QWindowMain.h
	src/GUI/QSqlSourceView.h
	src/GUIController/QWindowMainController.h
	src/GUIController/QDatabaseConnectionViewController.h
	src/GUIController/QDatabaseTableViewController.h
	src/GUIController/QDatabaseWorksheetViewController.h
	src/GUIController/QOpenDatabaseViewController.h
    src/GUIController/QOpenHistoryViewController.h
	src/GUIController/QSqlHighlighterController.h
	src/Widget/QDropAreaWidget.h
   	src/Widget/QFileExplorerWidget.h
)
qt_wrap_cpp(
	APPLICATION_MOC_OUTFILES
	${APPLICATION_MOC_HEADERS}
)

####################
# i18n
####################

set (I18N_FILES
	src/main.cpp
	src/GUI/QAboutDialog.cpp
	src/GUI/QDatabaseConnectionView.cpp
	src/GUI/QDatabaseTableView.cpp
	src/GUI/QDatabaseWorksheetView.cpp
	src/GUI/QOpenDatabaseView.cpp
    src/GUI/QOpenHistoryView.cpp
	src/GUI/QQueryResultView.cpp
	src/GUI/QWindowMain.cpp
	src/GUIController/QDatabaseConnectionViewController.cpp
	src/GUIController/QDatabaseWorksheetViewController.cpp
	src/GUIController/QOpenDatabaseViewController.cpp
	src/GUIController/QOpenHistoryViewController.cpp
	src/GUIController/QWindowMainController.cpp
	src/Widget/QDropAreaWidget.cpp
   	src/Widget/QFileExplorerWidget.cpp
)

set (TS_FILES
    ts/fr.ts
)

option (UPDATE_TRANSLATIONS "Update source translation ts/*.ts files (WARNING: make clean will delete the source .ts files! Danger!)" ON)
if (UPDATE_TRANSLATIONS)
  qt_create_translation(qm_FILES ${TS_FILES} ${I18N_FILES})
else (UPDATE_TRANSLATIONS)
  qt_add_translation(qm_FILES ${TS_FILES})
endif (UPDATE_TRANSLATIONS)

add_custom_target(update_ts ALL DEPENDS ${qm_FILES})

####################
# Resource files
####################

# Configure translation file
configure_file (
  "${PROJECT_SOURCE_DIR}/ts.qrc.in"
  "${PROJECT_BINARY_DIR}/ts.qrc"
)

# Theme resource file
product_image_generate_icon(${PROJECT_BINARY_DIR}/data/theme/icons_src/svg ${PROJECT_BINARY_DIR}/data/theme/icons/app_default)

# Icon resource file
file(COPY ${PROJECT_SOURCE_DIR}/data/theme/icons_src/author DESTINATION ${PROJECT_BINARY_DIR}/data/theme/icons/app_default)
file(COPY ${PROJECT_SOURCE_DIR}/data/theme/icons_src/index.theme DESTINATION ${PROJECT_BINARY_DIR}/data/theme/icons/app_default)

# Resources
configure_file (
  "${PROJECT_SOURCE_DIR}/data/logos/icon.rc.in"
  "${PROJECT_BINARY_DIR}/data/logos/icon.rc"
)
configure_file (
  "${PROJECT_SOURCE_DIR}/data/app.qrc.in"
  "${PROJECT_BINARY_DIR}/data/app.qrc"
)

set (QRC_FILES
	${PROJECT_BINARY_DIR}/ts.qrc
	${PROJECT_BINARY_DIR}/data/app.qrc
)
QT_ADD_RESOURCES(RCS_OUTFILES ${QRC_FILES})

####################
# Create Executable
####################

add_executable (${APPLICATION_EXE_NAME}
	${APPLICATION_SOURCES}
	${APPLICATION_MODEL_SOURCES}
	${APPLICATION_MOC_OUTFILES}
    ${RCS_OUTFILES}
	${qm_FILES}
)
target_link_libraries (${APPLICATION_EXE_NAME}
    ${QT_LIBRARIES}
)
install(TARGETS ${APPLICATION_EXE_NAME} DESTINATION ${INSTALL_PATH_BIN})

####################
# Unit test
####################

set (UNITTEST_SOURCE
	test/main.cpp
	test/Model/HistoryDatabaseListTestCase.cpp
	test/Model/HistoryDatabaseListTestCase.h
)

set (UNITTEST_MOC_HEADERS
	test/Model/HistoryDatabaseListTestCase.h
)

qt_wrap_cpp(
	UNITTEST_MOC_OUTFILES
	${UNITTEST_MOC_HEADERS}
)

set(UNITTEST_EXE ${APPLICATION_EXE_NAME}-testsuite)

add_executable(${UNITTEST_EXE}
	${UNITTEST_SOURCE}
	${UNITTEST_MOC_OUTFILES}
	${APPLICATION_MODEL_SOURCES}
    ${RCS_OUTFILES}
)

set_target_properties(${UNITTEST_EXE} PROPERTIES COMPILE_FLAGS -DUNITTEST)

target_link_libraries(${UNITTEST_EXE}
	${QT_LIBRARIES}
)

############################
# Common files installation
############################

# Documentation file
set(DOCFILES_EXT "")
if(WIN32)
	set(DOCFILES_EXT ".txt")
endif(WIN32)

# README file
configure_file (
  "${PROJECT_SOURCE_DIR}/README.md"
  "${PROJECT_BINARY_DIR}/README${DOCFILES_EXT}"
  NEWLINE_STYLE ${AUTO_NEWLINE_STYLE}
)
install(FILES "${PROJECT_BINARY_DIR}/README${DOCFILES_EXT}" DESTINATION ${INSTALL_PATH_DOC})

# COPYING file
configure_file (
  "${PROJECT_SOURCE_DIR}/COPYING"
  "${PROJECT_BINARY_DIR}/COPYING${DOCFILES_EXT}"
  NEWLINE_STYLE ${AUTO_NEWLINE_STYLE}
)
# ChangeLog file
set(CHANGELOG "ChangeLog")
install(FILES "${PROJECT_SOURCE_DIR}/${CHANGELOG}" DESTINATION ${INSTALL_PATH_DOC} RENAME ChangeLog${DOCFILES_EXT})

# Install icons theme
install(DIRECTORY ${PROJECT_BINARY_DIR}/data/theme/icons/
    DESTINATION ${INSTALL_PATH_SHARE}/icons_themes/
    PATTERN ".svn" EXCLUDE
    PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ GROUP_EXECUTE GROUP_READ WORLD_READ WORLD_EXECUTE
)

# Desktop file
configure_file (
  "${PROJECT_SOURCE_DIR}/data/desktop.in"
  "${PROJECT_BINARY_DIR}/data/${APPLICATION_PACKAGE_NAME}.desktop"
)
if(UNIX AND NOT WIN32)
    install(FILES ${PROJECT_BINARY_DIR}/data/${APPLICATION_PACKAGE_NAME}.desktop DESTINATION ${INSTALL_PATH_APPLICATIONS})
endif(UNIX AND NOT WIN32)

#################
# Build package
#################

INCLUDE(InstallRequiredSystemLibraries)

# CPack confile file
configure_file(${PROJECT_SOURCE_DIR}/CPackOptions.cmake.in
    ${PROJECT_BINARY_DIR}/CPackOptions.cmake
    @ONLY)
set(CPACK_PROJECT_CONFIG_FILE ${CMAKE_BINARY_DIR}/CPackOptions.cmake)

if(WIN32)
    if(CMAKE_BUILD_TYPE MATCHES Debug)
        SET(CPACK_GENERATOR ZIP)
    else()
        SET(CPACK_GENERATOR ZIP NSIS)
    endif()
elseif(APPLE)
    configure_file(${PROJECT_SOURCE_DIR}/platforms/osx/Info.plist.in
        ${PROJECT_BINARY_DIR}/platforms/osx/Info.plist
        @ONLY)
    configure_file(${PROJECT_SOURCE_DIR}/platforms/osx/run.sh.in
        ${PROJECT_BINARY_DIR}/platforms/osx/${APPLICATION_NAME}
        @ONLY)
    SET(CPACK_GENERATOR Bundle)
else()
    SET(CPACK_GENERATOR TGZ DEB)
endif(WIN32)

SET(CPACK_PACKAGE_NAME ${APPLICATION_PACKAGE_NAME})
SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY ${APPLICATION_DESCRIPTION_SUMMARY})
SET(CPACK_PACKAGE_VENDOR ${APPLICATION_VENDOR_NAME})
SET(CPACK_PACKAGE_CONTACT ${APPLICATION_CONTACT})
SET(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_BINARY_DIR}/README${DOCFILES_EXT}")
SET(CPACK_PACKAGE_VERSION_MAJOR ${APPLICATION_VERSION_MAJOR})
SET(CPACK_PACKAGE_VERSION_MINOR ${APPLICATION_VERSION_MINOR})
SET(CPACK_PACKAGE_VERSION_PATCH ${APPLICATION_VERSION_PATCH}${APPLICATION_VERSION_BUILD})
SET(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_BINARY_DIR}/COPYING${DOCFILES_EXT}")
SET(CPACK_PACKAGE_INSTALL_DIRECTORY ${APPLICATION_PACKAGE_NAME}) # Useful for registry key and NSIS auto uninstaller

SET(CPACK_STRIP_FILES "${APPLICATION_EXE_NAME}")
SET(CPACK_SOURCE_STRIP_FILES "")
if(WIN32)
	# NSIS
	SET(CPACK_NSIS_DISPLAY_NAME "${APPLICATION_NAME}")
elseif(APPLE)
    if (NOT CPACK_SYSTEM_NAME)
        IF(${CMAKE_SYSTEM_PROCESSOR} STREQUAL "x86_64")
            SET(CPACK_SYSTEM_NAME amd64)
        ELSEIF(${CMAKE_SYSTEM_PROCESSOR} STREQUAL "i686")
            SET(CPACK_SYSTEM_NAME i386)
        ENDIF(${CMAKE_SYSTEM_PROCESSOR} STREQUAL "x86_64")
    endif()

    SET(CPACK_BUNDLE_NAME "${APPLICATION_NAME}")
else()
	if (NOT CPACK_SYSTEM_NAME)
		IF(${CMAKE_SYSTEM_PROCESSOR} STREQUAL "x86_64")
			SET(CPACK_SYSTEM_NAME amd64)
		ELSEIF(${CMAKE_SYSTEM_PROCESSOR} STREQUAL "i686")
			SET(CPACK_SYSTEM_NAME i386)
		ENDIF(${CMAKE_SYSTEM_PROCESSOR} STREQUAL "x86_64")
	endif()

	JOINLIST(PACKAGE_DEPS ", " DEBIAN_DEPS)

	SET(CPACK_DEBIAN_PACKAGE_NAME ${APPLICATION_PACKAGE_NAME})
	SET(CPACK_DEBIAN_PACKAGE_VERSION ${APPLICATION_VERSION})
	SET(CPACK_DEBIAN_PACKAGE_DEPENDS ${DEBIAN_DEPS})
	SET(CPACK_DEBIAN_PACKAGE_SECTION "misc")
	# SET(CPACK_DEBIAN_PACKAGE_ARCHITECTURE ${DEB_ARCH})
ENDIF()
SET(CPACK_PACKAGE_EXECUTABLES "${APPLICATION_EXE_NAME}" "${APPLICATION_NAME}")
include(CPack)
