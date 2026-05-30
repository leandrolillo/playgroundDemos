#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>

// Pre-include system headers to satisfy missing includes in the fetched
// dependency headers (math, playground) that were developed on macOS and are
// missing these on Linux/GCC.
#include <stdexcept>
#include <cstring>
#include <memory>
#include <algorithm>
#include <climits>

#include <Math3d.h>
#include <Logger.h>    // must precede MimeTypes.h which uses Logger without including it
#include <MimeTypes.h>
#include <Paths.h>
#include <StringUtils.h>
#include <BreakoutLevel.h>

using Catch::Approx;

// ─────────────────────────────────────────────────────────────────────────────
// vector2
// ─────────────────────────────────────────────────────────────────────────────
TEST_CASE("vector2: construction", "[math][vector2]")
{
    SECTION("Default constructor produces zero vector") {
        vector2 v;
        REQUIRE(v.x == 0.0f);
        REQUIRE(v.y == 0.0f);
    }

    SECTION("Parameterized constructor stores components") {
        vector2 v(3.0f, -4.0f);
        REQUIRE(v.x == 3.0f);
        REQUIRE(v.y == -4.0f);
    }

    SECTION("uv aliases match xy") {
        vector2 v(1.0f, 2.0f);
        REQUIRE(v.u == v.x);
        REQUIRE(v.v == v.y);
    }
}

TEST_CASE("vector2: arithmetic", "[math][vector2]")
{
    vector2 a(3.0f, 4.0f);
    vector2 b(1.0f, 2.0f);

    SECTION("Addition") {
        vector2 r = a + b;
        REQUIRE(r.x == Approx(4.0f));
        REQUIRE(r.y == Approx(6.0f));
    }

    SECTION("Subtraction") {
        vector2 r = a - b;
        REQUIRE(r.x == Approx(2.0f));
        REQUIRE(r.y == Approx(2.0f));
    }

    SECTION("Negation") {
        vector2 r = -a;
        REQUIRE(r.x == Approx(-3.0f));
        REQUIRE(r.y == Approx(-4.0f));
    }

    SECTION("Scalar multiplication (right)") {
        vector2 r = a * 2.0f;
        REQUIRE(r.x == Approx(6.0f));
        REQUIRE(r.y == Approx(8.0f));
    }

    SECTION("Scalar multiplication (left)") {
        vector2 r = 2.0f * a;
        REQUIRE(r.x == Approx(6.0f));
        REQUIRE(r.y == Approx(8.0f));
    }

    SECTION("Dot product") {
        real dot = a * b;  // 3*1 + 4*2 = 11
        REQUIRE(dot == Approx(11.0f));
    }

    SECTION("Compound addition") {
        vector2 r = a;
        r += b;
        REQUIRE(r.x == Approx(4.0f));
        REQUIRE(r.y == Approx(6.0f));
    }

    SECTION("Compound subtraction") {
        vector2 r = a;
        r -= b;
        REQUIRE(r.x == Approx(2.0f));
        REQUIRE(r.y == Approx(2.0f));
    }

    SECTION("Compound scalar multiplication") {
        vector2 r = a;
        r *= 3.0f;
        REQUIRE(r.x == Approx(9.0f));
        REQUIRE(r.y == Approx(12.0f));
    }
}

TEST_CASE("vector2: modulo", "[math][vector2]")
{
    SECTION("3-4-5 right triangle") {
        vector2 v(3.0f, 4.0f);
        REQUIRE(v.modulo() == Approx(5.0f));
    }

    SECTION("Unit vector on x-axis") {
        vector2 v(1.0f, 0.0f);
        REQUIRE(v.modulo() == Approx(1.0f));
    }

    SECTION("Zero vector has zero length") {
        vector2 v;
        REQUIRE(v.modulo() == Approx(0.0f));
    }
}

