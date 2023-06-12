cmake_minimum_required(VERSION 3.5)

project(GUI_application VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)

include_directories(figures forms DataModeler/dependencies/json/include DataModeler/include)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        figures/colorlabel.h
        figures/colorlabel.cpp
        figures/rectgraphicitem.h
        figures/rectgraphicitem.cpp
        figures/rectgraphicitemsettings.h
        figures/rectgraphicitemsettings.cpp
        figures/rectitemsettings.ui
        figures/selectionrect.h
        figures/selectionrect.cpp
        figures/connectorgraphicitem.h
        figures/connectorgraphicitem.cpp
        figures/connectorgraphicitemsettings.h
        figures/connectorgraphicitemsettings.cpp
        figures/connectoritemsettings.ui
        figures/cornerdots.h
        figures/cornerdots.cpp
        forms/config_types.h
        forms/config_types.cpp
        forms/config_types.ui
        forms/entitynamedialog.h
        forms/entitynamedialog.cpp
        forms/entitynamedialog.ui
        forms/addattributedialog.h
        forms/addattributedialog.cpp
        forms/addattributedialog.ui
        forms/parametersdialog.h
        forms/parametersdialog.cpp
        forms/parametersdialog.ui
        forms/redactdialog.h
        forms/redactdialog.cpp
        forms/redactdialog.ui
        mainscene.h
        mainscene.cpp
        sceneview.h
        sceneview.cpp
        icons.qrc
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(GUI_application
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET GUI_application APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(GUI_application SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(GUI_application
            ${PROJECT_SOURCES}
        )
    endif()
endif()

add_subdirectory(DataModeler)
target_link_libraries(GUI_application PRIVATE DataModeler)
target_link_libraries(GUI_application PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

set_target_properties(GUI_application PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(GUI_application)
endif()

configure_file(${CMAKE_SOURCE_DIR}/DataModeler/types.json ${CMAKE_CURRENT_BINARY_DIR})
