cmake_minimum_required(VERSION 3.22)
project(toasts LANGUAGES C CXX)


set(SOURCE_DIR "${PROJECT_SOURCE_DIR}/src")
set(INCLUDE_DIR "${PROJECT_SOURCE_DIR}/include")

file(GLOB_RECURSE PROJECT_SOURCES CONFIGURE_DEPENDS "${SOURCE_DIR}/*.cpp")
file(GLOB_RECURSE SQLITE_SOURCES CONFIGURE_DEPENDS "${SOURCE_DIR}/sqlite/*.c")

add_executable(${PROJECT_NAME} "${SOURCE_DIR}/main.cpp")

target_sources(
    ${PROJECT_NAME}
    PRIVATE ${PROJECT_SOURCES}
    PRIVATE ${SQLITE_SOURCES}
    )

target_include_directories(${PROJECT_NAME} PRIVATE ${INCLUDE_DIR})

install(TARGETS ${PROJECT_NAME} DESTINATION /bin/)
