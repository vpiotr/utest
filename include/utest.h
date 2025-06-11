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
#include <vector>
#include <type_traits>
#include <typeinfo>

namespace utest {

#define UTEST_FUNC(a) utest::details::testFunc(#a, test_##a, errorFound)

#define UTEST_PROLOG() bool errorFound = false; \
    utest::details::getTestResults().clear()

#define UTEST_ALLOW_EMPTY_TESTS() utest::details::getAllowEmptyTests() = true

class AssertionException : public std::runtime_error {
public:
    explicit AssertionException(const std::string& message) 
        : std::runtime_error(message), 
          file_("unknown"), 
          line_(0), 
          function_("unknown") {}
    
    AssertionException(const std::string& message, const std::string& file, 
                      int line, const std::string& function)
        : std::runtime_error(message), 
          file_(file), 
          line_(line), 
          function_(function) {}
    
    virtual ~AssertionException() = default;
    
    const std::string& getFile() const { return file_; }
    int getLine() const { return line_; }
    const std::string& getFunction() const { return function_; }
    
    // Format a complete error message
    std::string getFormattedMessage() const {
        std::ostringstream ss;
        ss << what() << " at " << file_ << ":" << line_ << " in " << function_;
        return ss.str();
    }
    
protected:
    std::string file_;
    int line_;
    std::string function_;
};

// Forward declarations for string handling
namespace details {
    // SFINAE helper to detect if type is streamable
    template<typename T>
    class is_streamable {
        template<typename TT>
        static auto test(int) -> decltype(std::declval<std::ostringstream&>() << std::declval<TT>(), std::true_type{});
        template<typename>
        static std::false_type test(...);
    public:
        static const bool value = decltype(test<T>(0))::value;
    };

    // Helper to detect arithmetic types (but not bool or char types that might be better as strings)
    template<typename T>
    struct is_numeric : std::integral_constant<bool, 
        std::is_arithmetic<T>::value && 
        !std::is_same<T, bool>::value && 
        !std::is_same<T, char>::value &&
        !std::is_same<T, wchar_t>::value &&
        !std::is_same<T, char16_t>::value &&
        !std::is_same<T, char32_t>::value> {};

    // For numeric types, use std::to_string
    template<typename T>
    inline typename std::enable_if<is_numeric<T>::value, std::string>::type
    convertToString(const T& value) {
        return std::to_string(value);
    }
    
    // For streamable non-numeric types, use ostringstream
    template<typename T>
    inline typename std::enable_if<!is_numeric<T>::value && is_streamable<T>::value, std::string>::type
    convertToString(const T& value) {
        std::ostringstream ss;
        ss << value;
        return ss.str();
    }
    
    // For non-streamable types, return type information
    template<typename T>
    inline typename std::enable_if<!is_numeric<T>::value && !is_streamable<T>::value, std::string>::type
    convertToString(const T& value) {
        std::ostringstream ss;
        ss << "[" << typeid(T).name() << " at " << &value << "]";
        return ss.str();
    }
    
    // Specialization for std::string
    inline std::string convertToString(const std::string& value) {
        return value;
    }
    
    // Specialization for const char*
    inline std::string convertToString(const char* value) {
        return std::string(value);
    }
    
    // Specialization for bool
    inline std::string convertToString(bool value) {
        return value ? "true" : "false";
    }
    
