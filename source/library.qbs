import qbs

StaticLibrary {
    name: "geohex"
    Depends {
        name: "cpp"
    }
    files: [
        "api/kmx/geohex/base.hpp",
        "api/kmx/geohex/cell.hpp",
        "api/kmx/geohex/cell/area.hpp",
        "api/kmx/geohex/cell/base.hpp",
        "api/kmx/geohex/cell/boundary.hpp",
        "api/kmx/geohex/cell/pentagon.hpp",
        "api/kmx/geohex/coordinate/ij.hpp",
        "api/kmx/geohex/coordinate/ijk.hpp",
        "api/kmx/geohex/geo_projection.hpp",
        "api/kmx/geohex/icosahedron/face.hpp",
        "api/kmx/geohex/index.hpp",
        "api/kmx/geohex/index_hash.hpp",
        "inc/kmx/math/vector.hpp",
        "inc/kmx/unsafe_ipow.hpp",
        "inc/kmx/gis/wgs84/coordinate.hpp",
        "src/kmx/geohex/base.cpp",
        "src/kmx/geohex/cell.cpp",
        "src/kmx/geohex/cell/area.cpp",
        "src/kmx/geohex/cell/base.cpp",
        "src/kmx/geohex/cell/boundary.cpp",
        "src/kmx/geohex/cell/pentagon.cpp",
        "src/kmx/geohex/coordinate/ijk.cpp",
        "src/kmx/geohex/geo_projection.cpp",
        "src/kmx/geohex/icosahedron/face.cpp",
        "src/kmx/geohex/index.cpp",
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
