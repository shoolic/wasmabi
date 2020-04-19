#include "SourceController/SourceController.hpp"
#include <boost/test/unit_test.hpp>
#include <sstream>
#include <string>
BOOST_AUTO_TEST_SUITE(SourceControllerTest)

BOOST_AUTO_TEST_CASE(WindowsNewLines) {
  std::stringstream ss{"a\r\n\r\nb"};
  wasmabi::SourceController sourceController(ss);
  while (char c = sourceController.get() != 'b')
    ;
  BOOST_CHECK_EQUAL(sourceController.getCurrentLineNumber(), 3);
  BOOST_CHECK_EQUAL(sourceController.getCurrentColumnNumber(), 1);
  BOOST_CHECK_EQUAL(sourceController.getCurrentLine(), "b");

  BOOST_CHECK_EQUAL(sourceController.getCurrentCharacterPosition(), 5);
}

BOOST_AUTO_TEST_CASE(MacNewLines) {
  std::stringstream ss{"a\r\rb"};
  wasmabi::SourceController sourceController(ss);
  while (char c = sourceController.get() != 'b')
    ;
  BOOST_CHECK_EQUAL(sourceController.getCurrentLineNumber(), 3);
  BOOST_CHECK_EQUAL(sourceController.getCurrentColumnNumber(), 1);
  BOOST_CHECK_EQUAL(sourceController.getCurrentLine(), "b");

  BOOST_CHECK_EQUAL(sourceController.getCurrentCharacterPosition(), 3);
}

BOOST_AUTO_TEST_CASE(LinuxNewLines) {
  std::stringstream ss{"a\n\nb"};
  wasmabi::SourceController sourceController(ss);
  while (char c = sourceController.get() != 'b')
    ;
  BOOST_CHECK_EQUAL(sourceController.getCurrentLineNumber(), 3);
  BOOST_CHECK_EQUAL(sourceController.getCurrentColumnNumber(), 1);
  BOOST_CHECK_EQUAL(sourceController.getCurrentLine(), "b");

  BOOST_CHECK_EQUAL(sourceController.getCurrentCharacterPosition(), 3);
}

BOOST_AUTO_TEST_SUITE_END()