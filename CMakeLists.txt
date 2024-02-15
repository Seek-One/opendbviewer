# CMake file

cmake_minimum_required(VERSION "3.13.4")

set (CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/CMakeModules")

# Define project data 
set (APPLICATION_VENDOR_NAME "seekone")
set (APPLICATION_VENDOR_DOMAIN "www.seek-one.fr")
set (APPLICATION_NAME "OpenDBViewer")
set (APPLICATION_VERSION_MAJOR 1)
set (APPLICATION_VERSION_MINOR 1)
set (APPLICATION_VERSION_PATCH 2)
if(NOT DEFINED APPLICATION_VERSION_BUILD)
	set(APPLICATION_VERSION_BUILD "")
endif()
if(CMAKE_BUILD_TYPE MATCHES Debug)
	set(APPLICATION_VERSION_BUILD ${APPLICATION_VERSION_BUILD}dbg)
endif()
set (APPLICATION_VERSION ${APPLICATION_VERSION_MAJOR}.${APPLICATION_VERSION_MINOR}.${APPLICATION_VERSION_PATCH}${APPLICATION_VERSION_BUILD})
set (APPLICATION_CONTACTMAIL "contact@seek-one.fr")
set (APPLICATION_CONTACT "Contact Seek-One <${APPLICATION_CONTACTMAIL}>")
set (APPLICATION_PACKAGE_VENDOR "seekone")
set (APPLICATION_PACKAGE_NAME "opendbviewer")
set (APPLICATION_EXE_NAME opendbviewer)

project (opendbviewer CXX C)

set (CMAKE_VERBOSE_MAKEFILE OFF)

##############################
# Project configuration
##############################

# Define common project config
include (CMakeModules/ProjectCommonConfig.cmake)

##################
# Define options
##################

include (CMakeModules/ProjectCompilerChecks.cmake)

####################
# Library checking
####################

# Compiler
set(WITH_COMPILER_LIBRARIES ON CACHE BOOL "Enable build with compiler libraries")
set(WITH_PTHREAD ON CACHE BOOL "Enable build with pthread library")
# GUI
set(WITH_QT ON CACHE BOOL "Enable build with qt library")
#set(QT_USE_VERSION 5)
set(QT4_COMPONENTS QtCore QtGui QtNetwork QtSql QtTest)
set(QT5_COMPONENTS Core Gui Widgets Network Sql Test)
set(QT6_COMPONENTS Core Gui Widgets Network Sql Test)
set(QT_IMAGEFORMATS_COMPONENTS qjpeg)
set(QT_SQL_COMPONENTS qsqlite qsqlmysql qsqlpsql)
if(APPLE)
	set(QT_PLATFORMS_COMPONENTS qcocoa)
endif()
if(WIN32)
	set(QT_PLATFORMS_COMPONENTS qwindows)
endif()

# Define output variable
set (PACKAGE_DEPS)
if(WITH_AUTONOMOUS_PACKAGE_BUILD)
	set(AUTONOMOUS_PACKAGE_BUILD ON CACHE BOOL "Enable build of autonomous package without dependencies")
endif()

include (CMakeModules/ProjectIncludeLibraries.cmake)
include (CMakeModules/ProductImageUtils.cmake)

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
	src/Model/ConfigDatabase.cpp
	src/Model/ConfigDatabase.h
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
	src/GUI/QExportParametersDialog.cpp
	src/GUI/QExportParametersDialog.h
	src/GUI/QImportParametersDialog.cpp
	src/GUI/QImportParametersDialog.h
	src/GUI/QLineNumberAreaView.h
	src/GUI/QOpenDatabaseView.cpp
	src/GUI/QOpenDatabaseView.h
	src/GUI/QOpenHistoryView.cpp
	src/GUI/QOpenHistoryView.h
	src/GUI/QProgressBarDialog.cpp
	src/GUI/QProgressBarDialog.h
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
	src/Controller/ConfigDatabaseController.cpp
	src/Controller/ConfigDatabaseController.h
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
	src/Widget/QHistoryMenuWidget.cpp
	src/Widget/QHistoryMenuWidget.h
	src/Widget/QMidClickClosableTabWidget.cpp
	src/Widget/QMidClickClosableTabWidget.h
)

