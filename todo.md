* ✓ add new macros:
  * ✓ UTEST_ASSERT_NOT_EQUALS - !equals
  * ✓ UTEST_ASSERT_STR_EQUALS - equals for for string literals
  * ✓ UTEST_ASSERT_STR_NOT_EQUALS - !equals for for string literals
* ✓ add new comparisons:
  * ✓ UTEST_ASSERT_GT
  * ✓ UTEST_ASSERT_GTE
  * ✓ UTEST_ASSERT_LT
  * ✓ UTEST_ASSERT_LTE
  * ✓ UTEST_ASSERT_EQ - alias for equals
  * ✓ UTEST_ASSERT_NEQ - alias for not equals
  * ✓ UTEST_ASSERT_SEQ - alias for string literal equals
  * ✓ UTEST_ASSERT_SNEQ - alias for string literal not equals
* ✓ UTEST_FUNC_DEF(LevelManagement, TimestampUtilities)
  expands to: instead of `void test_LevelManagement_TimestampUtilities()`
* ✓ UTEST_FUNC(a, b)
  expands to: `utest::details::testFunc(#a#b, test_##a##_##b, errorFound)`
* ✓ show summary of test execution with unicode checkmarks
* ✓ collect statuses of each test execution and at the end show each test with unicode checkmark indicating success or error
  * ✓ in UTEST_PROLOG declare required structures
  * ✓ in testFunc write test status
  * ✓ in UTEST_EPILOG write compact summary
* ✓ at the end show "SUCCESS" or "FAILURE" as one word in last line
* ✓ no tests run is also an error
* ✓ implement UTEST_ASSERT_EQUALS, UTEST_ASSERT_NOT_EQUALS in a way it supports both std::string and (char *) string literals

## Additional Enhancements Completed:
* ✓ Added UTEST_ASSERT_DOES_NOT_THROW and UTEST_ASSERT_DOES_NOT_THROW_MSG
* ✓ Added MSG versions for all assertion macros
* ✓ Implemented advanced convertToString with SFINAE for automatic type detection
* ✓ Moved all tests to tests/ directory with proper naming convention
* ✓ Created comprehensive test suite with edge cases and realistic scenarios
* ✓ Enhanced test runner script with detailed output options

* rename existing UTEST_FUNC_DEF to UTEST_FUNC_DEF2
* add UTEST_FUNC_DEF with single parameter
* when UTEST_FUNC_DEF2 with UTEST_FUNC2 is used, all tests with same first parameter for UTEST_FUNC2 should be grouped together 
* add option to use ASCII checkmarks instead of Unicode
* add basic performance information for each test (elapsed time)
* add test for UTEST_ASSERT_EQUALS with std::string and (char *) string literals
* add throwing / not throwing test with functor and lambda
