# Roadmap

The long-term goal is a complete space simulation: everything from planets and moons up to neutron stars and black holes, simulated with believable physics and rendered convincingly.

Check an item off by changing `- [ ]` to `- [x]` in the PR that finishes it. When work on an item starts, open a GitHub issue for it and put the number next to it (e.g. `(#12)`). Every item ships with tests (see the Testing section in CLAUDE.md).

Phases are roughly in dependency order. Items within a phase can usually be done in any order.

---

## Phase 0: Foundations (done)

- [x] Window and OpenGL context (#1)
- [x] Boilerplate and Makefile (#3)
- [x] Body class (#2)
- [x] Sphere mesh (#5)
- [x] Free-fly camera (#6)
- [x] Newtonian gravity and integration (#9)
- [x] Simulation speed controls with substepping (#13)
- [x] Test system (`make test`, headless + GL tests)
- [x] CLAUDE.md, clangd setup

## Phase 1: Core engine cleanup

Groundwork that later phases depend on.

- [ ] Model matrix per body (translation, rotation, scale) instead of baking the position into the view matrix
- [ ] One shared sphere mesh, scaled per body by the model matrix, instead of one mesh per body
- [ ] Projection and `glViewport` follow window resizes
- [ ] Fixed physics timestep with an accumulator, so results don't depend on frame rate
- [ ] Symplectic integrator (velocity Verlet / leapfrog) to replace semi-implicit Euler, with energy-drift tests
- [ ] Unit system (e.g. AU, solar masses, days) and the real `G`
- [ ] Body properties: name, colour, body type
- [ ] Load scenes from a file instead of hard-coding bodies in `Sim::init`
- [ ] Basic lighting: diffuse + specular shading, with stars as light sources

## Phase 2: Collisions (#12)

- [ ] Sphere–sphere collision detection
- [ ] Elastic collision response (conserves momentum and kinetic energy)
- [ ] Inelastic collisions with a coefficient of restitution
- [ ] Merging: combine mass, momentum and volume into one body
- [ ] Per-body or per-scene choice of collision mode
- [ ] Continuous collision detection so fast bodies can't tunnel through each other
- [ ] Broad phase (uniform grid or sweep-and-prune) so collisions scale past O(n²)

## Phase 3: Rotation physics (#10)

Needs the Phase 1 model matrix.

- [ ] Orientation (quaternion) and angular velocity on `Body`
- [ ] Moment of inertia (solid sphere `2/5 m r²`, extendable to other shapes)
- [ ] Integrate orientation each step; torque → angular acceleration
- [ ] Render rotation through the model matrix
- [ ] Planet textures, so spin is visible (UVs and tangents already exist)
- [ ] Axial tilt
- [ ] Collisions transfer angular momentum (friction at the contact point)
- [ ] Angular momentum conservation tests

## Phase 4: Interaction and tools

- [ ] Pause and single-step
- [ ] Click a body to select it (ray picking)
- [ ] Info panel for the selected body (mass, radius, velocity, orbital period, …)
- [ ] Camera modes: follow / orbit around the selected body
- [ ] Add, delete and edit bodies at runtime
- [ ] Orbit trails
- [ ] Predicted orbit paths
- [ ] Readouts for total energy and momentum (debug overlay)
- [ ] Save and load simulation state
- [ ] Simulated time / date display
- [ ] Bind wireframe toggle and other debug views to keys

## Phase 5: Scale and performance

Space is huge and full of things.

- [ ] Double-precision positions (or a floating origin) to avoid float jitter at solar-system scale
- [ ] Logarithmic depth buffer for huge view distances
- [ ] Toggle between true scale and visually enlarged bodies
- [ ] Barnes–Hut tree for O(n log n) gravity
- [ ] Multithreaded physics
- [ ] Instanced rendering for many bodies
- [ ] Sphere level of detail by screen size
- [ ] Particle systems: asteroid belts, planetary rings, debris from collisions
- [ ] GPU compute for gravity (stretch)

## Phase 6: Realistic solar systems

- [ ] Orbital elements ↔ state vectors (Kepler) helpers
- [ ] Real solar system scene (planets, major moons) from published data
- [ ] Validation tests: orbital periods match reality
- [ ] Star rendering: blackbody colour from temperature, emissive surface
- [ ] HDR and bloom
- [ ] Starfield / skybox background
- [ ] Atmospheres (scattering)
- [ ] Planetary rings
- [ ] Tidal forces
- [ ] Roche limit breakup (bodies torn apart into particles)
- [ ] Tidal locking (uses Phase 3 rotation)

## Phase 7: Neutron stars

- [ ] Neutron star body type (extreme density, ~10 km radius)
- [ ] Pulsars: rapid rotation with a tilted magnetic axis and rendered beams
- [ ] Accretion from a companion star
- [ ] Surface gravity and escape velocity in the info panel

## Phase 8: Black holes

- [ ] Black hole body type with Schwarzschild radius
- [ ] Event horizon: anything crossing it is absorbed (mass and momentum merge)
- [ ] Post-Newtonian gravity corrections (1PN)
- [ ] Validation test: Mercury's perihelion precession (~43″/century)
- [ ] Gravitational lensing, screen-space approximation
- [ ] Gravitational lensing, ray-marched null geodesics in the shader (Schwarzschild)
- [ ] Photon sphere and ISCO visualisation
- [ ] Accretion disk rendering
- [ ] Doppler beaming and gravitational redshift on the disk
- [ ] Time dilation display near massive bodies
- [ ] Rotating (Kerr) black holes
- [ ] Gravitational-wave energy loss in close binaries (inspiral)
- [ ] Black hole and neutron star mergers

## Phase 9: Beyond (stretch)

- [ ] Stellar evolution (main sequence → giant → white dwarf / neutron star / black hole)
- [ ] Supernovae
- [ ] Star clusters and galaxies
- [ ] Dark matter halos
- [ ] Nebulae and volumetric rendering
