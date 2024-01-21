#version 330 core
out vec4 FragColor;

in vec3 FragmentPosition;  
in vec3 NormalVector;  
in vec3 LightPosition;   // extra in variable, since we need the light position in view space we calculate this in the vertex shader

//uniform vec3 viewPosition; 
uniform vec3 lightColor;
uniform vec3 objectColor;

void main()
{
    // Ambient Lighting
    float ambientStrength = 0.1;
    vec3 ambientLighting = ambientStrength * lightColor;    

    // Diffuse Lighting
    vec3 normalVector = normalize(NormalVector);
    vec3 lightDirection = normalize(LightPosition - FragmentPosition); 
    float diffuseQuantity = max(dot(normalVector, lightDirection), 0.0);
    vec3 diffuseColor = diffuseQuantity * lightColor;

    // Specular Lighting
    float specularStrength = 0.5;
    vec3 viewDirection = normalize(- FragmentPosition);
    vec3 reflectDirection = reflect(-lightDirection, normalVector);  
    float spec = pow(max(dot(viewDirection, reflectDirection), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;  

    vec3 result = (ambientLighting + diffuseColor + specular) * objectColor;
    FragColor = vec4(result, 1.0);
}