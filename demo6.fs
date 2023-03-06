/******************************************************************************
 * This fragment shader calculates a reflection direction and converts this
 * into environment map texture coordinates.
 *
 * Happy hacking! - eric
 *****************************************************************************/

#version 330 core

in vec3 shaderPosition;
in vec3 shaderNormal;
in vec2 shaderTexCoord;
in mat3 shaderInverseView;
uniform sampler2D diffuseMap;
uniform sampler2D environmentMap;
out vec4 fragmentColor;

void main()
{
    // normalize the interpolated normal from the vertex shader
    // (reflection calculations will break if the normal direction is not a unit vector)
    vec3 normalDir = normalize(shaderNormal);

    // calculate reflection direction (and undo its camera transformation)
    vec3 reflectDir = normalize(reflect(shaderPosition, normalDir));
    reflectDir = shaderInverseView * reflectDir;

    // calculate texture coordinates into the environment map by converting
    // the reflection direction into spherical polar coordinates
    vec2 texCoord = vec2(atan(reflectDir.z, reflectDir.x) / radians(360.0f) + 0.5f,
                         acos(-reflectDir.y) / radians(180.0f));

    // mix the diffuse and environment colors using the diffuse color's alpha
    // (the final fragment color is forced to be fully opaque
    // in case OpenGL alpha-blending is also active)
    vec4 diffuseColor = texture(diffuseMap, shaderTexCoord);
    vec4 environmentColor = texture(environmentMap, texCoord);
    fragmentColor = vec4(mix(environmentColor, diffuseColor, diffuseColor.a).rgb, 1.0f);
}
