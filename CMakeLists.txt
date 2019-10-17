cmake_minimum_required(VERSION 3.0.0)
project(unbounded VERSION 0.1.0)

cmake_policy(SET CMP0057 NEW)

find_package(LibXml2 REQUIRED)

include_directories(include)

add_library(unbounded
  src/Xml/Dom/Document.cpp
  src/Xml/Dom/Node.cpp
)

target_include_directories(unbounded PRIVATE ${LIBXML2_INCLUDE_DIR})
target_link_libraries(unbounded PRIVATE ${LIBXML2_LIBRARIES})

enable_testing()

find_package(GTest CONFIG REQUIRED)
include(GoogleTest)

add_executable(XmlDomParserTests test/Xml/Dom/TestDocument.cpp)

gtest_add_tests(XmlDomParserTests "" AUTO)

target_link_libraries(XmlDomParserTests PRIVATE unbounded GTest::gtest GTest::gtest_main GTest::gmock GTest::gmock_main)
