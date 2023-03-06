/******************************************************************************
 * This vertex shader is similar to demo5.vs but with lighting precalculations
 * omitted.
 *
 * We also pass to the fragment shader a matrix to undo the view (camera)
 * transformation, so we can properly use an environment map.
 *
 * Happy hacking! - eric
 *****************************************************************************/

#version 330 core

layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec3 vertexNormal;
layout (location = 2) in vec2 vertexTexCoord;
uniform mat4 projectionTransform;
uniform mat4 viewTransform;
uniform mat4 modelTransform;
out vec3 shaderPosition;
out vec3 shaderNormal;
out vec2 shaderTexCoord;
out mat3 shaderInverseView;

void main()
{
    // combine the model and view transforms to get the camera space transform
    mat4 modelViewTransform = viewTransform * modelTransform;

    // compute the vertex's attributes in camera space
    shaderPosition = vec3(modelViewTransform * vec4(vertexPosition, 1.0f));
    shaderTexCoord = vertexTexCoord;

    // compute the normal transform as the transpose of the inverse of the camera transform,
    // then transform the normal vector accordingly
    mat3 normalTransform = mat3(transpose(inverse(modelViewTransform)));
    shaderNormal = normalTransform * vertexNormal;

    // to get the proper environment map coordinates, we need to
    // undo the view transformation on the reflected vector in the fragment shader
    shaderInverseView = mat3(inverse(viewTransform));

    // we still need OpenGL to compute the final vertex position in projection space
    // to correctly determine where the fragments of the triangle actually go on the screen
    gl_Position = projectionTransform * vec4(shaderPosition, 1.0f);
}
