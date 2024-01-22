#version 330 core
out vec4 FragmentColor;

in vec3 FragmentPosition;  
in vec3 NormalVector;  
in vec3 LightPosition;   // extra in variable, since we need the light position in view space we calculate this in the vertex shader

in vec2 TextureCoordinates;

struct Material {
    sampler2D diffuseMap;
    sampler2D specularMap;
    sampler2D emissionMap;
    float     shininess;
}; 
  
uniform Material material;

struct LightProperties {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
	
    float attenuationConstantTerm;
    float attenuationLinearTerm;
    float attenuationQuadraticTerm;
};

uniform LightProperties lightProperties;  

void main()
{
    // Ambient Lighting
    vec3 ambientLightingColor = lightProperties.ambient * (texture(material.diffuseMap, TextureCoordinates).rgb);

    // Diffuse Lighting
    vec3 lightDirection = normalize(LightPosition - FragmentPosition); 
    float diffuseQuantity = max(dot(NormalVector, lightDirection), 0.0);
    vec3 diffuseColor = lightProperties.diffuse * diffuseQuantity * (texture(material.diffuseMap, TextureCoordinates).rgb);

    // Specular Lighting
    vec3 viewDirection = normalize(- FragmentPosition);
    vec3 reflectDirection = reflect(-lightDirection, NormalVector);  
    float specularPower = pow(max(dot(viewDirection, reflectDirection), 0.0), material.shininess);
    vec3 specularMap = texture(material.specularMap, TextureCoordinates).rgb;
    vec3 specularColor = lightProperties.specular * specularPower * specularMap;

    // Emission map with emission mask (for the box borders) */
    vec3 emissionMap = texture(material.emissionMap, TextureCoordinates).rgb;
    vec3 emissionMask = step(vec3(1.0f), vec3(1.0f)-specularMap);
    vec3 emissionColor = vec3(0.0f);// = emissionMap * emissionMask;

    
    float lightFragmentdistance = length(LightPosition - FragmentPosition);
    float attenuation = 1.0 / 
    (lightProperties.attenuationConstantTerm 
    + lightProperties.attenuationLinearTerm * lightFragmentdistance 
    + lightProperties.attenuationQuadraticTerm * (lightFragmentdistance * lightFragmentdistance)); 
    
    // If you want "negative light" to work better with attenuation, better comment this line:
    //ambientLightingColor *= attenuation;
    diffuseColor *= attenuation;
    specularColor *= attenuation;
   
    // By replacing the "+" by "-", we make this "negative light".
    FragmentColor = vec4(ambientLightingColor - diffuseColor - specularColor + emissionColor, 1.0);
}