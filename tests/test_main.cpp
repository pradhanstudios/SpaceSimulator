#include "test.hpp"

int main(int argc, char** argv) {
    std::string filter = argc > 1 ? argv[1] : "";
    int passed = 0, failed = 0, skipped = 0;

    std::ostream out(std::cout.rdbuf());
    std::ostringstream silenced;
    std::cout.rdbuf(silenced.rdbuf());

    for (const test::TestCase& t : test::registry()) {
        if (!filter.empty() && t.name.find(filter) == std::string::npos) continue;

        test::failures().clear();
        silenced.str("");
        try {
            t.fn();
        } catch (const test::Skipped& s) {
            out << "SKIP  " << t.name << " (" << s.reason << ")\n";
            skipped++;
            continue;
        } catch (const std::exception& e) {
            test::failures().push_back(std::string("uncaught exception: ") + e.what());
        }

        if (test::failures().empty()) {
            out << "PASS  " << t.name << "\n";
            passed++;
        } else {
            out << "FAIL  " << t.name << "\n";
            for (const std::string& f : test::failures()) out << "        " << f << "\n";
            failed++;
        }
    }

    out << "\n" << passed << " passed, " << failed << " failed, " << skipped << " skipped\n";
    std::cout.rdbuf(out.rdbuf());
    return failed == 0 ? 0 : 1;
}
