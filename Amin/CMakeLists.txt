cmake_minimum_required(VERSION 3.16)

project(adam VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets Charts)

qt_standard_project_setup()

qt_add_executable(adam
    main.cpp
    PollutantOverviewWindow.cpp
    PollutantChartWidget.cpp
    POPwindow.cpp
    rawwindow.cpp
    WQDataset.cpp
    WQSample.cpp
    WQTableModel.cpp
)

target_link_libraries(adam PRIVATE Qt6::Widgets Qt6::Charts)

set_target_properties(adam PROPERTIES
    WIN32_EXECUTABLE ON
    MACOSX_BUNDLE OFF
) 