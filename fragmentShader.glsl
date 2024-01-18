#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;

uniform vec4 varyingColor;

void main()
{
    FragColor = 
    mix(
        texture(texture1, TexCoord), 
        texture(texture2, TexCoord),0.7
    ) 
//    * vec4(
//        2*ourColor.x*varyingColor.x/(ourColor.x + varyingColor.x) +1.0, 
//        2*ourColor.y*varyingColor.y/(ourColor.y + varyingColor.y) +1.0, 
//        2*ourColor.z*varyingColor.z/(ourColor.z + varyingColor.z) +1.0, 
//        1.0f
//    )
    ;
}