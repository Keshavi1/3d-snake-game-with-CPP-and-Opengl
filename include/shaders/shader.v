#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTextCoord;

uniform mat4 transformation;

out vec3 vColor;
out vec2 textCoord;

void main()
{
   gl_Position = transformation * vec4(aPos, 1.0);
   vColor = aColor; 
   textCoord = vec2(aTextCoord.x, aTextCoord.y);

};
//vertex shader