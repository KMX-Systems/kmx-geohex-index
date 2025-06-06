import qbs

CppApplication {
    Depends
    {
        name: 'geohex'
    }
    consoleApplication: true
    cpp.debugInformation: true

    files: [
        "src/index_test.cpp",
        "src/util.cpp",
    ]
    cpp.cxxLanguageVersion: "c++23"
    cpp.enableRtti: false
    //cpp.cxxFlags: "-gdwarf-4"
    cpp.includePaths: [
        "inc",
        "inc_dep"
    ]
    //cpp.includePaths: ["/usr/local/include"]
    //cpp.libraryPaths: ["/usr/local/lib"]
    cpp.dynamicLibraries: ["Catch2Main", "Catch2"]
}
