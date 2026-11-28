#include "test.hpp"
#include "ui.hpp"

namespace {

const int WIDTHS[] = {400, 800, 1280, 1920, 3840};

double centreX(const Rect& r) { return r.x + r.w / 2.0; }
double centreY(const Rect& r) { return r.y + r.h / 2.0; }

}

TEST("ui: buttons are anchored a fixed distance from the right edge") {
    float margin = 1280 - (speedUpRectFor(1280).x + speedUpRectFor(1280).w);
    CHECK(margin > 0.f);
    for (int w : WIDTHS) {
        Rect up = speedUpRectFor(w);
        CHECK_NEAR(w - (up.x + up.w), margin, 1e-4);
        CHECK(up.x >= 0.f);
        CHECK(up.y > 0.f);
    }
}

TEST("ui: buttons do not depend on width except for x") {
    Rect a = speedUpRectFor(800), b = speedUpRectFor(1920);
    CHECK_EQ(a.y, b.y);
    CHECK_EQ(a.w, b.w);
    CHECK_EQ(a.h, b.h);
}

TEST("ui: slow-down sits directly below speed-up with a gap") {
    for (int w : WIDTHS) {
        Rect up = speedUpRectFor(w), down = slowDownRectFor(w);
        CHECK_EQ(up.x, down.x);
        CHECK_EQ(up.w, down.w);
        CHECK_EQ(up.h, down.h);
        CHECK(down.y > up.y + up.h);
    }
}

TEST("ui: rect contains is inclusive of its edges") {
    Rect r{10, 20, 30, 40};
    CHECK(r.contains(10, 20));
    CHECK(r.contains(40, 60));
    CHECK(r.contains(25, 40));
    CHECK(!r.contains(9.99, 30));
    CHECK(!r.contains(40.01, 30));
    CHECK(!r.contains(20, 19.99));
    CHECK(!r.contains(20, 60.01));
}

TEST("ui: hit test maps button centres to their actions") {
    for (int w : WIDTHS) {
        Rect up = speedUpRectFor(w), down = slowDownRectFor(w);
        CHECK(hitTestFor(w, centreX(up), centreY(up)) == UIAction::SpeedUp);
        CHECK(hitTestFor(w, centreX(down), centreY(down)) == UIAction::SlowDown);
    }
}

TEST("ui: hit test misses the gap, the label and the rest of the window") {
    int w = 1280;
    Rect up = speedUpRectFor(w), down = slowDownRectFor(w);
    double gapY = (up.y + up.h + down.y) / 2.0;
    CHECK(hitTestFor(w, centreX(up), gapY) == UIAction::None);
    CHECK(hitTestFor(w, centreX(up), up.y / 2.0) == UIAction::None);
    CHECK(hitTestFor(w, up.x - 1.0, centreY(up)) == UIAction::None);
    CHECK(hitTestFor(w, 0.0, 0.0) == UIAction::None);
    CHECK(hitTestFor(w, w / 2.0, 360.0) == UIAction::None);
    CHECK(hitTestFor(w, -1.0, -1.0) == UIAction::None);
}
