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

// Custom exception class for assertion failures
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

namespace details {

    template<typename Func>
    void testFunc(const char *name, Func f, bool &failed) {
        try {
            f();
            std::cout << "OK: Test [" << std::string(name) << "] " << "succeeded\n";
        }
        catch (const AssertionException &e) {
            std::cout << "Error: Test [" << std::string(name) << "] failed!, error: " << e.getFormattedMessage() << "\n";
            failed = true;
        }
        catch (std::exception &e) {
            std::cout << "Error: Test [" << std::string(name) << "] failed with unexpected exception!, error: " << e.what() << "\n";
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
                throw utest::AssertionException(ss.str());
            } else {
                throw utest::AssertionException("assertion.throws failed");
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

