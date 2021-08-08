cmake_minimum_required(VERSION 3.20)
project(scraper C)

set(CMAKE_C_STANDARD 99)
set(CURL_LIBRARY "-lcurl")

find_package(CURL REQUIRED)
find_package(LibXml2 REQUIRED)

add_executable(scraper main.c)

include_directories(${CURL_INCLUDE_DIR} ${LIBXML2_INCLUDE_DIR})
target_link_libraries(scraper ${CURL_LIBRARIES} ${LIBXML2_LIBRARIES})
