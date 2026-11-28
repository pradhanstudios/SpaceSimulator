#include "test.hpp"
#include "body.hpp"
#include "camera.hpp"
#include "mesh.hpp"
#include "renderer.hpp"
#include "shader.hpp"
#include "sphere.hpp"
#include "ui.hpp"

namespace {

struct GLContext {
    GLFWwindow* window = nullptr;
    std::string error;

    GLContext() {
        glfwSetErrorCallback([](int, const char*) {});
        if (!glfwInit()) { error = "glfwInit failed (no display?)"; return; }
        glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
        glfwWindowHint(GLFW_DEPTH_BITS, 24);
        window = glfwCreateWindow(defaultWidth, defaultHeight, "SpaceSimulatorTests", nullptr, nullptr);
        if (!window) { error = "could not create hidden window"; glfwTerminate(); return; }
        glfwMakeContextCurrent(window);
        if (glewInit() != GLEW_OK) { error = "glewInit failed"; glfwDestroyWindow(window); glfwTerminate(); window = nullptr; return; }
        while (glGetError() != GL_NO_ERROR) {}
    }

    ~GLContext() {
        if (window) {
            glfwDestroyWindow(window);
            glfwTerminate();
        }
    }
};

GLFWwindow* requireGL() {
    static GLContext context;
    if (!context.window) SKIP(context.error);
    return context.window;
}

void checkNoGLError(const char* file, int line) {
    GLenum err = glGetError();
    if (err != GL_NO_ERROR) test::fail(file, line, "glGetError() = " + test::str(err));
    while (glGetError() != GL_NO_ERROR) {}
}

}

#define CHECK_NO_GL_ERROR() checkNoGLError(__FILE__, __LINE__)

TEST("gl: body shader compiles, links and exposes its uniforms") {
    requireGL();
    Shader shader(vertexShaderPath, fragmentShaderPath);
    CHECK(shader.getID() != 0);
    CHECK(shader.getLocation("projection") != -1);
    CHECK(shader.getLocation("view") != -1);
    CHECK_NO_GL_ERROR();
}

TEST("gl: ui shader compiles, links and exposes its uniforms") {
    requireGL();
    Shader shader(uiVertexShaderPath, uiFragmentShaderPath);
    CHECK(shader.getID() != 0);
    CHECK(shader.getLocation("projection") != -1);
    CHECK(shader.getLocation("color") != -1);
    CHECK(shader.getLocation("useTexture") != -1);
    CHECK(shader.getLocation("glyph") != -1);
    CHECK_NO_GL_ERROR();
}

TEST("gl: missing shader files give program id 0") {
    requireGL();
    Shader shader("assets/does_not_exist.vert", "assets/does_not_exist.frag");
    CHECK_EQ(shader.getID(), GLuint(0));
}

TEST("gl: mesh uploads sphere vertices into a VAO and VBO") {
    requireGL();
    std::vector<float> verts = generateSphereVertices(1.f);
    Mesh mesh(verts.data(), verts.size());
    CHECK(mesh.getVAOID() != 0);
    CHECK(mesh.getVBOID() != 0);
    CHECK_EQ(mesh.getVertexCount(), verts.size() / 11);
    CHECK_EQ(mesh.getIndexCount(), size_t(0));

    GLint bufferSize = 0;
    glBindBuffer(GL_ARRAY_BUFFER, mesh.getVBOID());
    glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &bufferSize);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    CHECK_EQ(size_t(bufferSize), verts.size() * sizeof(float));
    CHECK_NO_GL_ERROR();
}

TEST("gl: sphere regenerates its mesh when the radius changes") {
    requireGL();
    Sphere sphere(1.f);
    size_t count = sphere.getMesh()->getVertexCount();
    sphere.setRadius(2.5f);
    CHECK_EQ(sphere.getRadius(), 2.5f);
    CHECK(sphere.getMesh() != nullptr);
    CHECK(sphere.getMesh()->getVAOID() != 0);
    CHECK_EQ(sphere.getMesh()->getVertexCount(), count);
    CHECK_NO_GL_ERROR();
}

