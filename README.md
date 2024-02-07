This is a basic header-only library for passing messages 
between threads safely.

To use it as a library just add to your source/external directory within your module.

    add_subdirectory(...the directory of TSML...)
    target_link_library({PROJECT_NAME} {VISIBILITY_CHOICE} TSML)

That should be enough to use it. You can find the basic usage of these
headers in tests directory.

