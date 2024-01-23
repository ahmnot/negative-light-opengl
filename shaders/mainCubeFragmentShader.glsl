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

    /**
    If you want "negative light" to work better, 
    it is better to have a strong ambient lighting. 
    "negative light" is not directly related to ambient light,
    but you want your scene to be fully lit or at least partially lit, 
    and then "substract light" (="substract color") from it.
    */

    // Ambient Lighting
    vec3 ambientColor = lightProperties.ambient * (texture(material.diffuseMap, TextureCoordinates).rgb);

    // Diffuse Lighting
    vec3 lightDirection = normalize(LightPosition - FragmentPosition); 
    float diffuseQuantity = max(dot(NormalVector, lightDirection), 0.0);
    vec3 diffuseColor = lightProperties.diffuse * diffuseQuantity * (texture(material.diffuseMap, TextureCoordinates).rgb);

    // Specular Lighting
    /** 
    When calculating light position in the vertex shader, in view space, 
    we can put (0,0,0) instead of viewPosition.
    */
    vec3 viewDirection = normalize(/* (0,0,0) */ - FragmentPosition);
    vec3 reflectDirection = reflect(-lightDirection, NormalVector);  
    float specularPower = pow(max(dot(viewDirection, reflectDirection), 0.0), material.shininess);
    vec3 specularMap = texture(material.specularMap, TextureCoordinates).rgb;
    vec3 specularColor = lightProperties.specular * specularPower * specularMap;

    float lightFragmentdistance = length(LightPosition - FragmentPosition);
    float attenuation = 1.0 / 
    (lightProperties.attenuationConstantTerm 
    + lightProperties.attenuationLinearTerm * lightFragmentdistance 
    + lightProperties.attenuationQuadraticTerm * (lightFragmentdistance * lightFragmentdistance)); 
    
    // "negative light" works better with no attenuation on the ambient.
    //ambientColor *= attenuation;
    diffuseColor *= attenuation;
    specularColor *= attenuation;
   
    /**
    By replacing the "+" by "-", we make this "negative light".
    We SUBSTRACT the diffuseColor and specularColor from the ambientColor.
    */
    FragmentColor = vec4(ambientColor - diffuseColor - specularColor, 1.0);
}