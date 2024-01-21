#version 330 core
out vec4 FragColor;

in vec3 FragmentPosition;  
in vec3 NormalVector;  
in vec3 LightPosition;   // extra in variable, since we need the light position in view space we calculate this in the vertex shader

//uniform vec3 viewPosition; 
uniform vec3 objectColor;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
}; 
  
uniform Material material;

struct LightProperties {
    vec3 position;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform LightProperties lightProperties;  

void main()
{
    // Ambient Lighting
    vec3 ambientLightingColor = material.ambient * lightProperties.ambient;

    // Diffuse Lighting
    vec3 lightDirection = normalize(LightPosition - FragmentPosition); 
    float diffuseQuantity = max(dot(NormalVector, lightDirection), 0.0);
    vec3 diffuseColor = (diffuseQuantity * material.diffuse) * lightProperties.diffuse;

    // Specular Lighting
    float specularStrength = 0.5;
    vec3 viewDirection = normalize(- FragmentPosition);
    vec3 reflectDirection = reflect(-lightDirection, NormalVector);  
    float specularPower = pow(max(dot(viewDirection, reflectDirection), 0.0), material.shininess);
    vec3 specularColor = (specularPower * material.specular) * lightProperties.specular;  

    vec3 result = (
    ambientLightingColor 
    + diffuseColor 
    + specularColor
    ) 
    * objectColor;
    FragColor = vec4(result, 1.0);
}