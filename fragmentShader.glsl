#version 330 core
out vec4 FragColor;

in vec3 ourColor;
uniform vec4 varyingColor;

void main()
{
    FragColor = vec4(
    2*ourColor.x*varyingColor.x/(ourColor.x + varyingColor.x) +0.85, 
    2*ourColor.y*varyingColor.y/(ourColor.y + varyingColor.y) +0.85, 
    2*ourColor.z*varyingColor.z/(ourColor.z + varyingColor.z) +0.85, 
    1.0f
    );
}