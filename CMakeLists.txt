cmake_minimum_required(VERSION 3.15)
project(stripe_pk_extractor)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCES
    src/main.cpp
    src/Stripe.cpp
    src/Algorithm.cpp
)

include_directories(
    ${PROJECT_SOURCE_DIR}/include
)

find_package(CURL REQUIRED)
find_package(Threads REQUIRED)

add_executable(GetPK ${SOURCES})

target_include_directories(GetPK PRIVATE
    ${CURL_INCLUDE_DIRS}
)

target_link_libraries(GetPK PRIVATE
    ${CURL_LIBRARIES}
    Threads::Threads
)

if(MSVC)
    target_compile_options(GetPK PRIVATE /W4 /permissive-)
else()
    target_compile_options(GetPK PRIVATE -Wall -Wextra -pedantic)
endif()