TEST_CASE("vector2: normalize", "[math][vector2]")
{
    SECTION("Normalized vector has unit length") {
        vector2 v(3.0f, 4.0f);
        vector2 n = v.normalizado();
        REQUIRE(n.modulo() == Approx(1.0f));
    }

    SECTION("Direction is preserved after normalization") {
        vector2 v(3.0f, 4.0f);
        vector2 n = v.normalizado();
        REQUIRE(n.x == Approx(0.6f));
        REQUIRE(n.y == Approx(0.8f));
    }

    SECTION("Normalizing zero vector throws") {
        vector2 v(0.0f, 0.0f);
        REQUIRE_THROWS_AS(v.normalizado(), std::invalid_argument);
    }
}

TEST_CASE("vector2: index access", "[math][vector2]")
{
    vector2 v(7.0f, 8.0f);

    SECTION("Index 0 returns x") {
        REQUIRE(v(0) == Approx(7.0f));
    }

    SECTION("Index 1 returns y") {
        REQUIRE(v(1) == Approx(8.0f));
    }

    SECTION("Out-of-bounds index throws") {
        REQUIRE_THROWS_AS(v(2), std::out_of_range);
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// vector3
// ─────────────────────────────────────────────────────────────────────────────
TEST_CASE("vector3: construction", "[math][vector3]")
{
    SECTION("Default constructor produces zero vector") {
        vector3 v;
        REQUIRE(v.x == 0.0f);
        REQUIRE(v.y == 0.0f);
        REQUIRE(v.z == 0.0f);
    }

    SECTION("Parameterized constructor stores components") {
        vector3 v(1.0f, -2.0f, 3.0f);
        REQUIRE(v.x == 1.0f);
        REQUIRE(v.y == -2.0f);
        REQUIRE(v.z == 3.0f);
    }

    SECTION("Constructor from vector2 sets z to zero by default") {
        vector2 xy(5.0f, 6.0f);
        vector3 v(xy);
        REQUIRE(v.x == 5.0f);
        REQUIRE(v.y == 6.0f);
        REQUIRE(v.z == 0.0f);
    }

    SECTION("Constructor from vector2 with explicit z") {
        vector2 xy(5.0f, 6.0f);
        vector3 v(xy, 9.0f);
        REQUIRE(v.z == 9.0f);
    }
}

TEST_CASE("vector3: arithmetic", "[math][vector3]")
{
    vector3 a(1.0f, 2.0f, 3.0f);
    vector3 b(4.0f, 5.0f, 6.0f);

    SECTION("Addition") {
        vector3 r = a + b;
        REQUIRE(r.x == Approx(5.0f));
        REQUIRE(r.y == Approx(7.0f));
        REQUIRE(r.z == Approx(9.0f));
    }

    SECTION("Subtraction") {
        vector3 r = a - b;
        REQUIRE(r.x == Approx(-3.0f));
        REQUIRE(r.y == Approx(-3.0f));
        REQUIRE(r.z == Approx(-3.0f));
    }

    SECTION("Negation") {
        vector3 r = -a;
        REQUIRE(r.x == Approx(-1.0f));
        REQUIRE(r.y == Approx(-2.0f));
        REQUIRE(r.z == Approx(-3.0f));
    }

    SECTION("Scalar multiplication (right)") {
        vector3 r = a * 3.0f;
        REQUIRE(r.x == Approx(3.0f));
        REQUIRE(r.y == Approx(6.0f));
        REQUIRE(r.z == Approx(9.0f));
    }

    SECTION("Scalar multiplication (left)") {
        vector3 r = 3.0f * a;
        REQUIRE(r.x == Approx(3.0f));
        REQUIRE(r.y == Approx(6.0f));
        REQUIRE(r.z == Approx(9.0f));
    }

    SECTION("Compound addition") {
        vector3 r = a;
        r += b;
        REQUIRE(r.x == Approx(5.0f));
        REQUIRE(r.y == Approx(7.0f));
        REQUIRE(r.z == Approx(9.0f));
    }

    SECTION("Compound subtraction") {
        vector3 r = a;
        r -= b;
        REQUIRE(r.x == Approx(-3.0f));
        REQUIRE(r.y == Approx(-3.0f));
        REQUIRE(r.z == Approx(-3.0f));
    }

    SECTION("Compound scalar multiplication") {
        vector3 r = a;
        r *= 2.0f;
        REQUIRE(r.x == Approx(2.0f));
        REQUIRE(r.y == Approx(4.0f));
        REQUIRE(r.z == Approx(6.0f));
    }
}

TEST_CASE("vector3: dot product", "[math][vector3]")
{
    SECTION("Parallel vectors") {
        vector3 a(1.0f, 0.0f, 0.0f);
        real dot = a * a;
        REQUIRE(dot == Approx(1.0f));
    }

    SECTION("Perpendicular vectors produce zero") {
        vector3 a(1.0f, 0.0f, 0.0f);
        vector3 b(0.0f, 1.0f, 0.0f);
        REQUIRE((a * b) == Approx(0.0f));
    }

    SECTION("General dot product") {
        vector3 a(1.0f, 2.0f, 3.0f);
        vector3 b(4.0f, -5.0f, 6.0f);  // 4 - 10 + 18 = 12
        REQUIRE((a * b) == Approx(12.0f));
    }
}

TEST_CASE("vector3: cross product", "[math][vector3]")
{
    SECTION("x cross y gives negative z (non-standard left-handed convention)") {
        // WARNING: productoVectorial uses the formula (a.z*b.y-a.y*b.z, a.x*b.z-a.z*b.x, a.y*b.x-a.x*b.y),
        // which is the negation of the standard right-hand cross product. Callers must account for this sign flip.
        vector3 x(1.0f, 0.0f, 0.0f);
        vector3 y(0.0f, 1.0f, 0.0f);
        vector3 r = x ^ y;
        REQUIRE(r.x == Approx(0.0f));
        REQUIRE(r.y == Approx(0.0f));
        REQUIRE(r.z == Approx(-1.0f));
    }

    SECTION("y cross x gives positive z (non-standard left-handed convention)") {
        vector3 x(1.0f, 0.0f, 0.0f);
        vector3 y(0.0f, 1.0f, 0.0f);
        vector3 r = y ^ x;
        REQUIRE(r.z == Approx(1.0f));
    }

    SECTION("Parallel vectors cross product is zero") {
        vector3 a(1.0f, 0.0f, 0.0f);
        vector3 r = a ^ a;
        REQUIRE(r.x == Approx(0.0f));
        REQUIRE(r.y == Approx(0.0f));
        REQUIRE(r.z == Approx(0.0f));
    }

    SECTION("Cross product is perpendicular to both operands") {
        vector3 a(1.0f, 2.0f, 3.0f);
        vector3 b(4.0f, 5.0f, 6.0f);
        vector3 c = a ^ b;
        REQUIRE((a * c) == Approx(0.0f).margin(1e-4f));
        REQUIRE((b * c) == Approx(0.0f).margin(1e-4f));
    }
}

TEST_CASE("vector3: modulo", "[math][vector3]")
{
    SECTION("Unit vector") {
        vector3 v(1.0f, 0.0f, 0.0f);
        REQUIRE(v.modulo() == Approx(1.0f));
    }

    SECTION("Known length") {
        // sqrt(1^2 + 2^2 + 2^2) = sqrt(9) = 3
        vector3 v(1.0f, 2.0f, 2.0f);
        REQUIRE(v.modulo() == Approx(3.0f));
    }

    SECTION("Zero vector") {
        vector3 v;
        REQUIRE(v.modulo() == Approx(0.0f));
    }
}

TEST_CASE("vector3: normalize", "[math][vector3]")
{
    SECTION("Normalized vector has unit length") {
        vector3 v(1.0f, 2.0f, 2.0f);
        vector3 n = v.normalizado();
        REQUIRE(n.modulo() == Approx(1.0f));
    }

    SECTION("Direction is preserved after normalization") {
        vector3 v(0.0f, 0.0f, 5.0f);
        vector3 n = v.normalizado();
        REQUIRE(n.x == Approx(0.0f));
        REQUIRE(n.y == Approx(0.0f));
        REQUIRE(n.z == Approx(1.0f));
    }

    SECTION("Normalizing zero vector throws") {
        vector3 v(0.0f, 0.0f, 0.0f);
        REQUIRE_THROWS_AS(v.normalizado(), std::invalid_argument);
    }
}

TEST_CASE("vector3: xy projection", "[math][vector3]")
{
    vector3 v(3.0f, 4.0f, 5.0f);
    vector2 xy = v.xy();
    REQUIRE(xy.x == Approx(3.0f));
    REQUIRE(xy.y == Approx(4.0f));
}

TEST_CASE("vector3: index access", "[math][vector3]")
{
    vector3 v(10.0f, 20.0f, 30.0f);

    SECTION("Index 0 returns x") {
        REQUIRE(v(0) == Approx(10.0f));
    }

    SECTION("Index 1 returns y") {
        REQUIRE(v(1) == Approx(20.0f));
    }

    SECTION("Index 2 returns z") {
        REQUIRE(v(2) == Approx(30.0f));
    }

    SECTION("Out-of-bounds index throws") {
        REQUIRE_THROWS_AS(v(3), std::out_of_range);
    }
}

TEST_CASE("vector3: equality", "[math][vector3]")
{
    vector3 a(1.0f, 2.0f, 3.0f);
    vector3 b(1.0f, 2.0f, 3.0f);
    vector3 c(1.0f, 2.0f, 4.0f);

    SECTION("Equal vectors") {
        REQUIRE(a == b);
    }

    SECTION("Unequal vectors") {
        REQUIRE(a != c);
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// matriz_mxn
// ─────────────────────────────────────────────────────────────────────────────
TEST_CASE("matriz_mxn: construction and dimensions", "[math][matrix]")
{
    SECTION("Dimensions are stored correctly") {
        matriz_mxn m(3, 4);
        REQUIRE(m.getNroFilas() == 3u);
        REQUIRE(m.getNroColumnas() == 4u);
        REQUIRE(m.getLength() == 12u);
    }

    SECTION("Elements are readable after construction") {
        matriz_mxn m(2, 2);
        // Default-constructed elements should not crash on access
        REQUIRE_NOTHROW(m(0, 0));
        REQUIRE_NOTHROW(m(1, 1));
    }
}

TEST_CASE("matriz_mxn: element access", "[math][matrix]")
{
    matriz_mxn m(2, 3);
    m(0, 0) = 1.0f;
    m(0, 1) = 2.0f;
    m(0, 2) = 3.0f;
    m(1, 0) = 4.0f;
    m(1, 1) = 5.0f;
    m(1, 2) = 6.0f;

    SECTION("Values can be read back") {
        REQUIRE(m(0, 0) == Approx(1.0f));
        REQUIRE(m(0, 2) == Approx(3.0f));
        REQUIRE(m(1, 0) == Approx(4.0f));
        REQUIRE(m(1, 2) == Approx(6.0f));
    }

    SECTION("Row out-of-bounds throws") {
        REQUIRE_THROWS_AS(m(2, 0), std::out_of_range);
    }

    SECTION("Column out-of-bounds throws") {
        REQUIRE_THROWS_AS(m(0, 3), std::out_of_range);
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// MimeTypes
// ─────────────────────────────────────────────────────────────────────────────
TEST_CASE("MimeTypes: defaultInputMimeType", "[resources][mimetypes]")
{
    SECTION("Text file") {
        REQUIRE(MimeTypes::defaultInputMimeType("file.txt") == MimeTypes::TXT);
    }

    SECTION("JSON file") {
        REQUIRE(MimeTypes::defaultInputMimeType("file.json") == MimeTypes::JSON);
    }

    SECTION("PNG image") {
        REQUIRE(MimeTypes::defaultInputMimeType("file.png") == MimeTypes::PNG);
    }

    SECTION("TGA image") {
        REQUIRE(MimeTypes::defaultInputMimeType("file.tga") == MimeTypes::TGA);
    }

    SECTION("JPEG image (.jpeg)") {
        REQUIRE(MimeTypes::defaultInputMimeType("file.jpeg") == MimeTypes::JPEG);
    }

    SECTION("JPEG image (.jpg)") {
        REQUIRE(MimeTypes::defaultInputMimeType("file.jpg") == MimeTypes::JPEG);
    }

    SECTION("GLSL shader") {
        REQUIRE(MimeTypes::defaultInputMimeType("file.glsl") == MimeTypes::GLSL);
    }

    SECTION("OGG audio") {
        REQUIRE(MimeTypes::defaultInputMimeType("file.ogg") == MimeTypes::OGG);
    }

    SECTION("WAV audio") {
        REQUIRE(MimeTypes::defaultInputMimeType("file.wav") == MimeTypes::WAVE);
    }

    SECTION("Wavefront OBJ model") {
        REQUIRE(MimeTypes::defaultInputMimeType("file.obj") == MimeTypes::WAVEFRONT_OBJ);
    }

    SECTION("Wavefront MTL material") {
        REQUIRE(MimeTypes::defaultInputMimeType("file.mtl") == MimeTypes::WAVEFRONT_MATERIAL);
    }

    SECTION("Extension is case-insensitive") {
        REQUIRE(MimeTypes::defaultInputMimeType("FILE.TXT") == MimeTypes::TXT);
        REQUIRE(MimeTypes::defaultInputMimeType("FILE.PNG") == MimeTypes::PNG);
        REQUIRE(MimeTypes::defaultInputMimeType("FILE.OGG") == MimeTypes::OGG);
    }

    SECTION("Unknown extension returns empty string") {
        REQUIRE(MimeTypes::defaultInputMimeType("file.xyz") == "");
    }

    SECTION("No extension returns empty string") {
        REQUIRE(MimeTypes::defaultInputMimeType("filename") == "");
    }

    SECTION("Path with subdirectory") {
        REQUIRE(MimeTypes::defaultInputMimeType("folder/file.json") == MimeTypes::JSON);
    }

    SECTION("Path with object name suffix (bug-fix case)") {
        REQUIRE(MimeTypes::defaultInputMimeType("models/car.obj/car_body") == MimeTypes::WAVEFRONT_OBJ);
    }
}

TEST_CASE("MimeTypes: defaultOutputMimeType", "[resources][mimetypes]")
{
    SECTION("PNG produces image type") {
        REQUIRE(MimeTypes::defaultOutputMimeType("file.png") == MimeTypes::IMAGE);
    }

    SECTION("JPEG produces image type") {
        REQUIRE(MimeTypes::defaultOutputMimeType("file.jpg") == MimeTypes::IMAGE);
        REQUIRE(MimeTypes::defaultOutputMimeType("file.jpeg") == MimeTypes::IMAGE);
    }

    SECTION("TGA produces image type") {
        REQUIRE(MimeTypes::defaultOutputMimeType("file.tga") == MimeTypes::IMAGE);
    }

    SECTION("OGG produces audio type") {
        REQUIRE(MimeTypes::defaultOutputMimeType("file.ogg") == MimeTypes::AUDIO);
    }

    SECTION("WAV produces audio type") {
        REQUIRE(MimeTypes::defaultOutputMimeType("file.wav") == MimeTypes::AUDIO);
    }

    SECTION("OBJ produces geometry type") {
        REQUIRE(MimeTypes::defaultOutputMimeType("file.obj") == MimeTypes::GEOMETRY);
    }

    SECTION("MTL produces material type") {
        REQUIRE(MimeTypes::defaultOutputMimeType("file.mtl") == MimeTypes::MATERIAL);
    }

    SECTION("Extension is case-insensitive") {
        REQUIRE(MimeTypes::defaultOutputMimeType("FILE.OGG") == MimeTypes::AUDIO);
    }

    SECTION("Unknown extension returns empty string") {
        REQUIRE(MimeTypes::defaultOutputMimeType("file.glsl") == "");
    }

    SECTION("Path with object name suffix (bug-fix case)") {
        REQUIRE(MimeTypes::defaultOutputMimeType("models/car.obj/car_body") == MimeTypes::GEOMETRY);
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// StringUtils
// ─────────────────────────────────────────────────────────────────────────────
TEST_CASE("StringUtils: trim", "[utils][string]")
{
    SECTION("Leading spaces") {
        REQUIRE(StringUtils::trim("  hello") == "hello");
    }

    SECTION("Trailing spaces") {
        REQUIRE(StringUtils::trim("hello  ") == "hello");
    }

    SECTION("Both sides") {
        REQUIRE(StringUtils::trim("  hello  ") == "hello");
    }

    SECTION("No whitespace") {
        REQUIRE(StringUtils::trim("hello") == "hello");
    }

    SECTION("Only whitespace becomes empty") {
        REQUIRE(StringUtils::trim("   ") == "");
    }

    SECTION("Tabs and newlines") {
        REQUIRE(StringUtils::trim("\t hello \n") == "hello");
    }
}

TEST_CASE("StringUtils: toLowercase", "[utils][string]")
{
    SECTION("Already lowercase is unchanged") {
        REQUIRE(StringUtils::toLowercase("hello") == "hello");
    }

    SECTION("All uppercase is converted") {
        REQUIRE(StringUtils::toLowercase("HELLO") == "hello");
    }

    SECTION("Mixed case is fully lowered") {
        REQUIRE(StringUtils::toLowercase("HeLLo WoRLd") == "hello world");
    }

    SECTION("Numbers and symbols are unchanged") {
        REQUIRE(StringUtils::toLowercase("ABC123!") == "abc123!");
    }
}

TEST_CASE("StringUtils: split", "[utils][string]")
{
    SECTION("Split on slash") {
        auto parts = StringUtils::split("a/b/c", '/');
        REQUIRE(parts.size() == 3u);
        REQUIRE(parts[0] == "a");
        REQUIRE(parts[1] == "b");
        REQUIRE(parts[2] == "c");
    }

    SECTION("No separator returns original string") {
        auto parts = StringUtils::split("abc", '/');
        REQUIRE(parts.size() == 1u);
        REQUIRE(parts[0] == "abc");
    }

    SECTION("Trailing separator: no empty last token") {
        // StringUtils::split uses std::getline internally; a trailing delimiter
        // does not produce a spurious empty final token.
        auto parts = StringUtils::split("a/b/", '/');
        REQUIRE(parts.size() == 2u);
        REQUIRE(parts[0] == "a");
        REQUIRE(parts[1] == "b");
    }

    SECTION("Leading separator produces empty first token") {
        auto parts = StringUtils::split("/a/b", '/');
        REQUIRE(parts[0] == "");
        REQUIRE(parts[1] == "a");
    }
}

TEST_CASE("StringUtils: join", "[utils][string]")
{
    SECTION("Basic join with slash") {
        std::vector<String> tokens = {"a", "b", "c"};
        REQUIRE(StringUtils::join(tokens, '/') == "a/b/c");
    }

    SECTION("Single element") {
        std::vector<String> tokens = {"alone"};
        REQUIRE(StringUtils::join(tokens, '/') == "alone");
    }

    SECTION("Empty vector produces empty string") {
        std::vector<String> tokens;
        REQUIRE(StringUtils::join(tokens, '/') == "");
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// Paths
// ─────────────────────────────────────────────────────────────────────────────
TEST_CASE("Paths: add", "[utils][paths]")
{
    SECTION("Joins prefix and suffix with slash") {
        REQUIRE(Paths::add("folder", "file.txt") == "folder/file.txt");
    }

    SECTION("Absolute suffix (starting with /) overrides prefix") {
        REQUIRE(Paths::add("folder", "/file.txt") == "/file.txt");
    }

    SECTION("Absolute suffix (starting with ~/) overrides prefix") {
        REQUIRE(Paths::add("folder", "~/file.txt") == "/file.txt");
    }

    SECTION("Empty prefix returns suffix") {
        REQUIRE(Paths::add("", "file.txt") == "file.txt");
    }

    SECTION("Empty suffix returns prefix") {
        REQUIRE(Paths::add("folder", "") == "folder");
    }

    SECTION("Resolves double dots") {
        REQUIRE(Paths::add("a/b", "../c.txt") == "a/c.txt");
    }
}

TEST_CASE("Paths: normalized", "[utils][paths]")
{
    SECTION("Removes single dots") {
        REQUIRE(Paths::normalized("a/./b") == "a/b");
    }

    SECTION("Resolves double dots") {
        REQUIRE(Paths::normalized("a/b/../c") == "a/c");
    }

    SECTION("Preserves absolute path leading slash") {
        REQUIRE(Paths::normalized("/a/b/c") == "/a/b/c");
    }

    SECTION("Handles ~ as root replacement") {
        REQUIRE(Paths::normalized("~/file.txt") == "/file.txt");
    }

    SECTION("Double dot past root throws") {
        REQUIRE_THROWS_AS(Paths::normalized("a/../.."), std::invalid_argument);
    }
}

TEST_CASE("Paths: getDirname", "[utils][paths]")
{
    SECTION("Returns folder part for a real file") {
        String actual = Paths::getDirname("/tmp/workspace/leandrolillo/playgroundDemos/test/tests.cpp");
        REQUIRE(actual == "/tmp/workspace/leandrolillo/playgroundDemos/test");
    }

    SECTION("Returns path unchanged if it is a directory") {
        String actual = Paths::getDirname("/tmp");
        REQUIRE(actual == "/tmp");
    }
}

TEST_CASE("Paths: getBasename", "[utils][paths]")
{
    SECTION("Returns filename from a path") {
        REQUIRE(Paths::getBasename("/some/folder/file.txt") == "file.txt");
    }

    SECTION("Returns full string when no slash present") {
        REQUIRE(Paths::getBasename("file.txt") == "file.txt");
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// BreakoutLevel
// ─────────────────────────────────────────────────────────────────────────────
TEST_CASE("BreakoutLevel: construction and dimensions", "[breakout]")
{
    SECTION("Stores correct row and column counts") {
        BreakoutLevel level(3, 5);
        REQUIRE(level.getRows() == 3u);
        REQUIRE(level.getColumns() == 5u);
    }
}

TEST_CASE("BreakoutLevel: brick data access", "[breakout]")
{
    BreakoutLevel level(4, 6);

    SECTION("All bricks are initially zero") {
        for (unsigned int i = 0; i < level.getRows(); ++i) {
            for (unsigned int j = 0; j < level.getColumns(); ++j) {
                REQUIRE(level.getBrickAt(i, j) == 0u);
            }
        }
    }

    SECTION("setBrick stores the value and getBrickAt retrieves it") {
        level.setBrick(0, 0, 3);
        REQUIRE(level.getBrickAt(0, 0) == 3u);
    }

    SECTION("Values at other cells are unaffected after set") {
        level.setBrick(1, 2, 5);
        REQUIRE(level.getBrickAt(0, 0) == 0u);
        REQUIRE(level.getBrickAt(1, 2) == 5u);
        REQUIRE(level.getBrickAt(1, 3) == 0u);
    }

    SECTION("Last cell can be written and read") {
        unsigned int lastRow = level.getRows() - 1;
        unsigned int lastCol = level.getColumns() - 1;
        level.setBrick(lastRow, lastCol, 1);
        REQUIRE(level.getBrickAt(lastRow, lastCol) == 1u);
    }

    SECTION("Multiple cells can hold different values") {
        level.setBrick(0, 0, 1);
        level.setBrick(0, 1, 2);
        level.setBrick(2, 3, 9);
        REQUIRE(level.getBrickAt(0, 0) == 1u);
        REQUIRE(level.getBrickAt(0, 1) == 2u);
        REQUIRE(level.getBrickAt(2, 3) == 9u);
    }

    SECTION("Out-of-bounds access throws") {
        REQUIRE_THROWS_AS(level.getBrickAt(level.getRows(), 0), std::out_of_range);
        REQUIRE_THROWS_AS(level.getBrickAt(0, level.getColumns()), std::out_of_range);
    }
}

