cmake_minimum_required(VERSION 3.5)

# 프로젝트 설정
project(qt_template VERSION 1.0 LANGUAGES CXX)

# C++ 표준 설정
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

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

# 소스 파일
set(SOURCES
    src/main.cpp
    src/mainwindow.cpp
    src/qcustomplot.cpp
)

# 헤더 파일
set(HEADERS
    include/mainwindow.h
    include/qcustomplot.h
)

# UI 파일
set(UI_FILES
    ui/mainwindow.ui
)

# 리소스 파일
set(RESOURCE_FILES
    resources/resources.qrc
)

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

# Windows 플랫폼 메시지 출력
if(WIN32)
    message(STATUS "Windows 64bit 플랫폼 설정 완료")
endif()
