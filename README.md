This is a basic header-only library for passing messages 
between threads safely. It is probably not suitable for 
advanced projects, and it has possibly full of bugs.

To use it as a library just add to your source directory. Set the
TEST_STATUS flag off in main CMakeLists.txt and link it with the
following code in your .

    add_subdirectory(...the directory of TSML...)
    target_link_library({PROJECT_NAME} {VISIBILITY_CHOICE} TSML)

That should be enough to use it. You can find the basic usage of these
headers in tests directory.

