cmake_minimum_required(VERSION 3.16)

project(QGame_PlayHook VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Multimedia)

set(PROJECT_SOURCES
        main.cpp
        widget.cpp
        widget.h
        widget.ui
)

set(PROJECT_RESOURCES resources.qrc)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(QGame_PlayHook
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
	${PROJECT_RESOURCES}
        Player.h
        Player.cpp


        GameManager.h

        GameManager.h
        GameManager.h

        platform.h platform.cpp

        tool.h tool.cpp
        rope.h rope.cpp
        GameManager.h
        GameManager.cpp
        mainmenu.h mainmenu.cpp
        mainmenu.h mainmenu.cpp mainmenu.ui
        gameinstruction.h gameinstruction.cpp gameinstruction.ui
        wintime.h wintime.cpp wintime.ui
        wintime_dl.h wintime_dl.cpp wintime_dl.ui




    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET QGame_PlayHook APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(QGame_PlayHook SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(QGame_PlayHook
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(QGame_PlayHook PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Multimedia)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.QGame_Apex)
endif()
set_target_properties(QGame_PlayHook PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS QGame_PlayHook
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(QGame_PlayHook)
endif()
