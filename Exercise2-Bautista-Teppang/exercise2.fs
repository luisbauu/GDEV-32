#version 330 core

in vec3 shaderPosition;
in mat3 shaderTBN;
in vec2 shaderTexCoord;
in vec3 shaderLightPosition;
in vec3 shaderFocusPosition;

uniform float uniformAmbientIntensity;
uniform float uniformSpecularIntensity;
uniform float uniformSpecularPower;

uniform float spotlightCutoff;
uniform float spotlightOuterAngle;

uniform sampler2D diffuseMap;
uniform sampler2D normalMap;
out vec4 fragmentColor;

void main()
{
    vec3 lightDir = normalize(shaderLightPosition - shaderPosition);
    vec3 spotlightDir = normalize(shaderFocusPosition - shaderPosition);
    float theta = dot(-lightDir, spotlightDir);
    
    float I = clamp((theta-spotlightOuterAngle)/(spotlightCutoff-spotlightOuterAngle), 0, 1);
    
    // define some constant properties for the light
    // (you should really be passing these parameters into the shader as uniform vars instead)
    vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);  // diffuse
    float ambientIntensity = uniformAmbientIntensity;            // ambient
    float specularIntensity = uniformSpecularIntensity;            // specular (better implementation: look this up from a specular map!)
    float specularPower = uniformSpecularPower;

    // look up the normal from the normal map, then reorient it with the current model transform via the TBN matrix
    vec3 textureNormal = vec3(texture(normalMap, shaderTexCoord));
    textureNormal = normalize(textureNormal * 2.0f - 1.0f);  // convert range from [0, 1] to [-1, 1]
    vec3 normalDir = normalize(shaderTBN * textureNormal);

    // calculate ambient
    vec3 lightAmbient = lightColor * ambientIntensity;

    // calculate diffuse
    vec3 lightDiffuse = max(dot(normalDir, lightDir), 0.0f) * lightColor;

    // calculate specular
    vec3 viewDir = normalize(-shaderPosition);
    vec3 reflectDir = reflect(-lightDir, normalDir);
    vec3 lightSpecular = pow(max(dot(reflectDir, viewDir), 0), specularPower) * lightColor * specularIntensity;


    if (I == 1)
    {
        fragmentColor = vec4((lightAmbient + lightDiffuse + lightSpecular), 1.0f) * texture(diffuseMap, shaderTexCoord);
    }
    else if (I > 0 && I < 1)
    {
        fragmentColor = vec4((lightAmbient + (lightDiffuse + lightSpecular) * I), 1.0f) * texture(diffuseMap, shaderTexCoord);
    }
    else
    {
        fragmentColor = vec4(lightAmbient, 1.0f) * texture(diffuseMap, shaderTexCoord);
    }
}
