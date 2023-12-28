cmake_minimum_required(VERSION 3.5)

project(LibraryManagementSystem VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(PROJECT_SOURCES

        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(LibraryManagementSystem
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET LibraryManagementSystem APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(LibraryManagementSystem SHARED
            ${PROJECT_SOURCES}

        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(LibraryManagementSystem
            ${PROJECT_SOURCES}
            source/light/lightstyle.qrc
            source/dark/darkstyle.qrc
            formhome.h formhome.cpp formhome.ui
            formabout.h formabout.cpp formabout.ui

            main.cpp
            LibraryManagementSystem.h LibraryManagementSystem.cpp
            images.qrc
            formsetting.h formsetting.cpp formsetting.ui
            formuser.h formuser.cpp formuser.ui
            source/switchbutton/switchbutton.cpp source/switchbutton/switchbutton.h
            dialogadd.h dialogadd.cpp dialogadd.ui
            basicfunction.h basicfunction.cpp
            dialogchange.h dialogchange.cpp dialogchange.ui
            tableview.h tableview.cpp
            dialoglogin.h dialoglogin.cpp dialoglogin.ui
            dialogregister.h dialogregister.cpp dialogregister.ui
            UsersFunction.cpp


            # framelesswindow.h framelesswindow.cpp

            # LibrarySystemInfo.json









        )
    endif()
endif()

target_link_libraries(LibraryManagementSystem PRIVATE Qt${QT_VERSION_MAJOR}::Widgets
                                                PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/cJSON.c)
# Qt for iOS sets MACOSX_BUNDLE_GUI_IDE  NTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.LibraryManagementSystem)
endif()
set_target_properties(LibraryManagementSystem PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
include_directories(
    "./source/switchbutton"
    "./"
)
install(TARGETS LibraryManagementSystem
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)


if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(LibraryManagementSystem)
endif()
