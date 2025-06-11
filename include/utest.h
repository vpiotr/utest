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
#include <chrono>
#include <map>
#include <iomanip>
#include <iomanip>

namespace utest {

#define UTEST_FUNC(a) utest::details::testFunc(#a, test_##a, errorFound)

#define UTEST_PROLOG() bool errorFound = false; \
    utest::details::getTestResults().clear()

#define UTEST_ALLOW_EMPTY_TESTS() utest::details::getAllowEmptyTests() = true

#define UTEST_USE_ASCII_CHECKMARKS() utest::details::getUseAsciiCheckmarks() = true

#define UTEST_SHOW_PERFORMANCE() utest::details::getShowPerformanceInfo() = true

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
        std::string group; // For grouping tests by first parameter
        bool passed;
        std::string error;
        double elapsedTime; // in milliseconds
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
    
    // Configuration for ASCII vs Unicode checkmarks
    inline bool& getUseAsciiCheckmarks() {
        static bool useAscii = false;
        return useAscii;
    }
    
    // Configuration for showing performance information
    inline bool& getShowPerformanceInfo() {
        static bool showPerf = false;
        return showPerf;
    }

    template<typename Func>
    void testFunc(const char *name, Func f, bool &failed) {
        TestResult result;
        result.name = name;
        result.group = ""; // No group for single tests
        result.passed = true;
        result.elapsedTime = 0.0;
        
        // Get checkmark symbols
        const char* successMark = getUseAsciiCheckmarks() ? "[OK]" : "✓";
        const char* failMark = getUseAsciiCheckmarks() ? "[FAIL]" : "✗";
        
        auto start = std::chrono::high_resolution_clock::now();
        
        try {
            f();
            auto end = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
            result.elapsedTime = duration.count() / 1000.0; // Convert to milliseconds
            
            std::cout << successMark << " Test [" << std::string(name) << "] succeeded";
            if (getShowPerformanceInfo()) {
                std::cout << " (" << std::fixed << std::setprecision(3) << result.elapsedTime << "ms)";
            }
            std::cout << "\n";
        }
        catch (const AssertionException &e) {
            auto end = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
            result.elapsedTime = duration.count() / 1000.0;
            
            std::cout << failMark << " Test [" << std::string(name) << "] failed!, error: " << e.getFormattedMessage();
            if (getShowPerformanceInfo()) {
                std::cout << " (" << std::fixed << std::setprecision(3) << result.elapsedTime << "ms)";
            }
            std::cout << "\n";
            failed = true;
            result.passed = false;
            result.error = e.getFormattedMessage();
        }
        catch (std::exception &e) {
            auto end = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
            result.elapsedTime = duration.count() / 1000.0;
            
            std::cout << failMark << " Test [" << std::string(name) << "] failed with unexpected exception!, error: " << e.what();
            if (getShowPerformanceInfo()) {
                std::cout << " (" << std::fixed << std::setprecision(3) << result.elapsedTime << "ms)";
            }
            std::cout << "\n";
            failed = true;
            result.passed = false;
            result.error = e.what();
        }
        
        getTestResults().push_back(result);
    }