    // Specialization for char
    inline std::string convertToString(char value) {
        return std::string(1, value);
    }
}

#define UTEST_TO_STRING( x ) utest::details::convertToString( ( x ) )

#define UTEST_ASSERT_TRUE( condition )                             \
{                                                                   \
  if( !( condition ) )                                              \
  {                                                                 \
    std::ostringstream ss;                                          \
    ss << "condition is false: '" << (#condition) << "'";           \
    throw utest::AssertionException(ss.str(), __FILE__, __LINE__, __PRETTY_FUNCTION__); \
  }                                                                 \
}

#define UTEST_ASSERT_TRUE_MSG( condition, msg )                    \
{                                                                   \
  if( !( condition ) )                                              \
  {                                                                 \
    std::ostringstream ss;                                          \
    ss << "assertion failed, '" << msg << "'";                      \
    throw utest::AssertionException(ss.str(), __FILE__, __LINE__, __PRETTY_FUNCTION__); \
  }                                                                 \
}

#define UTEST_ASSERT_FALSE( condition )                            \
{                                                                   \
  if( ( condition ) )                                               \
  {                                                                 \
    std::ostringstream ss;                                          \
    ss << "condition is true: '" << (#condition) << "'";            \
    throw utest::AssertionException(ss.str(), __FILE__, __LINE__, __PRETTY_FUNCTION__); \
  }                                                                 \
}

#define UTEST_ASSERT_FALSE_MSG( condition, msg )                   \
{                                                                   \
  if( ( condition ) )                                               \
  {                                                                 \
    std::ostringstream ss;                                          \
    ss << "assertion failed, '" << msg << "'";                      \
    throw utest::AssertionException(ss.str(), __FILE__, __LINE__, __PRETTY_FUNCTION__); \
  }                                                                 \
}

#define UTEST_ASSERT_EQUALS( x, y )                                \
{                                                                   \
  if( ( x ) != ( y ) )                                              \
  {                                                                 \
    std::ostringstream ss;                                          \
    ss << "Assertion failed: " << UTEST_TO_STRING( ( x ) )          \
       << " != " << UTEST_TO_STRING( ( y ) );                       \
    throw utest::AssertionException(ss.str(), __FILE__, __LINE__, __PRETTY_FUNCTION__); \
  }                                                                 \
}

#define UTEST_ASSERT_EQUALS_MSG( x, y, msg )                       \
{                                                                   \
  if( ( x ) != ( y ) )                                              \
  {                                                                 \
    std::ostringstream ss;                                          \
    ss << "Assertion failed, '" << msg << "': "                     \
       << UTEST_TO_STRING( ( x ) ) << " != " << UTEST_TO_STRING( ( y ) ); \
    throw utest::AssertionException(ss.str(), __FILE__, __LINE__, __PRETTY_FUNCTION__); \
  }                                                                 \
}

#define UTEST_ASSERT_NULL( ptr )                                   \
{                                                                   \
  if( ( ptr ) != nullptr )                                          \
  {                                                                 \
    std::ostringstream ss;                                          \
    ss << "Assertion failed, pointer is not null: " << (#ptr);      \
    throw utest::AssertionException(ss.str(), __FILE__, __LINE__, __PRETTY_FUNCTION__); \
  }                                                                 \
}

#define UTEST_ASSERT_NOT_NULL( ptr )                               \
{                                                                   \
  if( ( ptr ) == nullptr )                                          \
  {                                                                 \
    std::ostringstream ss;                                          \
    ss << "Assertion failed, pointer is null: '" << (#ptr) << "'";  \
    throw utest::AssertionException(ss.str(), __FILE__, __LINE__, __PRETTY_FUNCTION__); \
  }                                                                 \
}

// New assertion macros

#define UTEST_ASSERT_NOT_EQUALS( x, y )                            \
{                                                                   \
  if( ( x ) == ( y ) )                                              \
  {                                                                 \
    std::ostringstream ss;                                          \
    ss << "Assertion failed: " << UTEST_TO_STRING( ( x ) )          \
       << " == " << UTEST_TO_STRING( ( y ) );                       \
    throw utest::AssertionException(ss.str(), __FILE__, __LINE__, __PRETTY_FUNCTION__); \
  }                                                                 \
}

#define UTEST_ASSERT_STR_EQUALS( x, y )                            \
{                                                                   \
  if( std::string( x ) != std::string( y ) )                       \
  {                                                                 \
    std::ostringstream ss;                                          \
    ss << "String assertion failed: \"" << std::string( x )         \
       << "\" != \"" << std::string( y ) << "\"";                   \
    throw utest::AssertionException(ss.str(), __FILE__, __LINE__, __PRETTY_FUNCTION__); \
  }                                                                 \
}

#define UTEST_ASSERT_STR_NOT_EQUALS( x, y )                        \
{                                                                   \
  if( std::string( x ) == std::string( y ) )                       \
  {                                                                 \
    std::ostringstream ss;                                          \
    ss << "String assertion failed: \"" << std::string( x )         \
       << "\" == \"" << std::string( y ) << "\"";                   \
    throw utest::AssertionException(ss.str(), __FILE__, __LINE__, __PRETTY_FUNCTION__); \
  }                                                                 \
}

#define UTEST_ASSERT_GT( x, y )                                    \
{                                                                   \
  if( !( ( x ) > ( y ) ) )                                          \
  {                                                                 \
    std::ostringstream ss;                                          \
    ss << "Assertion failed: " << UTEST_TO_STRING( ( x ) )          \
       << " is not greater than " << UTEST_TO_STRING( ( y ) );      \
    throw utest::AssertionException(ss.str(), __FILE__, __LINE__, __PRETTY_FUNCTION__); \
  }                                                                 \
}

#define UTEST_ASSERT_GTE( x, y )                                   \
{                                                                   \
  if( !( ( x ) >= ( y ) ) )                                         \
  {                                                                 \
    std::ostringstream ss;                                          \
    ss << "Assertion failed: " << UTEST_TO_STRING( ( x ) )          \
       << " is not greater than or equal to " << UTEST_TO_STRING( ( y ) ); \
    throw utest::AssertionException(ss.str(), __FILE__, __LINE__, __PRETTY_FUNCTION__); \
  }                                                                 \
}

#define UTEST_ASSERT_LT( x, y )                                    \
{                                                                   \
  if( !( ( x ) < ( y ) ) )                                          \
  {                                                                 \
    std::ostringstream ss;                                          \
    ss << "Assertion failed: " << UTEST_TO_STRING( ( x ) )          \
       << " is not less than " << UTEST_TO_STRING( ( y ) );         \
    throw utest::AssertionException(ss.str(), __FILE__, __LINE__, __PRETTY_FUNCTION__); \
  }                                                                 \
}

#define UTEST_ASSERT_LTE( x, y )                                   \
{                                                                   \
  if( !( ( x ) <= ( y ) ) )                                         \
  {                                                                 \
    std::ostringstream ss;                                          \
    ss << "Assertion failed: " << UTEST_TO_STRING( ( x ) )          \
       << " is not less than or equal to " << UTEST_TO_STRING( ( y ) ); \
    throw utest::AssertionException(ss.str(), __FILE__, __LINE__, __PRETTY_FUNCTION__); \
  }                                                                 \
}

// MSG versions for all assertion macros

#define UTEST_ASSERT_NOT_EQUALS_MSG( x, y, msg )                   \
{                                                                   \
  if( ( x ) == ( y ) )                                              \
  {                                                                 \
    std::ostringstream ss;                                          \
    ss << "Assertion failed, '" << msg << "': "                     \
       << UTEST_TO_STRING( ( x ) ) << " == " << UTEST_TO_STRING( ( y ) ); \
    throw utest::AssertionException(ss.str(), __FILE__, __LINE__, __PRETTY_FUNCTION__); \
  }                                                                 \
}

#define UTEST_ASSERT_STR_EQUALS_MSG( x, y, msg )                   \
{                                                                   \
  if( std::string( x ) != std::string( y ) )                       \
  {                                                                 \
    std::ostringstream ss;                                          \
    ss << "String assertion failed, '" << msg << "': \""            \
       << std::string( x ) << "\" != \"" << std::string( y ) << "\""; \
    throw utest::AssertionException(ss.str(), __FILE__, __LINE__, __PRETTY_FUNCTION__); \
  }                                                                 \
}

#define UTEST_ASSERT_STR_NOT_EQUALS_MSG( x, y, msg )               \
{                                                                   \
  if( std::string( x ) == std::string( y ) )                       \
  {                                                                 \
    std::ostringstream ss;                                          \
    ss << "String assertion failed, '" << msg << "': \""            \
       << std::string( x ) << "\" == \"" << std::string( y ) << "\""; \
    throw utest::AssertionException(ss.str(), __FILE__, __LINE__, __PRETTY_FUNCTION__); \
  }                                                                 \
}

#define UTEST_ASSERT_GT_MSG( x, y, msg )                           \
{                                                                   \
  if( !( ( x ) > ( y ) ) )                                          \
  {                                                                 \
    std::ostringstream ss;                                          \
    ss << "Assertion failed, '" << msg << "': "                     \
       << UTEST_TO_STRING( ( x ) ) << " is not greater than "       \
       << UTEST_TO_STRING( ( y ) );                                 \
    throw utest::AssertionException(ss.str(), __FILE__, __LINE__, __PRETTY_FUNCTION__); \
  }                                                                 \
}

#define UTEST_ASSERT_GTE_MSG( x, y, msg )                          \
{                                                                   \
  if( !( ( x ) >= ( y ) ) )                                         \
  {                                                                 \
    std::ostringstream ss;                                          \
    ss << "Assertion failed, '" << msg << "': "                     \
       << UTEST_TO_STRING( ( x ) ) << " is not greater than or equal to " \
       << UTEST_TO_STRING( ( y ) );                                 \
    throw utest::AssertionException(ss.str(), __FILE__, __LINE__, __PRETTY_FUNCTION__); \
  }                                                                 \
}

#define UTEST_ASSERT_LT_MSG( x, y, msg )                           \
{                                                                   \
  if( !( ( x ) < ( y ) ) )                                          \
  {                                                                 \
    std::ostringstream ss;                                          \
    ss << "Assertion failed, '" << msg << "': "                     \
       << UTEST_TO_STRING( ( x ) ) << " is not less than "          \
       << UTEST_TO_STRING( ( y ) );                                 \
    throw utest::AssertionException(ss.str(), __FILE__, __LINE__, __PRETTY_FUNCTION__); \
  }                                                                 \
}

#define UTEST_ASSERT_LTE_MSG( x, y, msg )                          \
{                                                                   \
  if( !( ( x ) <= ( y ) ) )                                         \
  {                                                                 \
    std::ostringstream ss;                                          \
    ss << "Assertion failed, '" << msg << "': "                     \
       << UTEST_TO_STRING( ( x ) ) << " is not less than or equal to " \
       << UTEST_TO_STRING( ( y ) );                                 \
    throw utest::AssertionException(ss.str(), __FILE__, __LINE__, __PRETTY_FUNCTION__); \
  }                                                                 \
}

#define UTEST_ASSERT_NULL_MSG( ptr, msg )                          \
{                                                                   \
  if( ( ptr ) != nullptr )                                          \
  {                                                                 \
    std::ostringstream ss;                                          \
    ss << "Assertion failed, '" << msg << "': pointer is not null: " << (#ptr); \
    throw utest::AssertionException(ss.str(), __FILE__, __LINE__, __PRETTY_FUNCTION__); \
  }                                                                 \
}

#define UTEST_ASSERT_NOT_NULL_MSG( ptr, msg )                      \
{                                                                   \
  if( ( ptr ) == nullptr )                                          \
  {                                                                 \
    std::ostringstream ss;                                          \
    ss << "Assertion failed, '" << msg << "': pointer is null: '" << (#ptr) << "'"; \
    throw utest::AssertionException(ss.str(), __FILE__, __LINE__, __PRETTY_FUNCTION__); \
  }                                                                 \
}

// Aliases for convenience
#define UTEST_ASSERT_EQ( x, y ) UTEST_ASSERT_EQUALS( x, y )
#define UTEST_ASSERT_NEQ( x, y ) UTEST_ASSERT_NOT_EQUALS( x, y )
#define UTEST_ASSERT_SEQ( x, y ) UTEST_ASSERT_STR_EQUALS( x, y )
#define UTEST_ASSERT_SNEQ( x, y ) UTEST_ASSERT_STR_NOT_EQUALS( x, y )

namespace details {

    // Test tracking structure
    struct TestResult {
        std::string name;
        bool passed;
        std::string error;
    };
    
    // Global test results collection
    inline std::vector<TestResult>& getTestResults() {
        static std::vector<TestResult> results;
        return results;
    }
    
    // Configuration for handling no-tests scenario
    inline bool& getAllowEmptyTests() {
        static bool allowEmpty = false;
        return allowEmpty;
    }

    template<typename Func>
    void testFunc(const char *name, Func f, bool &failed) {
        TestResult result;
        result.name = name;
        result.passed = true;
        
        try {
            f();
            std::cout << "✓ Test [" << std::string(name) << "] " << "succeeded\n";
        }
        catch (const AssertionException &e) {
            std::cout << "✗ Test [" << std::string(name) << "] failed!, error: " << e.getFormattedMessage() << "\n";
            failed = true;
            result.passed = false;
            result.error = e.getFormattedMessage();
        }
        catch (std::exception &e) {
            std::cout << "✗ Test [" << std::string(name) << "] failed with unexpected exception!, error: " << e.what() << "\n";
            failed = true;
            result.passed = false;
            result.error = e.what();
        }
        
        getTestResults().push_back(result);
    }

    template<typename Func>
    inline void AssertThrows(Func assertion, const std::string &msg = "") {
        bool throwFound = false;
        std::string exceptionMsg;
        try {
            assertion();
        }
        catch (const std::exception& e) {
            throwFound = true;
            exceptionMsg = e.what();
        }
        catch (...) {
            throwFound = true;
            exceptionMsg = "unknown exception";
        }

        if (!throwFound) {
            if (msg.size() > 0) {
                std::ostringstream ss;
                ss << "Expected exception was not thrown: " << msg;
                throw utest::AssertionException(ss.str());
            } else {
                throw utest::AssertionException("Expected exception was not thrown");
            }
        }
    }

    template<typename Func>
    inline void AssertDoesNotThrow(Func assertion, const std::string &msg = "") {
        try {
            assertion();
        }
        catch (const std::exception& e) {
            if (msg.size() > 0) {
                std::ostringstream ss;
                ss << "Unexpected exception thrown: " << msg << " - " << e.what();
                throw utest::AssertionException(ss.str());
            } else {
                std::ostringstream ss;
                ss << "Unexpected exception thrown: " << e.what();
                throw utest::AssertionException(ss.str());
            }
        }
        catch (...) {
            if (msg.size() > 0) {
                std::ostringstream ss;
                ss << "Unexpected unknown exception thrown: " << msg;
                throw utest::AssertionException(ss.str());
            } else {
                throw utest::AssertionException("Unexpected unknown exception thrown");
            }
        }
    }
} // namespace

#define UTEST_ASSERT_THROWS(F) utest::details::AssertThrows(F)

#define UTEST_ASSERT_THROWS_MSG(F, MSG) utest::details::AssertThrows(F, std::string(MSG))

#define UTEST_ASSERT_DOES_NOT_THROW(F) utest::details::AssertDoesNotThrow(F)

#define UTEST_ASSERT_DOES_NOT_THROW_MSG(F, MSG) utest::details::AssertDoesNotThrow(F, std::string(MSG))

// Function definition macro
#define UTEST_FUNC_DEF(a, b) void test_##a##_##b()

// Test function execution macros
#define UTEST_FUNC(a) utest::details::testFunc(#a, test_##a, errorFound)
#define UTEST_FUNC2(a, b) utest::details::testFunc(#a"_"#b, test_##a##_##b, errorFound)

#define UTEST_PROLOG() bool errorFound = false; \
    utest::details::getTestResults().clear()

#define UTEST_EPILOG() do { \
    std::cout << "\n======================================\n"; \
    std::cout << "Test Summary:\n"; \
    std::cout << "======================================\n"; \
    auto& results = utest::details::getTestResults(); \
    int passed = 0, failed = 0; \
    if (results.empty()) { \
        std::cout << "No tests were run!\n"; \
        std::cout << "======================================\n"; \
        if (utest::details::getAllowEmptyTests()) { \
            std::cout << "SUCCESS (empty tests allowed)\n"; \
            return EXIT_SUCCESS; \
        } else { \
            std::cout << "FAILURE\n"; \
            return EXIT_FAILURE; \
        } \
    } \
    for (const auto& result : results) { \
        if (result.passed) { \
            std::cout << "✓ " << result.name << "\n"; \
            passed++; \
        } else { \
            std::cout << "✗ " << result.name << " - " << result.error << "\n"; \
            failed++; \
        } \
    } \
    std::cout << "--------------------------------------\n"; \
    std::cout << "Total: " << (passed + failed) << " tests, "; \
    std::cout << passed << " passed ✓, " << failed << " failed ✗\n"; \
    std::cout << "======================================\n"; \
    if (errorFound || failed > 0) { \
        std::cout << "FAILURE\n"; \
        return EXIT_FAILURE; \
    } else { \
        std::cout << "SUCCESS\n"; \
        return EXIT_SUCCESS; \
    } \
} while(0)

} // namespace

#endif

