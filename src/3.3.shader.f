#version 330 core
out vec4 FragColor;

in vec3 vColor;
in vec2 textCoord;
uniform sampler2D theTexture;

void main()
{
   FragColor = texture(theTexture,textCoord);

};
//fragment shader