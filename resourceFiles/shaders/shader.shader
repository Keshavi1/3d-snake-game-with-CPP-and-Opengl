#VERTEX SHADER
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 atexcoord;


out vec2 TexCoord;
out vec3 FragPos;

uniform mat4 model;

void main(){
    gl_Position = model * vec4(aPos,1.0);
    TexCoord = atexcoord;
    FragPos = vec3(model * (vec4(aPos, 1.0)));

}

#FRAGMENT SHADER
#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
in vec3 FragPos;

uniform sampler2D Tex;
uniform vec3 birdPos;
uniform float linear;
uniform float quad;


void main(){

    FragColor = texture(Tex,TexCoord);
    if(FragColor.a < 0.1){discard;}
    float dist = length(birdPos - FragPos);
    FragColor *= 1.0/ (1.0 + linear * dist + quad * (dist*dist)) + 0.05;
}

