# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Build & run

There is no build system beyond the Makefile. It compiles every `src/*.cpp` in one g++ call into a single `./SpaceSimulator` binary. The binary is git-ignored. `.gitignore` ignores CMake-generated `Makefile`s but has a `!/Makefile` exception, so the root Makefile is tracked.

- `make` — plain build, no warnings
- `make warnings` — `-Wall -Wextra -Werror`
- `make debug` — `-g` with the warning flags
- `make test` — build `./SpaceSimulatorTests` with the warning flags and run it (see Testing)
- `./SpaceSimulator` — run. Run it from the repo root, because the shader and font paths in `globals.hpp` (`assets/...`) are relative to the working directory.

Dependencies: GLEW, GLFW, OpenGL 3.3, GLM (`/usr/include/glm`), FreeType (`/usr/include/freetype2`).

There is no linter.

## Testing

`make test` compiles every `src/*.cpp` except `main.cpp`, plus every `tests/*.cpp`, into `./SpaceSimulatorTests` (git-ignored) and runs it. Run it from the repo root, because the GL tests load `assets/...`. The runner prints PASS / FAIL / SKIP per test with `file:line` and values for each failed check, then a summary. It exits non-zero if anything failed. `./SpaceSimulatorTests <substring>` runs only tests whose name contains the substring, e.g. `./SpaceSimulatorTests camera:` (include the colon: `gl` alone also matches "triangle"). Production `std::cout` output is muted while tests run, but `std::cerr` still shows.

The framework is home-grown, in `tests/test.hpp`:
- `TEST("area: what it checks") { ... }` registers itself automatically, so a new file in `tests/` needs no other wiring.
- `CHECK(expr)`, `CHECK_EQ(a, b)`, `CHECK_NEAR(a, b, eps)`, `CHECK_VEC_NEAR(v1, v2, eps)` record a failure and keep going.
- `SKIP("reason")` ends the test as skipped.

One file per area: `test_physics`, `test_body`, `test_simulation` (multi-body behaviour and conservation laws), `test_camera`, `test_sphere` (mesh geometry), `test_ui_layout` and `test_gl`. All of them except `test_gl` are headless. They test pure functions, and bodies are built with `withMesh = false`. `test_gl` opens one shared hidden GLFW window. It covers shader compile/link, mesh upload, sphere regeneration, a renderer draw and UI construction/render, and checks `glGetError()` after each. Each GL test calls `requireGL()` first, which SKIPs when no display is available.

Keep logic testable without a GL context: put it in a free function (as with `stepPhysics`, `substepsFor`, `clampSpeedExponent`, `generateSphereVertices`, `speedUpRectFor`/`slowDownRectFor`/`hitTestFor`) and have the class wrapper call it. New or changed behaviour gets tests in the same branch. Put them in the matching `tests/test_<area>.cpp`, or in a new file for a new area. A known bug gets a test asserting the correct behaviour, even if it fails until the bug is fixed.

## Architecture

A C++17 / OpenGL N-body gravity simulator. Spheres attract each other, the user flies a free camera around them, and an on-screen overlay changes the simulation speed. The physics is basic and meant to be extended.

**Sim** (`sim.cpp/hpp`) owns every component through raw pointers that it allocates in `init()` and frees in the destructor. Destruction order matters: anything holding GL objects (`UI`, shaders, meshes) must be deleted before `Window`, which owns the GL context. Frame loop (`run()`): `processInput` → `update` → `render` → swap/poll → `updateDeltaTime()`, then a sleep that caps the frame rate at `m_fps`.

**Timing** (`deltaTime.hpp`) is a set of `inline` globals: `deltaTime` (real frame time, used by the camera and mouse) and `physicsDeltaTime` (the physics step size). `Body::updateVel`/`updatePos` read `physicsDeltaTime` directly instead of taking it as a parameter.

**Simulation speed** is `10^m_speedExponent`, clamped by `clampSpeedExponent` to `[minSpeedExponent, maxSpeedExponent]` (`globals.hpp`, currently 0.01x–1000x). `Sim::update` speeds up by running more physics steps, not bigger ones: `substeps = substepsFor(speed)` (`ceil(speed)`, at least 1) and `physicsDeltaTime = deltaTime * speed / substeps`, so no step is longer than one real frame. This keeps close encounters from blowing up at high speeds.

**Physics** (`stepPhysics(bodies)` in `physics.cpp`, `body.cpp`, `physics.hpp`): each step zeroes all forces, then runs a pairwise O(n²) gravity pass (`Body::calcGravForceVec` → `calcGravityForceVectors`, which returns zero force below a distance of 0.0001). It then integrates each body with semi-implicit Euler: `updateAcc` (F/m) → `updateVel` → `updatePos`. `G` is `1.f` in `globals.hpp` for testing, and the real constant is commented out. The force and speed helpers are inline in `physics.hpp`. `Body` also has an unused `m_momentum`.

