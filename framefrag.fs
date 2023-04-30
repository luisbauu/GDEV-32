#version 330 core

out vec4 FragColor;
in vec2 texCoords;

uniform sampler2D screentexture;

void main()
{
    vec4 tex = texture(screentexture, texCoords);
    float avg = (tex.x + tex.y + tex.z) / 3.0f;
    FragColor = vec4(avg,avg,avg, 1.0f);
}