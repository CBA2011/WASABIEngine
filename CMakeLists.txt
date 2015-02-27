cmake_minimum_required(VERSION 2.8.11)
project(WASABIEngine)

# Add Qt5 support for this project
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
find_package(Qt5Core REQUIRED)

# Configure the target
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
add_library(WASABIEngine SHARED
    src/AffectiveState.cc
    src/cogaAttendee.cc
    src/cogaEmotionalAttendee.cc
    src/EmotionContainer.cc
    src/EmotionConverter.cc
    src/EmotionDynamics.cc
    src/PrimaryEmotion.cc
    src/SecondaryEmotion.cc
    src/WASABIEngine.cc
    src/WASABIRunner.cc
)

# Link Qt to the library
target_link_libraries(WASABIEngine Qt5::Core)
