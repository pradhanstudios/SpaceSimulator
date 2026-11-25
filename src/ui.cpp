#include "ui.hpp"
#include <cstdio>
#include <ft2build.h>
#include FT_FREETYPE_H

// Layout (pixels)
constexpr float UI_MARGIN = 20.f;
constexpr float UI_BUTTON_WIDTH = 120.f;
constexpr float UI_BUTTON_HEIGHT = 40.f;
constexpr float UI_SPACING = 10.f;
constexpr float UI_LABEL_HEIGHT = 30.f;
constexpr int UI_FONT_SIZE = 20;

const glm::vec4 UI_BUTTON_COLOR = {0.20f, 0.20f, 0.25f, 0.85f};
const glm::vec4 UI_BUTTON_HOVER_COLOR = {0.35f, 0.35f, 0.45f, 0.95f};
const glm::vec4 UI_TEXT_COLOR = {1.f, 1.f, 1.f, 1.f};

UI::UI(GLFWwindow* window)
    : m_window(window), m_shader(nullptr), m_vao(0), m_vbo(0), m_glyphs{}, m_fontLoaded(false) {
    m_shader = new Shader(uiVertexShaderPath, uiFragmentShaderPath);

    // One dynamic quad (6 verts * vec4) reused for every rect and glyph
    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);
    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, nullptr, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    loadFont(uiFontPath, UI_FONT_SIZE);
    std::cout << "UI initialized." << std::endl;
}

UI::~UI() {
    for (Glyph& g : m_glyphs)
        if (g.texture) glDeleteTextures(1, &g.texture);
    glDeleteBuffers(1, &m_vbo);
    glDeleteVertexArrays(1, &m_vao);
    delete m_shader;
}

void UI::loadFont(const char* path, int pixelSize) {
    FT_Library ft;
    if (FT_Init_FreeType(&ft)) {
        std::cerr << "ERROR: Could not init FreeType" << std::endl;
        return;
    }

    FT_Face face;
    if (FT_New_Face(ft, path, 0, &face)) {
        std::cerr << "ERROR: Failed to load font '" << path << "'" << std::endl;
        FT_Done_FreeType(ft);
        return;
    }
    FT_Set_Pixel_Sizes(face, 0, pixelSize);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // glyph bitmaps are tightly packed single bytes
    for (unsigned char c = 32; c < 128; c++) {
        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
            continue;

        FT_GlyphSlot slot = face->glyph;
        Glyph& g = m_glyphs[c];
        g.width = slot->bitmap.width;
        g.height = slot->bitmap.rows;
        g.bearingX = slot->bitmap_left;
        g.bearingY = slot->bitmap_top;
        g.advance = slot->advance.x >> 6;

        if (g.width == 0 || g.height == 0)
            continue; // e.g. space: advance only, no texture

        glGenTextures(1, &g.texture);
        glBindTexture(GL_TEXTURE_2D, g.texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, g.width, g.height, 0, GL_RED, GL_UNSIGNED_BYTE, slot->bitmap.buffer);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
    glBindTexture(GL_TEXTURE_2D, 0);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

    FT_Done_Face(face);
    FT_Done_FreeType(ft);
    m_fontLoaded = true;
}

// Layout: [speed label] above [10x] above [0.1x], anchored top-right
float UI::columnX() const {
    int w, h;
    glfwGetWindowSize(m_window, &w, &h);
    return w - UI_MARGIN - UI_BUTTON_WIDTH;
}

Rect UI::speedUpRect() const {
    return {columnX(), UI_MARGIN + UI_LABEL_HEIGHT + UI_SPACING, UI_BUTTON_WIDTH, UI_BUTTON_HEIGHT};
}

Rect UI::slowDownRect() const {
    Rect up = speedUpRect();
    return {up.x, up.y + up.h + UI_SPACING, UI_BUTTON_WIDTH, UI_BUTTON_HEIGHT};
}

UIAction UI::hitTest(double x, double y) const {
    if (speedUpRect().contains(x, y)) return UIAction::SpeedUp;
    if (slowDownRect().contains(x, y)) return UIAction::SlowDown;
    return UIAction::None;
}

void UI::render(float simSpeed, bool cursorVisible) {
    int w, h;
    glfwGetWindowSize(m_window, &w, &h);

    double mx = -1, my = -1;
    if (cursorVisible)
        glfwGetCursorPos(m_window, &mx, &my);

    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    m_shader->enable();
    // y-down ortho so UI coords match cursor coords
    glm::mat4 projection = glm::ortho(0.f, float(w), float(h), 0.f);
    m_shader->setm4("projection", glm::value_ptr(projection));
    m_shader->set("glyph", 0);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(m_vao);

    Rect up = speedUpRect();
    Rect down = slowDownRect();
    Rect label = {up.x, UI_MARGIN, UI_BUTTON_WIDTH, UI_LABEL_HEIGHT};

    drawRect(up, up.contains(mx, my) ? UI_BUTTON_HOVER_COLOR : UI_BUTTON_COLOR);
    drawRect(down, down.contains(mx, my) ? UI_BUTTON_HOVER_COLOR : UI_BUTTON_COLOR);

    if (m_fontLoaded) {
        char buf[32];
        std::snprintf(buf, sizeof(buf), "Speed: %gx", simSpeed);
        drawTextCentered(buf, label, UI_TEXT_COLOR);
        drawTextCentered("10x", up, UI_TEXT_COLOR);
        drawTextCentered("0.1x", down, UI_TEXT_COLOR);
    }

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    m_shader->disable();

    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
}

void UI::drawQuad(float x0, float y0, float x1, float y1, float u0, float v0, float u1, float v1) {
    float verts[6][4] = {
        {x0, y0, u0, v0}, {x0, y1, u0, v1}, {x1, y1, u1, v1},
        {x0, y0, u0, v0}, {x1, y1, u1, v1}, {x1, y0, u1, v0},
    };
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(verts), verts);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void UI::drawRect(const Rect& r, const glm::vec4& color) {
    m_shader->set("useTexture", 0);
    m_shader->set("color", color.r, color.g, color.b, color.a);
    drawQuad(r.x, r.y, r.x + r.w, r.y + r.h, 0.f, 0.f, 0.f, 0.f);
}

void UI::drawText(const std::string& text, float x, float baselineY, const glm::vec4& color) {
    m_shader->set("useTexture", 1);
    m_shader->set("color", color.r, color.g, color.b, color.a);
    for (char c : text) {
        if (c < 0) continue; // non-ASCII
        const Glyph& g = m_glyphs[int(c)];
        if (g.texture) {
            float x0 = x + g.bearingX;
            float y0 = baselineY - g.bearingY;
            glBindTexture(GL_TEXTURE_2D, g.texture);
            // FreeType rows go top-to-bottom, so v=0 is the glyph's top in y-down space
            drawQuad(x0, y0, x0 + g.width, y0 + g.height, 0.f, 0.f, 1.f, 1.f);
        }
        x += g.advance;
    }
}

void UI::drawTextCentered(const std::string& text, const Rect& r, const glm::vec4& color) {
    float x = r.x + (r.w - textWidth(text)) / 2.f;
    // Approximate vertical centring: cap height ~0.7 of the font pixel size
    float baseline = r.y + (r.h + UI_FONT_SIZE * 0.7f) / 2.f;
    drawText(text, x, baseline, color);
}

float UI::textWidth(const std::string& text) const {
    float w = 0.f;
    for (char c : text)
        if (c >= 0) w += m_glyphs[int(c)].advance;
    return w;
}