TEST("gl: body with a mesh constructs, resizes and destroys cleanly") {
    requireGL();
    {
        Body body(1.f, glm::vec3(1, 2, 3), glm::vec3(0.f), 5.f);
        CHECK(body.getMesh() != nullptr);
        body.setRadius(3.f);
        CHECK_EQ(body.getRadius(), 3.f);
        CHECK(body.getMesh() != nullptr);
    }
    CHECK_NO_GL_ERROR();
}

TEST("gl: renderer draws a body without GL errors") {
    requireGL();
    Renderer renderer;
    Shader shader(vertexShaderPath, fragmentShaderPath);
    Camera camera(glm::vec3(0, 0, 10));
    Body body(1.f, glm::vec3(0.f), glm::vec3(0.f), 1.f);
    renderer.clear();
    renderer.draw(&body, &shader, &camera);
    renderer.toggleWireframeDraw();
    renderer.draw(&body, &shader, &camera);
    renderer.toggleWireframeDraw();
    CHECK_NO_GL_ERROR();
}

TEST("gl: ui loads its font and renders without GL errors") {
    GLFWwindow* window = requireGL();
    UI ui(window);
    CHECK(ui.isFontLoaded());
    ui.render(1.f, false);
    ui.render(1000.f, true);
    ui.render(0.01f, true);
    CHECK_NO_GL_ERROR();
}

TEST("gl: ui hit test uses the live window width") {
    GLFWwindow* window = requireGL();
    UI ui(window);
    int w, h;
    glfwGetWindowSize(window, &w, &h);
    Rect up = speedUpRectFor(w), down = slowDownRectFor(w);
    CHECK(ui.hitTest(up.x + up.w / 2, up.y + up.h / 2) == UIAction::SpeedUp);
    CHECK(ui.hitTest(down.x + down.w / 2, down.y + down.h / 2) == UIAction::SlowDown);
    CHECK(ui.hitTest(1, 1) == UIAction::None);
}

TEST("gl: default-constructed mesh owns no GL objects and destroys cleanly") {
    requireGL();
    {
        Mesh mesh;
        CHECK_EQ(mesh.getVAOID(), GLuint(0));
        CHECK_EQ(mesh.getVBOID(), GLuint(0));
        CHECK_EQ(mesh.getVertexCount(), size_t(0));
        CHECK_EQ(mesh.getIndexCount(), size_t(0));
    }
    CHECK_NO_GL_ERROR();
}

TEST("gl: mesh vertex attributes match the 11-float layout") {
    requireGL();
    std::vector<float> verts = generateSphereVertices(1.f);
    Mesh mesh(verts.data(), verts.size());
    mesh.bind();
    const GLint expectedSize[] = {3, 3, 2, 3};
    const GLint expectedOffset[] = {0, 3, 6, 8};
    for (GLuint attrib = 0; attrib < 4; attrib++) {
        GLint size = 0, stride = 0, enabled = 0;
        void* offset = nullptr;
        glGetVertexAttribiv(attrib, GL_VERTEX_ATTRIB_ARRAY_SIZE, &size);
        glGetVertexAttribiv(attrib, GL_VERTEX_ATTRIB_ARRAY_STRIDE, &stride);
        glGetVertexAttribiv(attrib, GL_VERTEX_ATTRIB_ARRAY_ENABLED, &enabled);
        glGetVertexAttribPointerv(attrib, GL_VERTEX_ATTRIB_ARRAY_POINTER, &offset);
        CHECK_EQ(size, expectedSize[attrib]);
        CHECK_EQ(stride, GLint(11 * sizeof(float)));
        CHECK_EQ(enabled, GLint(GL_TRUE));
        CHECK_EQ(reinterpret_cast<size_t>(offset), expectedOffset[attrib] * sizeof(float));
    }
    mesh.unbind();
    CHECK_NO_GL_ERROR();
}
