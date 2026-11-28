#pragma once

#include <cmath>
#include <functional>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <glm/glm.hpp>

namespace test {

struct TestCase {
    std::string name;
    std::function<void()> fn;
};

struct Skipped {
    std::string reason;
};

inline std::vector<TestCase>& registry() {
    static std::vector<TestCase> tests;
    return tests;
}

inline std::vector<std::string>& failures() {
    static std::vector<std::string> list;
    return list;
}

struct Registrar {
    Registrar(const char* name, std::function<void()> fn) { registry().push_back({name, fn}); }
};

inline void fail(const char* file, int line, const std::string& message) {
    std::ostringstream out;
    out << file << ":" << line << ": " << message;
    failures().push_back(out.str());
}

inline std::string str(const glm::vec3& v) {
    std::ostringstream out;
    out << "(" << v.x << ", " << v.y << ", " << v.z << ")";
    return out.str();
}

template <typename T>
std::string str(const T& value) {
    std::ostringstream out;
    out << value;
    return out.str();
}

}

#define TEST_CONCAT_INNER(a, b) a##b
#define TEST_CONCAT(a, b) TEST_CONCAT_INNER(a, b)

#define TEST(name)                                                                  \
    static void TEST_CONCAT(test_fn_, __LINE__)();                                  \
    static test::Registrar TEST_CONCAT(test_reg_, __LINE__)(name, TEST_CONCAT(test_fn_, __LINE__)); \
    static void TEST_CONCAT(test_fn_, __LINE__)()

#define CHECK(expr)                                                                 \
    do { if (!(expr)) test::fail(__FILE__, __LINE__, "CHECK(" #expr ")"); } while (0)

#define CHECK_EQ(a, b)                                                              \
    do {                                                                            \
        auto _a = (a); auto _b = (b);                                               \
        if (!(_a == _b))                                                            \
            test::fail(__FILE__, __LINE__, "CHECK_EQ(" #a ", " #b "): " + test::str(_a) + " != " + test::str(_b)); \
    } while (0)

#define CHECK_NEAR(a, b, eps)                                                       \
    do {                                                                            \
        double _a = (a); double _b = (b);                                           \
        if (!(std::fabs(_a - _b) <= (eps)))                                         \
            test::fail(__FILE__, __LINE__, "CHECK_NEAR(" #a ", " #b "): " + test::str(_a) + " vs " + test::str(_b)); \
    } while (0)

#define CHECK_VEC_NEAR(a, b, eps)                                                   \
    do {                                                                            \
        glm::vec3 _a = (a); glm::vec3 _b = (b);                                     \
        if (!(glm::length(_a - _b) <= (eps)))                                       \
            test::fail(__FILE__, __LINE__, "CHECK_VEC_NEAR(" #a ", " #b "): " + test::str(_a) + " vs " + test::str(_b)); \
    } while (0)

#define SKIP(reason) throw test::Skipped{reason}
