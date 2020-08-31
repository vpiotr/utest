# Overview
Micro (header-only) library for unit testing.

Initial code based on blog post by Bastian Rieck, 
see: https://bastian.rieck.me/blog/posts/2017/simple_unit_tests/

Library author: Piotr Likus

Created: 31/08/2020

# How to use
Create a runner application executing single function which executes all tests plus adds prolog (UTEST_PROLOG) and epilog (UTEST_EPILOG) code.
 
See tests in "tests" subdirectory for details.

There are two ways of runner execution:
1) As standard application - from IDE or console, this will show which tests are failing

2) As `make test` - this will only show which runners are failing 
 
# License
See LICENSE.txt