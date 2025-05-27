import qbs

StaticLibrary {
    name: "h3"
    Depends {
        name: "cpp"
    }
    files: [
        "api/kmx/h3/base.hpp",
        "api/kmx/h3/cell.hpp",
        "api/kmx/h3/cell/area.hpp",
        "api/kmx/h3/cell/base.hpp",
        "api/kmx/h3/cell/pentagon.hpp",
        "api/kmx/h3/coordinate/ij.hpp",
        "api/kmx/h3/coordinate/ijk.hpp",
        "api/kmx/h3/icosahedron.hpp",
        "api/kmx/h3/index.hpp",
        "inc/kmx/math/vector.hpp",
        "inc/kmx/unsafe_ipow.hpp",
        "inc/kmx/gis/wgs84/coordinate.hpp",
        "src/kmx/h3/cell.cpp",
        "src/kmx/h3/cell/area.cpp",
        "src/kmx/h3/cell/base.cpp",
        "src/kmx/h3/cell/pentagon.cpp",
        "src/kmx/h3/coordinate/ijk.cpp",
        "src/kmx/h3/icosahedron.cpp",
        "src/kmx/h3/index.cpp",
    ]
    cpp.cxxLanguageVersion: "c++23"
    //cpp.cxxFlags: "-gdwarf-4"
    cpp.enableRtti: false
    cpp.includePaths: [
        "api",
        "inc",
        "inc_dep",
    ]
    //cpp.staticLibraries: ["pthread"]
}
