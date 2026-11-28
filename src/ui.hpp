#pragma once

#include "globals.hpp"
#include "shader.hpp"
#include <string>

enum class UIAction { None, SpeedUp, SlowDown };

struct Rect {
    float x, y, w, h;
    bool contains(double px, double py) const {
        return px >= x && px <= x + w && py >= y && py <= y + h;
    }
};

Rect speedUpRectFor(int windowWidth);
Rect slowDownRectFor(int windowWidth);
UIAction hitTestFor(int windowWidth, double x, double y);

// Screen-space overlay: simulation speed readout + 10x / 0.1x buttons (top-right corner).
// Coordinates are window pixels with the origin at the top-left, matching glfwGetCursorPos.
class UI {
public:
    UI(GLFWwindow* window);
    ~UI();

    // Draw the overlay. Hover highlighting only happens while the cursor is visible.
    void render(float simSpeed, bool cursorVisible);

    // Which button (if any) is at the given cursor position.
    UIAction hitTest(double x, double y) const;

    inline bool isFontLoaded() const { return m_fontLoaded; }

private:
    struct Glyph {
        GLuint texture;
        int width, height;
        int bearingX, bearingY;
        int advance; // pixels
    };

    GLFWwindow* m_window;
    Shader* m_shader;
    GLuint m_vao, m_vbo;
    Glyph m_glyphs[128];
    bool m_fontLoaded;

    void loadFont(const char* path, int pixelSize);

    int windowWidth() const;

    void drawQuad(float x0, float y0, float x1, float y1, float u0, float v0, float u1, float v1);
    void drawRect(const Rect& r, const glm::vec4& color);
    void drawText(const std::string& text, float x, float baselineY, const glm::vec4& color);
    void drawTextCentered(const std::string& text, const Rect& r, const glm::vec4& color);
    float textWidth(const std::string& text) const;
};
