cmake_minimum_required(VERSION 2.8.9)

IF(APPLE)
  set(CMAKE_PREFIX_PATH "/Users/ira/Qt5.1.1/5.1.1/clang_64/lib/cmake")
ENDIF()

project(newviewer)

# Find includes in corresponding build directories
  SET (CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
  SET (CMAKE_AUTOMOC ON)

# Find the QtWidgets library
find_package(Qt5Concurrent REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5OpenGL REQUIRED)

set(VIEWER_HEADERS
../include/QtGlWidget.h
../include/QtTwoDimensionalGlWidget.h
../include/QtViewerWidget.h
../include/QtMainWindow.h
../include/irtkImageListModel.h
../include/irtkQtImageObject.h
../include/irtkQtViewer.h
../include/irtkQtTwoDimensionalViewer.h
../include/irtkQtLookupTable.h)

SET(VIEWER_SOURCES
main.cpp
QtGlWidget.cpp 
QtTwoDimensionalGlWidget.cpp
QtViewerWidget.cpp
QtMainWindow.cpp
irtkImageListModel.cpp
irtkQtImageObject.cpp
irtkQtViewer.cpp
irtkQtTwoDimensionalViewer.cpp
irtkQtLookupTable.cpp)

SET(VIEWER_RESOURCES ../resources.qrc)

qt5_add_resources(ResourceSources ${VIEWER_RESOURCES})

add_executable(newviewer MACOSX_BUNDLE ${VIEWER_SOURCES} ${VIEWER_HEADERS} ${ResourceSources})

# Use the Widgets module from Qt 5.
qt5_use_modules(newviewer Widgets Concurrent OpenGL)
