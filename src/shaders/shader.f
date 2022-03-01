#version 330 core
out vec4 FragColor;

in vec3 vColor;
in vec2 texCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform float mixValue;

void main()
{
   FragColor = vec4(vColor, 1.0); 

};
//fragment shader