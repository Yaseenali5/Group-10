
cmake_minimum_required(VERSION 3.16)

project(final VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets Charts)

qt_standard_project_setup()

qt_add_executable(final
    main.cpp
    POPwindow.cpp
    rawwindow.cpp
    WQDataset.cpp
    WQSample.cpp
    WQTableModel.cpp
)

target_link_libraries(final PRIVATE Qt6::Widgets Qt6::Charts)

set_target_properties(final PROPERTIES
    WIN32_EXECUTABLE ON
    MACOSX_BUNDLE OFF
) 