# Creating MOC file for sources file using "slots" or Q_OBJECT macro
set(APPLICATION_MOC_HEADERS
	src/Database/DatabaseController.h
	src/GUI/QAboutDialog.h
	src/GUI/QDatabaseConnectionView.h
	src/GUI/QDatabaseTableView.h
	src/GUI/QDatabaseWorksheetView.h
	src/GUI/QExportParametersDialog.h
	src/GUI/QImportParametersDialog.h
	src/GUI/QOpenDatabaseView.h
	src/GUI/QOpenHistoryView.h
	src/GUI/QProgressBarDialog.h
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
	src/GUIModel/QSqlDisplayTableModel.h
	src/Widget/QDropAreaWidget.h
	src/Widget/QFileExplorerWidget.h
	src/Widget/QHistoryMenuWidget.h
)
qtx_wrap_cpp(
	APPLICATION_MOC_OUTFILES
	${APPLICATION_MOC_HEADERS}
)

####################
# i18n
####################

set (I18N_FILES
	src/main.cpp
	src/Database/DatabaseController.cpp
	src/GUI/QAboutDialog.cpp
	src/GUI/QDatabaseConnectionView.cpp
	src/GUI/QDatabaseTableView.cpp
	src/GUI/QDatabaseWorksheetView.cpp
	src/GUI/QExportParametersDialog.cpp
	src/GUI/QImportParametersDialog.cpp
	src/GUI/QOpenDatabaseView.cpp
	src/GUI/QOpenHistoryView.cpp
	src/GUI/QProgressBarDialog.cpp
	src/GUI/QQueryResultView.cpp
	src/GUI/QWindowMain.cpp
	src/GUIController/QDatabaseConnectionViewController.cpp
	src/GUIController/QDatabaseTableViewController.cpp
	src/GUIController/QDatabaseWorksheetViewController.cpp
	src/GUIController/QOpenDatabaseViewController.cpp
	src/GUIController/QOpenHistoryViewController.cpp
	src/GUIController/QWindowMainController.cpp
	src/Widget/QDropAreaWidget.cpp
	src/Widget/QFileExplorerWidget.cpp
	src/Widget/QHistoryMenuWidget.cpp
)

set (TS_FILES
	ts/fr.ts
	ts/ru.ts
)

option (UPDATE_TRANSLATIONS "Update source translation ts/*.ts files (WARNING: make clean will delete the source .ts files! Danger!)" ON)
if (UPDATE_TRANSLATIONS)
	qtx_create_translation(qm_FILES ${TS_FILES} ${I18N_FILES})
else (UPDATE_TRANSLATIONS)
	qtx_add_translation(qm_FILES ${TS_FILES})
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

if (WIN32)
	set (QT_TS_FILES
		"${QT_TRANSLATIONS_DIR}/qt_fr.qm"
		"${QT_TRANSLATIONS_DIR}/qt_ru.qm"
	)
	INSTALL(FILES ${QT_TS_FILES} DESTINATION ${INSTALL_PATH_SHARE}/translations)

	if(NOT QT_USE_VERSION LESS 5)
		set (QT_TS_FILES
			"${QT_TRANSLATIONS_DIR}/qt_en.qm"
			"${QT_TRANSLATIONS_DIR}/qtbase_en.qm"
			"${QT_TRANSLATIONS_DIR}/qtbase_fr.qm"
			"${QT_TRANSLATIONS_DIR}/qtbase_ru.qm"
		)
		INSTALL(FILES ${QT_TS_FILES} DESTINATION ${INSTALL_PATH_SHARE}/translations)
	endif()
endif()

# Theme resource file
product_image_make_icon(${PROJECT_SOURCE_DIR}/data/theme/icons_src/svg ${PROJECT_BINARY_DIR}/data/theme/icons/app_default)

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

# Resources
set (QRC_TS_FILES
	${PROJECT_BINARY_DIR}/ts.qrc
)
set (QRC_FILES
	${PROJECT_BINARY_DIR}/data/app.qrc
)
qtx_add_resources(RCS_OUTFILES ${QRC_FILES})
qtx_add_resources(RCS_TS_OUTFILES ${QRC_TS_FILES})

IF(WIN32)
	set (RC_FILES
		"${PROJECT_BINARY_DIR}/data/logos/icon.rc"
		"platforms/win32/versioninfo.rc"
	)
	file(COPY ${PROJECT_SOURCE_DIR}/data/logos/@APPLICATION_PACKAGE_NAME@.ico DESTINATION ${PROJECT_BINARY_DIR}/data/logos/)
	# Fix resource compiler with MinGW
	if(MINGW)
		set(CMAKE_RC_COMPILER_INIT windres)
		ENABLE_LANGUAGE(RC)
		SET(CMAKE_RC_COMPILE_OBJECT "<CMAKE_RC_COMPILER> -O coff -i <SOURCE> -o <OBJECT>")
	endif(MINGW)
