Compiling/running unit tests
------------------------------------

Unit tests will be automatically compiled if dependencies were met in `./configure`
and tests weren't explicitly disabled.

To run the scrytd tests launch `src/test/test_scryt`.

To add more scrytd tests, add `BOOST_AUTO_TEST_CASE` functions to the existing
.cpp files in the `test/` directory or add new .cpp files that
implement new BOOST_AUTO_TEST_SUITE sections.

To run the scryt-qt tests manually, launch `src/qt/test/test_scryt-qt`

To add more scryt-qt tests, add them to the `src/qt/test/` directory and
the `src/qt/test/test_main.cpp` file.
