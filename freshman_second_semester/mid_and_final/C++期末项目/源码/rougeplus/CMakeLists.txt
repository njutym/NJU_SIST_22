cmake_minimum_required(VERSION 3.5)

project(rougeplus VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
find_package(Qt6 REQUIRED COMPONENTS Multimedia)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        res.qrc
        mypushbutton.h
        mypushbutton.cpp
        mystore.h
        mystore.cpp
        mycheet.h
        mycheet.cpp
        myconfig.h
        bullet.h
        bullet.cpp
        chooseplayer.h
        chooseplayer.cpp
        scene.h
        scene.cpp
        playscene.h
        playscene.cpp
        myobject.h
        myobject.cpp
        player.h
        player.cpp
        enemy.h
        enemy.cpp
        eliteenemy.h
        eliteenemy.cpp
        mygood.h
        mygood.cpp
        myobstacle.h
        myobstacle.cpp
        boss.h
        boss.cpp
        bombeffect.h
        bombeffect.cpp
        helpscene.h
        helpscene.cpp
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(rougeplus
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET rougeplus APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(rougeplus SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(rougeplus
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(rougeplus PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(rougeplus PRIVATE Qt6::Multimedia)

set_target_properties(rougeplus PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

install(TARGETS rougeplus
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(rougeplus)
endif()
