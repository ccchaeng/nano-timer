cmake_minimum_required(VERSION 3.5)

<<<<<<< HEAD
# 프로젝트 설정
=======
# Project name
>>>>>>> 389d2f9 (first commit)
project(qt_template VERSION 1.0 LANGUAGES CXX)

# C++ 표준 설정
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

<<<<<<< HEAD
# Qt6 라이브러리 찾기
find_package(Qt6 REQUIRED COMPONENTS Widgets PrintSupport)

# WaveForms SDK 설정 (설치 경로 확인 필요)
set(WAVEFORMS_SDK_INCLUDE "C:/Program Files (x86)/Digilent/WaveFormsSDK/inc")
set(WAVEFORMS_SDK_LIB "C:/Program Files (x86)/Digilent/WaveFormsSDK/lib/x64")

# 인클루드 디렉토리 설정
include_directories(
    ${Qt6Widgets_INCLUDE_DIRS}
    ${WAVEFORMS_SDK_INCLUDE}
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_BINARY_DIR}
)

# 링크 디렉토리 설정
link_directories(${WAVEFORMS_SDK_LIB})

=======
# Qt 설정
find_package(Qt6 COMPONENTS Widgets QUIET)
if(Qt6_FOUND)
    set(QT_VERSION_MAJOR 6)
    message(STATUS "Found Qt6: ${Qt6_VERSION}")
else()
    find_package(Qt5 COMPONENTS Widgets REQUIRED)
    set(QT_VERSION_MAJOR 5)
    message(STATUS "Found Qt5: ${Qt5_VERSION}")
endif()

# WaveForms SDK 설정
set(WAVEFORMS_SDK_INCLUDE "C:/Program Files (x86)/Digilent/WaveFormsSDK/inc")
set(WAVEFORMS_SDK_LIB "C:/Program Files (x86)/Digilent/WaveFormsSDK/lib/x64")
include_directories(${WAVEFORMS_SDK_INCLUDE})
link_directories(${WAVEFORMS_SDK_LIB})

# Eigen 경로 수동 지정
include_directories("C:/eigen")
include_directories("C:/eigen/Eigen")

# 프로젝트 디렉토리
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include ${CMAKE_BINARY_DIR})

>>>>>>> 389d2f9 (first commit)
# 소스 파일
set(SOURCES
    src/main.cpp
    src/mainwindow.cpp
<<<<<<< HEAD
    src/qcustomplot.cpp
=======
    src/dwfNano.cpp
    src/dwfThread.cpp
    src/fitSine.cpp
>>>>>>> 389d2f9 (first commit)
)

# 헤더 파일
set(HEADERS
    include/mainwindow.h
    include/dwfconstants.h
<<<<<<< HEAD
    include/qcustomplot.h
=======
    include/dwfNano.h
    include/dwfThread.h
    include/fitSine.h
>>>>>>> 389d2f9 (first commit)
)

# UI 파일
set(UI_FILES
    ui/mainwindow.ui
)

# 리소스 파일
set(RESOURCE_FILES
    resources/resources.qrc
)

<<<<<<< HEAD
# Qt6에서 MOC, UI, 리소스 처리
qt6_wrap_cpp(MOC_SOURCES ${HEADERS})
qt6_wrap_ui(UIC_SOURCES ${UI_FILES})
qt_add_resources(RESOURCE_SOURCES ${RESOURCE_FILES})

# 실행 파일 생성
add_executable(${PROJECT_NAME}
    ${SOURCES}
    ${MOC_SOURCES}
    ${UIC_SOURCES}
    ${RESOURCE_SOURCES}
)

# Qt6 및 WaveForms SDK 라이브러리 연결
target_link_libraries(${PROJECT_NAME}
    Qt6::Widgets
    Qt6::PrintSupport
    "${WAVEFORMS_SDK_LIB}/dwf.lib"
)
=======
# MOC 파일 생성
if(QT_VERSION_MAJOR EQUAL 6)
    qt6_wrap_cpp(MOC_SOURCES ${HEADERS})
    qt6_wrap_ui(UIC_SOURCES ${UI_FILES})
    qt_add_resources(RESOURCE_SOURCES ${RESOURCE_FILES})
else()
    qt5_wrap_cpp(MOC_SOURCES ${HEADERS})
    qt5_wrap_ui(UIC_SOURCES ${UI_FILES})
    qt5_add_resources(RESOURCE_SOURCES ${RESOURCE_FILES})
endif()

# 실행 파일 생성
add_executable(${PROJECT_NAME} ${SOURCES} ${MOC_SOURCES} ${UIC_SOURCES} ${RESOURCE_SOURCES})

# 라이브러리 연결
if(QT_VERSION_MAJOR EQUAL 6)
    target_link_libraries(${PROJECT_NAME} Qt6::Widgets "${WAVEFORMS_SDK_LIB}/dwf.lib")
else()
    target_link_libraries(${PROJECT_NAME} Qt5::Widgets "${WAVEFORMS_SDK_LIB}/dwf.lib")
endif()
>>>>>>> 389d2f9 (first commit)

# Windows 플랫폼 메시지 출력
if(WIN32)
    message(STATUS "Windows 64bit 플랫폼 설정 완료")
endif()