**Bodies** are hard-coded in `Sim::init` (`m_bodyList`, marked `// temp`). Ownership chain: `Body` → `Sphere` → `Mesh` (VAO/VBO/EBO). `Body`'s last constructor argument `withMesh` (default true) can skip the sphere, for headless use. The body then keeps only `m_radius`, and `getMesh()` returns `nullptr`. `Sphere::setRadius` regenerates the whole mesh from `generateSphereVertices(r)`. The mesh is a set of `SPHERE_RESOLUTION` latitude bands, each a triangle strip of `2 * (SPHERE_RESOLUTION + 1)` vertices. The pole axis is z. Each vertex has 11 floats: position, normal, UV, tangent. `Renderer::draw` issues one `glDrawArrays(GL_TRIANGLE_STRIP, ...)` per band, so the generator in `sphere.cpp` and the draw loop must stay in sync.

**Rendering** (`renderer.cpp`, `shader.cpp`, `assets/shader.*`): there is no model matrix. Every sphere mesh is built at the origin, and `Camera::getViewForPosition(bodyPos)` puts the body's position into the view matrix as an eye offset. The vertex shader computes `projection * view * pos`, and the fragment shader is a flat colour with no lighting. `Renderer::toggleWireframeDraw` exists but is not bound to a key.

**Camera** (`camera.cpp/hpp`): yaw/pitch free-look with pitch clamped to ±89°. The projection is fixed at construction from `defaultWidth`/`defaultHeight`, so it does not follow window resizes. `updateView()` runs every frame in `Sim::update`.

**Input** (`Sim::processInput`, `Sim::mouseCallback`): the cursor starts captured. Mouse-look goes through the static `mouseCallback`, which finds the `Sim` via `glfwGetWindowUserPointer`. Releasing Tab calls `tab_out()`, which frees the cursor and swaps in a no-op cursor callback, or recaptures it and sets `m_firstMouseMove` so the view doesn't jump. Keys: WASD move along the camera's front and right axes, Space/Left Ctrl move along world Y, and Esc quits. Left clicks reach the UI only while the cursor is free.

**UI** (`ui.cpp/hpp`, `assets/ui.*`): a screen-space overlay in the top-right corner showing the speed readout and 10x / 0.1x buttons. It has its own shader and renders text from FreeType glyph textures (ASCII 0–127, `DejaVuSans.ttf`). Coordinates are window pixels with a top-left origin, matching `glfwGetCursorPos`. Button rects come from the free functions `speedUpRectFor`/`slowDownRectFor(windowWidth)`. They are recomputed from the live window size each call. `UI::hitTest` wraps `hitTestFor(width, x, y)` and returns a `UIAction`, which `Sim::processInput` acts on. To add a button: add a `UIAction` value, a `...RectFor` function, drawing in `UI::render`, a case in `hitTestFor`, handling in `processInput`, and tests in `test_ui_layout.cpp`.

**globals.hpp** holds the shared tunables (window size, FPS, `SPHERE_RESOLUTION`, camera defaults, asset paths, `G`, speed limits) and the GL/GLFW/GLM/std includes. Every header pulls it in directly or indirectly.

## Coding conventions

- Keep comments to a minimum. Code should explain itself through clear function and variable names.
- The one exception: a block that has to be hard to read for efficiency (e.g. vertex-buffer index arithmetic, math tricks) gets a single one-line comment saying what it does.

## Workflow

Work happens on issue-numbered branches (e.g. `9-add-physics-to-body-class`, `13-simulation-speed-slider`) merged into `main` via GitHub PRs.

`ROADMAP.md` lists the planned work as phased checkboxes, from the core engine through to black holes. When a branch finishes a roadmap item, tick it (`- [ ]` → `- [x]`) in the same branch. When work on an item starts, add its issue number next to it, e.g. `(#12)`. New planned work goes into the matching phase.

Only commit when the user asks to. Before committing, build with `make warnings` and confirm it compiles with no warnings (they are errors). Run `make test` and confirm every test passes, or that the only failures are known bugs the user has been told about. Then run `./SpaceSimulator` to check it starts and the simulation behaves as expected.

Claude may edit this CLAUDE.md file as it sees fit, but only with the user's permission.

Commits and pull requests:

- Do not add a `Co-Authored-By: Claude` line, a "Generated with Claude Code" footer, or any other Claude/AI attribution to commits or PR descriptions. The user is the sole author.
- Commit each file separately (a matching `.cpp`/`.hpp` pair may share a commit), each with a brief message describing what changed in that file, e.g. `Add step physics for different speeds`, `Add buttons`.
