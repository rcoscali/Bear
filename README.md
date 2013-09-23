Build EAR
=========

Bear is a tool to generate compilation database for clang tooling.

The [JSON compilation database][JSONCDB] is used in clang project to provide
information how a single compilation unit was processed. When that
is available then it is easy to re-run the compilation with different
programs.

One way to get compilation database is to use `cmake` as build tool. Passing
`-DCMAKE_EXPORT_COMPILE_COMMANDS=ON` to cmake generates `compile_commands.json`
file into the current directory.

When the project compiles with no cmake, but another build system, there is
no free json file. Bear is a tool to generate such file during the build
process.

The concept behind Bear is to exec the original build command and
intercept the `exec` calls of the build tool. To achive that Bear uses the
`LD_PRELOAD` or `DYLD_INSERT_LIBRARIES` mechanisms provided by the dynamic
linker. So it has two components: the library and the binary. The library
defines the `exec` methods and used in every child processes. The executable
sets the environment up to child processes and writes the output file.

  [JSONCDB]: http://clang.llvm.org/docs/JSONCompilationDatabase.html


How to build
------------

Bear should be quiet portable on UNIX OSs, it has been tested on FreeBSD,
Linux and OS X only.

### Prerequisites

1. ANSI **C compiler** to compile the sources.
2. **cmake** to configure the build process.
3. **make** to run the build. Makefiles are generated by `cmake`.
4. **libconfig** to parse the config file. (Version shall be greater than 1.4)
5. Optional **pandoc** to generate man page from markdown.

### Build commands

It could be the best to build it in a separate build directory.

    cmake $BEAR_SOURCE_DIR
    make all
    make install # to install
    make check   # to run tests
    make package # to create tgz, rpm, deb packages

You can configure the build process with passing arguments to cmake.


How to use
----------

After installation the usage is like this:

    bear -- make

The `--` separate the parameters from the build command. The output file
called `compile_commands.json` found  in current directory.

For more options you can check the man page or pass `--help` parameter.


Known issues
------------

Compiler wrappers like [ccache][CCACHE] and [distcc][DISTCC] could cause
duplicates or missing items in the compilation database. Make sure you have
been disabled before you run Bear.

  [CCACHE]: http://ccache.samba.org/
  [DISTCC]: http://code.google.com/p/distcc/


Problem reports
---------------

If you find a bug in this documentation or elsewhere in the program or would
like to propose an improvement, please use the project's [github issue
tracker][ISSUES]. Please describing the bug and where you found it. If you
have a suggestion how to fix it, include that as well. Patches are also
welcome.

  [ISSUES]: https://github.com/rizsotto/Bear/issues
