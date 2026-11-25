#version 330 core
in vec2 uv;
out vec4 FragColor;

uniform sampler2D glyph;
uniform vec4 color;
uniform int useTexture; // 1 = text (alpha from glyph texture), 0 = solid rect

void main() {
    float alpha = (useTexture == 1) ? texture(glyph, uv).r : 1.0;
    FragColor = vec4(color.rgb, color.a * alpha);
}
