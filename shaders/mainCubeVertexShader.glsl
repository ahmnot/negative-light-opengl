#version 330 core
layout (location = 0) in vec3 positionAttribute;
layout (location = 1) in vec3 normalVectorAttribute;
layout (location = 2) in vec2 textureCoordinatesAttribute;

out vec3 FragmentPosition; 
out vec3 NormalVector;
out vec3 LightPosition;
out vec2 TextureCoordinates;

uniform vec3 lightPosition; // we define the uniform in the vertex shader and pass the 'view space' lightpos to the fragment shader. lightPos is currently in world space.

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main()
{
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(positionAttribute, 1.0);

    FragmentPosition = vec3(viewMatrix * modelMatrix * vec4(positionAttribute, 1.0));
    NormalVector = normalize(mat3(transpose(inverse(viewMatrix * modelMatrix))) * normalVectorAttribute);  
    LightPosition = vec3(viewMatrix * vec4(lightPosition, 1.0)); // Transform world-space light position to view-space light position
    TextureCoordinates = textureCoordinatesAttribute;
} 