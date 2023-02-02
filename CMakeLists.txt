cmake_minimum_required(VERSION 3.16)

project(hcplayer-demo VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON) # Meta-Object Compiler
#set(CMAKE_AUTORCC ON) # Resource Compiler
#set(CMAKE_AUTOUIC ON) # User Interface Compiler
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Quick QuickControls2 Gui Multimedia Widgets)

file(GLOB_RECURSE CPP_PATHS  cpp/*.cpp cpp/*.h cpp/*.cc cpp/*.hpp)
foreach(filepath ${CPP_PATHS})
    message("加载cpp文件：" ${filepath})
    string(REPLACE "${CMAKE_CURRENT_SOURCE_DIR}/" "" filename ${filepath}) #.+/(.+)\\\\..*" "\\\\1
    list(APPEND CPP_FILES ${filename})
endforeach(filepath)
list(REMOVE_DUPLICATES CPP_FILES)

file(GLOB_RECURSE QML_PATHS qml/*.qml qml/*.js qml/*.ttf)
foreach(filepath ${QML_PATHS})
    string(REPLACE "${CMAKE_CURRENT_SOURCE_DIR}/" "" filename ${filepath}) #.+/(.+)\\\\..*" "\\\\1
    message("加载qml文件：" ${filename})
    list(APPEND QML_FILES ${filename})
endforeach(filepath)
list(REMOVE_DUPLICATES QML_FILES)


qt_add_executable(app-${CMAKE_PROJECT_NAME}
    ${CPP_FILES}
)

qt_add_resources(app-${CMAKE_PROJECT_NAME} "qml"
   PREFIX
        "/"
   FILES
        ${QML_FILES}
)

# lib
if(WIN32)
    # 海康lib
    add_library (hclib1 SHARED IMPORTED)
    add_library (hclib2 SHARED IMPORTED)
    add_library (hclib3 SHARED IMPORTED)
    add_library (hclib4 SHARED IMPORTED)
    add_library (hclib5 SHARED IMPORTED)
    add_library (hclib6 SHARED IMPORTED)
    add_library (hclib7 SHARED IMPORTED)
    set_property (TARGET hclib1 PROPERTY IMPORTED_IMPLIB ${CMAKE_CURRENT_SOURCE_DIR}/lib/libhcnet_win/HCNetSDK.lib)
    set_property (TARGET hclib2 PROPERTY IMPORTED_IMPLIB ${CMAKE_CURRENT_SOURCE_DIR}/lib/libhcnet_win/GdiPlus.lib)
    set_property (TARGET hclib3 PROPERTY IMPORTED_IMPLIB ${CMAKE_CURRENT_SOURCE_DIR}/lib/libhcnet_win/HCCore.lib)
    set_property (TARGET hclib4 PROPERTY IMPORTED_IMPLIB ${CMAKE_CURRENT_SOURCE_DIR}/lib/libhcnet_win/PlayCtrl.lib)
    set_property (TARGET hclib5 PROPERTY IMPORTED_IMPLIB ${CMAKE_CURRENT_SOURCE_DIR}/lib/libhcnet_win/HCNetSDKCom/HCAlarm.lib)
    set_property (TARGET hclib6 PROPERTY IMPORTED_IMPLIB ${CMAKE_CURRENT_SOURCE_DIR}/lib/libhcnet_win/HCNetSDKCom/HCGeneralCfgMgr.lib)
    set_property (TARGET hclib7 PROPERTY IMPORTED_IMPLIB ${CMAKE_CURRENT_SOURCE_DIR}/lib/libhcnet_win/HCNetSDKCom/HCPreview.lib)

    target_link_libraries(app-${CMAKE_PROJECT_NAME} PRIVATE hclib1 hclib2 hclib3 hclib4 hclib5 hclib6 hclib7)

elseif(APPLE)
    message(STATUS "Now is APPLE OS's.")
elseif(UNIX)
    message(STATUS "Now is UNIX-like OS's.")
endif()

target_link_libraries(app-${CMAKE_PROJECT_NAME}
    PRIVATE Qt6::Quick Qt6::QuickControls2 Qt6::Gui Qt6::Multimedia Qt6::Widgets)

set_target_properties(app-${CMAKE_PROJECT_NAME} PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    # FALSE 表示出现控制台调试框
    WIN32_EXECUTABLE FALSE
)

install(TARGETS app-${CMAKE_PROJECT_NAME}
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
