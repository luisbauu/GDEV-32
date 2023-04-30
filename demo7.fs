/******************************************************************************
 * This fragment shader inverts the colors of the framebuffer.
 *
 * Modify this file to experiment with different post-processing effects!
 *
 * Happy hacking! - eric
 *****************************************************************************/

#version 330 core

in vec2 shaderTexCoord;

uniform sampler2D screenTexture;
uniform int effectNum;

out vec4 fragmentColor;

void main()
{
    /*vec4 color = texture(screenTexture,
                         shaderTexCoord);
    fragmentColor = 1.0f - color;*/

    vec4 tex = texture(screenTexture, shaderTexCoord);
    float avg = (tex.x + tex.y + tex.z) / 3.0f;
    fragmentColor = vec4(avg,avg,avg, 1.0f);
}