endif(WIN32)

####################
# RPATH
####################

if(UNIX AND NOT APPLE)
	# Default library path for debian
	set(CMAKE_INSTALL_RPATH "/usr/${INSTALL_PATH_LIB}")
	# Define RPATH instead of RUNPATH
	set(CMAKE_EXE_LINKER_FLAGS "-Wl,--disable-new-dtags")
endif(UNIX AND NOT APPLE)

####################
# Create Executable
####################

## Remove the output console
IF(WIN32)
	SET(GUI_TYPE WIN32)
ENDIF(WIN32)
#IF (APPLE)
	#SET(GUI_TYPE MACOSX_BUNDLE)
#ENDIF (APPLE)
# Console is useful in Debug mode
if(CMAKE_BUILD_TYPE MATCHES Debug)
	SET(GUI_TYPE "")
endif()

add_executable (${APPLICATION_EXE_NAME}
	${GUI_TYPE}
	${RC_FILES}
	${APPLICATION_SOURCES}
	${APPLICATION_MODEL_SOURCES}
	${APPLICATION_MOC_OUTFILES}
	${RCS_OUTFILES}
	${RCS_TS_OUTFILES}
	${qm_FILES}
)
target_link_libraries (${APPLICATION_EXE_NAME}
	${QT_LIBRARIES}
	${QT_QTMAIN_LIBRARY}
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

qtx_wrap_cpp(
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

# Application icons
if(UNIX AND NOT WIN32)
	file(GLOB LIST_ICONS RELATIVE ${PROJECT_SOURCE_DIR}/data/logos ${PROJECT_SOURCE_DIR}/data/logos/*${APPLICATION_PACKAGE_NAME}.png)
	foreach(ICON ${LIST_ICONS})
		set(ICON_DST "")
		if(${ICON} MATCHES "_[^_]*${APPLICATION_PACKAGE_NAME}.png")
			STRING(REGEX REPLACE "_[^_]*${APPLICATION_PACKAGE_NAME}.png" "" ICON_DST ${ICON})
			STRING(REPLACE "_" "/" ICON_DST ${ICON_DST})
			set(ICON_DST ${INSTALL_PATH_ICONS}/${ICON_DST})
		else()
			set(ICON_DST ${INSTALL_PATH_ICONS})
		endif()
		install(FILES ${PROJECT_SOURCE_DIR}/data/logos/${ICON}
			DESTINATION ${ICON_DST} RENAME ${APPLICATION_PACKAGE_NAME}.png
			PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ GROUP_EXECUTE GROUP_READ WORLD_READ WORLD_EXECUTE
		)
	endforeach(ICON)
endif(UNIX AND NOT WIN32)

#################
# Build package
#################

set(CMAKE_INSTALL_SYSTEM_RUNTIME_DESTINATION ${INSTALL_PATH_BIN})
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
endif()

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

if(CMAKE_BUILD_TYPE MATCHES Debug)
	SET(CPACK_STRIP_FILES FALSE)
	SET(CPACK_SOURCE_STRIP_FILES FALSE)
else()
	SET(CPACK_STRIP_FILES TRUE)
	SET(CPACK_SOURCE_STRIP_FILES TRUE)
endif()

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
	# DEB instruction files
	configure_file (
		"${PROJECT_SOURCE_DIR}/platforms/debian/postinst.in"
		"${PROJECT_BINARY_DIR}/platforms/debian/postinst"
	)
	configure_file (
		"${PROJECT_SOURCE_DIR}/platforms/debian/prerm.in"
		"${PROJECT_BINARY_DIR}/platforms/debian/prerm"
	)

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
	SET(CPACK_DEBIAN_PACKAGE_CONTROL_EXTRA "${PROJECT_BINARY_DIR}/platforms/debian/postinst;${PROJECT_BINARY_DIR}/platforms/debian/prerm;")
	SET(CPACK_DEBIAN_PACKAGE_SECTION "misc")
	# SET(CPACK_DEBIAN_PACKAGE_ARCHITECTURE ${DEB_ARCH})
endif()
SET(CPACK_PACKAGE_EXECUTABLES "${APPLICATION_EXE_NAME}" "${APPLICATION_NAME}")
include(CPack)
