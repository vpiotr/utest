#ifndef __UTEST_H__
#define __UTEST_H__

//----------------------------------------------------------------------------------
// Name:        utest.h
// Purpose:     Unit testing library
// Author:      Piotr Likus
// Created:     31/08/2020
//----------------------------------------------------------------------------------

#include <cstdlib>
#include <stdexcept>
#include <string>
#include <iostream>
#include <sstream>

namespace utest {

// Forward declarations for string handling
namespace details {
    template<typename T>
    inline std::string convertToString(const T& value) {
        return std::to_string(value);
    }
    
    // Specialization for std::string
    inline std::string convertToString(const std::string& value) {
        return value;
    }
    
    // Specialization for const char*
    inline std::string convertToString(const char* value) {
        return std::string(value);
    }
}

#define UTEST_TO_STRING( x ) utest::details::convertToString( ( x ) )

#define UTEST_ASSERT_TRUE( condition )                             \
{                                                                   \
  if( !( condition ) )                                              \
  {                                                                 \
    std::ostringstream ss;                                          \
    ss << "condition is false: '" << (#condition) << "' at "        \
       << __FILE__ << ":" << __LINE__                               \
       << " in " << __PRETTY_FUNCTION__;                            \
    throw std::runtime_error(ss.str());                             \
  }                                                                 \
}

#define UTEST_ASSERT_TRUE_MSG( condition, msg )                    \
{                                                                   \
  if( !( condition ) )                                              \
  {                                                                 \
    std::ostringstream ss;                                          \
    ss << "assertion failed, '" << msg << "' at "                   \
       << __FILE__ << ":" << __LINE__                               \
       << " in " << __PRETTY_FUNCTION__;                            \
    throw std::runtime_error(ss.str());                             \
  }                                                                 \
}

#define UTEST_ASSERT_FALSE( condition )                            \
{                                                                   \
  if( ( condition ) )                                               \
  {                                                                 \
    std::ostringstream ss;                                          \
    ss << "condition is true: '" << (#condition) << "' at "         \
       << __FILE__ << ":" << __LINE__                               \
       << " in " << __PRETTY_FUNCTION__;                            \
    throw std::runtime_error(ss.str());                             \
  }                                                                 \
}

#define UTEST_ASSERT_FALSE_MSG( condition, msg )                   \
{                                                                   \
  if( ( condition ) )                                               \
  {                                                                 \
    std::ostringstream ss;                                          \
    ss << "assertion failed, '" << msg << "' at "                   \
       << __FILE__ << ":" << __LINE__                               \
       << " in " << __PRETTY_FUNCTION__;                            \
    throw std::runtime_error(ss.str());                             \
  }                                                                 \
}

#define UTEST_ASSERT_EQUALS( x, y )                                \
{                                                                   \
  if( ( x ) != ( y ) )                                              \
  {                                                                 \
    std::ostringstream ss;                                          \
    ss << "Assertion failed, at "                                   \
       << __FILE__ << ":" << __LINE__                               \
       << " in " << __PRETTY_FUNCTION__                             \
       << ": " << UTEST_TO_STRING( ( x ) )                          \
       << " != " << UTEST_TO_STRING( ( y ) );                       \
    throw std::runtime_error(ss.str());                             \
  }                                                                 \
}

#define UTEST_ASSERT_EQUALS_MSG( x, y, msg )                       \
{                                                                   \
  if( ( x ) != ( y ) )                                              \
  {                                                                 \
    std::ostringstream ss;                                          \
    ss << "Assertion failed, '" << msg << "' at "                   \
       << __FILE__ << ":" << __LINE__                               \
       << " in " << __PRETTY_FUNCTION__                             \
       << ": " << UTEST_TO_STRING( ( x ) )                          \
       << " != " << UTEST_TO_STRING( ( y ) );                       \
    throw std::runtime_error(ss.str());                             \
  }                                                                 \
}

#define UTEST_ASSERT_NULL( ptr )                                   \
{                                                                   \
  if( ( ptr ) != nullptr )                                          \
  {                                                                 \
    std::ostringstream ss;                                          \
    ss << "Assertion failed, pointer is not null at "               \
       << __FILE__ << ":" << __LINE__                               \
       << " in " << __PRETTY_FUNCTION__                             \
       << ": " << (#ptr);                                           \
    throw std::runtime_error(ss.str());                             \
  }                                                                 \
}

#define UTEST_ASSERT_NOT_NULL( ptr )                               \
{                                                                   \
  if( ( ptr ) == nullptr )                                          \
  {                                                                 \
    std::ostringstream ss;                                          \
    ss << "Assertion failed, pointer is null: '" << (#ptr) << "' at " \
       << __FILE__ << ":" << __LINE__                               \
       << " in " << __PRETTY_FUNCTION__;                            \
    throw std::runtime_error(ss.str());                             \
  }                                                                 \
}

namespace details {

    template<typename Func>
    void testFunc(const char *name, Func f, bool &failed) {
        try {
            f();
            std::cout << "OK: Test [" << std::string(name) << "] " << "succeeded\n";
        }
        catch (std::exception &e) {
            std::cout << "Error: Test [" << std::string(name) << "] failed!, error: " << e.what() << "\n";
            failed = true;
        }
    }

    template<typename Func>
    inline void AssertThrows(Func assertion, const std::string &msg = "") {
        bool throwFound = false;
        try {
            assertion();
        }
        catch (...) {
            throwFound = true;
        }

        if (!throwFound) {
            if (msg.size() > 0) {
                std::ostringstream ss;
                ss << "assertion.throws = [" << msg << "]";
                throw std::runtime_error(ss.str());
            } else {
                throw std::runtime_error("assertion.throws failed");
            }
        }
    }
} // namespace

#define UTEST_ASSERT_THROWS(F) utest::details::AssertThrows(F)

#define UTEST_ASSERT_THROWS_MSG(F, MSG) utest::details::AssertThrows(F, std::string(MSG))

#define UTEST_FUNC(a) utest::details::testFunc(#a, test_##a, errorFound)

#define UTEST_PROLOG() bool errorFound = false

#define UTEST_EPILOG() do { if (errorFound) { \
std::cout << "Failures!\n"; \
return EXIT_FAILURE; \
} \
else { \
	std::cout << "Success!\n"; \
	return EXIT_SUCCESS; \
} } while(0)

} // namespace

#endif

