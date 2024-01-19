#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;

uniform vec4 varyingColor;

uniform float mixCoefficient;

void main()
{
    FragColor = 
//    mix(
//        texture(texture1, TexCoord), 
//        texture(texture2, TexCoord),
//        mixCoefficient
//    ) 
//    * 
    vec4(
        2*(ourColor.x+0.02)*(varyingColor.x)/(ourColor.x + varyingColor.x) +0.65, 
        2*ourColor.y*varyingColor.y/(ourColor.y + varyingColor.y) +0.65, 
        2*ourColor.z*varyingColor.z/(ourColor.z + varyingColor.z) +0.65, 
        1.0f
    )
    ;
}