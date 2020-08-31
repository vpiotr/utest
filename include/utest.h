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

namespace utest {

#define UTEST_TO_STRING( x ) std::to_string( ( x ) )

#define UTEST_ASSERT_TRUE( condition )                             \
{                                                                   \
  if( !( condition ) )                                              \
  {                                                                 \
    throw std::runtime_error( std::string("condition is false: '")   \
                              + std::string((#condition))           \
                              + std::string("' at ")                 \
                              + std::string( __FILE__ )             \
                              + std::string( ":" )                  \
                              + std::to_string( __LINE__ )          \
                              + std::string( " in " )               \
                              + std::string( __PRETTY_FUNCTION__ )  \
    );                                                              \
  }                                                                 \
}

#define UTEST_ASSERT_TRUE_MSG( msg, condition )                             \
{                                                                   \
  if( !( condition ) )                                              \
  {                                                                 \
    throw std::runtime_error( std::string("assertion failed, '")     \
                              + std::string(msg)                    \
                              + std::string("' at ")                 \
                              + std::string( __FILE__ )             \
                              + std::string( ":" )                  \
                              + std::to_string( __LINE__ )          \
                              + std::string( " in " )               \
                              + std::string( __PRETTY_FUNCTION__ )  \
    );                                                              \
  }                                                                 \
}

#define UTEST_ASSERT_FALSE( condition )    \
{                                                                   \
  if( ( condition ) )                                              \
  {                                                                 \
    throw std::runtime_error( std::string("condition is true: '")   \
                              + std::string((#condition))           \
                              + std::string("' at ")                 \
                              + std::string( __FILE__ )             \
                              + std::string( ":" )                  \
                              + std::to_string( __LINE__ )          \
                              + std::string( " in " )               \
                              + std::string( __PRETTY_FUNCTION__ )  \
    );                                                              \
  }                                                                 \
}

#define UTEST_ASSERT_FALSE_MSG( msg, condition )         \
{                                                                   \
  if( ( condition ) )                                               \
  {                                                                 \
    throw std::runtime_error( std::string("assertion failed, '")    \
                              + std::string(msg)                    \
                              + std::string("' at ")                \
                              + std::string( __FILE__ )             \
                              + std::string( ":" )                  \
                              + std::to_string( __LINE__ )          \
                              + std::string( " in " )               \
                              + std::string( __PRETTY_FUNCTION__ )  \
    );                                                              \
  }                                                                 \
}

#define UTEST_ASSERT_EQUALS( x, y )                                  \
{                                                                   \
  if( ( x ) != ( y ) )                                              \
  {                                                                 \
    throw std::runtime_error( std::string("Assertion failed, ")     \
                              + std::string(" at ")                 \
                              + std::string( __FILE__ )             \
                              + std::string( ":" )                  \
                              + std::to_string( __LINE__ )          \
                              + std::string( " in " )               \
                              + std::string( __PRETTY_FUNCTION__ )  \
                              + std::string( ": " )                 \
                              + UTEST_TO_STRING( ( x ) )            \
                              + std::string( " != " )               \
                              + UTEST_TO_STRING( ( y ) )            \
    );                                                              \
  }                                                                 \
}

#define UTEST_ASSERT_EQUALS_MSG( msg, x, y )                             \
{                                                                   \
  if( ( x ) != ( y ) )                                              \
  {                                                                 \
    throw std::runtime_error( std::string("Assertion failed, '")     \
                              + std::string(msg)                    \
                              + std::string("' at ")                 \
                              + std::string( __FILE__ )             \
                              + std::string( ":" )                  \
                              + std::to_string( __LINE__ )          \
                              + std::string( " in " )               \
                              + std::string( __PRETTY_FUNCTION__ )  \
                              + std::string( ": " )                 \
                              + UTEST_TO_STRING( ( x ) )            \
                              + std::string( " != " )               \
                              + UTEST_TO_STRING( ( y ) )            \
    );                                                              \
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
            if (msg.size() > 0)
                throw std::runtime_error(std::string("assertion.throws = [") + msg + "]");
            else
                throw std::runtime_error("assertion.throws failed");
        }
    }
} // namespace

#define UTEST_ASSERT_THROWS(F) utest::details::AssertThrows(F)

#define UTEST_ASSERT_THROWS_MSG(MSG, F) utest::details::AssertThrows(F, std::string(MSG))

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

