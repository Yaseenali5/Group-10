
cmake_minimum_required(VERSION 3.16)

project(wqm2 VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets Charts)

qt_standard_project_setup()

qt_add_resources(WQMonitor2_RESOURCES resources.qrc)

qt_add_executable(wqm2
    main.cpp
    POPwindow.cpp
    FCwindow.cpp
    rawwindow.cpp
    WQDataset.cpp
    WQSample.cpp
    WQTableModel.cpp
    filterProxy.cpp
    PollutantChartWidget.cpp
    PollutantOverviewWindow.cpp
    homePage.cpp
    LWindow.cpp
    ComplianceWindow.cpp
)

target_link_libraries(wqm2 PRIVATE Qt6::Widgets Qt6::Charts)

set_target_properties(wqm2 PROPERTIES
    WIN32_EXECUTABLE ON
    MACOSX_BUNDLE OFF
) 