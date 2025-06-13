import qbs

StaticLibrary {
    name: "geohex"
    Depends {
        name: "cpp"
    }
    files: [
        "api/kmx/geohex/base.hpp",
        "api/kmx/geohex/index.hpp",
        "inc/kmx/geohex/cell.hpp",
        "inc/kmx/geohex/cell/area.hpp",
        "inc/kmx/geohex/cell/base.hpp",
        "inc/kmx/geohex/cell/boundary.hpp",
        "inc/kmx/geohex/cell/pentagon.hpp",
        "inc/kmx/geohex/coordinate/ij.hpp",
        "inc/kmx/geohex/coordinate/ijk.hpp",
        "inc/kmx/geohex/icosahedron/face.hpp",
        "inc/kmx/geohex/index_helper.hpp",
        "inc/kmx/geohex/projection.hpp",
        "inc/kmx/geohex/utils.hpp",
        "inc/kmx/gis/wgs84/coordinate.hpp",
        "inc/kmx/math/vector.hpp",
        "src/kmx/geohex/cell.cpp",
        "src/kmx/geohex/cell/area.cpp",
        "src/kmx/geohex/cell/base.cpp",
        "src/kmx/geohex/cell/boundary.cpp",
        "src/kmx/geohex/cell/pentagon.cpp",
        "src/kmx/geohex/coordinate/ijk.cpp",
        "src/kmx/geohex/icosahedron/face.cpp",
        "src/kmx/geohex/index.cpp",
        "src/kmx/geohex/index_helper.cpp",
        "src/kmx/geohex/projection.cpp",
        "src/kmx/geohex/utils.cpp",
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