    // Overloaded version for grouped tests (UTEST_FUNC2)
    template<typename Func>
    void testFunc2(const char *group, const char *name, Func f, bool &failed) {
        TestResult result;
        result.name = name;
        result.group = group;
        result.passed = true;
        result.elapsedTime = 0.0;
        
        // Get checkmark symbols
        const char* successMark = getUseAsciiCheckmarks() ? "[OK]" : "✓";
        const char* failMark = getUseAsciiCheckmarks() ? "[FAIL]" : "✗";
        
        auto start = std::chrono::high_resolution_clock::now();
        
        try {
            f();
            auto end = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
            result.elapsedTime = duration.count() / 1000.0; // Convert to milliseconds
            
            std::cout << successMark << " Test [" << std::string(group) << "::" << std::string(name) << "] succeeded";
            if (getShowPerformanceInfo()) {
                std::cout << " (" << std::fixed << std::setprecision(3) << result.elapsedTime << "ms)";
            }
            std::cout << "\n";
        }
        catch (const AssertionException &e) {
            auto end = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
            result.elapsedTime = duration.count() / 1000.0;
            
            std::cout << failMark << " Test [" << std::string(group) << "::" << std::string(name) << "] failed!, error: " << e.getFormattedMessage();
            if (getShowPerformanceInfo()) {
                std::cout << " (" << std::fixed << std::setprecision(3) << result.elapsedTime << "ms)";
            }
            std::cout << "\n";
            failed = true;
            result.passed = false;
            result.error = e.getFormattedMessage();
        }
        catch (std::exception &e) {
            auto end = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
            result.elapsedTime = duration.count() / 1000.0;
            
            std::cout << failMark << " Test [" << std::string(group) << "::" << std::string(name) << "] failed with unexpected exception!, error: " << e.what();
            if (getShowPerformanceInfo()) {
                std::cout << " (" << std::fixed << std::setprecision(3) << result.elapsedTime << "ms)";
            }
            std::cout << "\n";
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

// Function definition macros
#define UTEST_FUNC_DEF(a) void test_##a()
#define UTEST_FUNC_DEF2(a, b) void test_##a##_##b()

// Test function execution macros
#define UTEST_FUNC(a) utest::details::testFunc(#a, test_##a, errorFound)
#define UTEST_FUNC2(a, b) utest::details::testFunc2(#a, #b, test_##a##_##b, errorFound)

#define UTEST_PROLOG() bool errorFound = false; \
    utest::details::getTestResults().clear()

#define UTEST_EPILOG() do { \
    std::cout << "\n======================================\n"; \
    std::cout << "Test Summary:\n"; \
    std::cout << "======================================\n"; \
    auto& results = utest::details::getTestResults(); \
    int passed = 0, failed = 0; \
    double totalTime = 0.0; \
    const char* successMark = utest::details::getUseAsciiCheckmarks() ? "[OK]" : "✓"; \
    const char* failMark = utest::details::getUseAsciiCheckmarks() ? "[FAIL]" : "✗"; \
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
    std::map<std::string, std::vector<const utest::details::TestResult*>> groupedResults; \
    for (const auto& result : results) { \
        if (result.group.empty()) { \
            groupedResults[""].push_back(&result); \
        } else { \
            groupedResults[result.group].push_back(&result); \
        } \
        totalTime += result.elapsedTime; \
    } \
    for (const auto& group : groupedResults) { \
        if (!group.first.empty()) { \
            std::cout << "\n" << group.first << ":\n"; \
        } \
        for (const auto* result : group.second) { \
            if (result->passed) { \
                std::cout << successMark << " "; \
                if (!result->group.empty()) { \
                    std::cout << result->name; \
                } else { \
                    std::cout << result->name; \
                } \
                if (utest::details::getShowPerformanceInfo()) { \
                    std::cout << " (" << std::fixed << std::setprecision(3) << result->elapsedTime << "ms)"; \
                } \
                std::cout << "\n"; \
                passed++; \
            } else { \
                std::cout << failMark << " "; \
                if (!result->group.empty()) { \
                    std::cout << result->name; \
                } else { \
                    std::cout << result->name; \
                } \
                std::cout << " - " << result->error; \
                if (utest::details::getShowPerformanceInfo()) { \
                    std::cout << " (" << std::fixed << std::setprecision(3) << result->elapsedTime << "ms)"; \
                } \
                std::cout << "\n"; \
                failed++; \
            } \
        } \
    } \
    std::cout << "--------------------------------------\n"; \
    std::cout << "Total: " << (passed + failed) << " tests, "; \
    std::cout << passed << " passed " << successMark << ", " << failed << " failed " << failMark; \
    if (utest::details::getShowPerformanceInfo()) { \
        std::cout << " (Total time: " << std::fixed << std::setprecision(3) << totalTime << "ms)"; \
    } \
    std::cout << "\n"; \
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

