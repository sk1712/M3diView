cmake_minimum_required(VERSION 2.8.9)

if(APPLE)
    # Check if environment variable QTDIR is set.
    # Extra security for windows environment as well.
    if (DEFINED ENV{QTDIR})
        set(CMAKE_PREFIX_PATH $ENV{QTDIR} ${CMAKE_PREFIX_PATH})
    else ()
        message (FATAL_ERROR "ERROR: Environment variable QTDIR is not set. Please locate your Qt folder MY_QT5_DIR.")
    endif ()
endif()

project(m3diview)

# Find includes in corresponding build directories
SET (CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
SET (CMAKE_AUTOMOC ON)

# Find the QtWidgets library
find_package(Qt5Concurrent REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5OpenGL REQUIRED)
find_package(Qt5Core REQUIRED)

set(VIEWER_HEADERS
../include/QtGlWidget.h
../include/QtTwoDimensionalGlWidget.h
../include/QtThreeDimensionalGlWidget.h
../include/QtViewerWidget.h
../include/Qt2dViewerWidget.h
../include/Qt3dViewerWidget.h
../include/QtMainWindow.h
../include/QtToolWidget.h
../include/QtInfoWidget.h
../../include/irtkQtTreeItem.h
../../include/irtkQtTreeModel.h
../../include/irtkQtImageObject.h
../../include/irtkQtConfiguration.h
../../include/irtkQtBaseViewer.h
../../include/irtkQtTwoDimensionalViewer.h
../../include/irtkQtThreeDimensionalViewer.h
../../include/irtkQtLookupTable.h)

set(VIEWER_SOURCES
main.cpp
QtGlWidget.cpp 
QtTwoDimensionalGlWidget.cpp
QtThreeDimensionalGlWidget.cpp
QtViewerWidget.cpp
Qt2dViewerWidget.cpp
Qt3dViewerWidget.cpp
QtMainWindow.cpp
QtToolWidget.cpp
QtInfoWidget.cpp
../../src/irtkQtTreeItem.cpp
../../src/irtkQtTreeModel.cpp
../../src/irtkQtImageObject.cpp
../../src/irtkQtConfiguration.cpp
../../src/irtkQtBaseViewer.cpp
../../src/irtkQtTwoDimensionalViewer.cpp
../../src/irtkQtThreeDimensionalViewer.cpp
../../src/irtkQtLookupTable.cpp)

set(VIEWER_RESOURCES ../../resources.qrc)

qt5_add_resources(ResourceSources ${VIEWER_RESOURCES})

add_executable(m3diview MACOSX_BUNDLE ${VIEWER_SOURCES} ${VIEWER_HEADERS} ${ResourceSources})

# Use the Widgets, Concurrent and OpenGL modules from Qt 5.
qt5_use_modules(m3diview Widgets Concurrent OpenGL Core)
