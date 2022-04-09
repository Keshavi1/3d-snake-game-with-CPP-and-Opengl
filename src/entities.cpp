#include "entities.h"
#include "shaderutil.h"


std::random_device rd;
std::mt19937 gen(rd());
std::uniform_real_distribution<> bpHeight(-1.3,-0.7), tpHeight(2.0, 1.7);

glm::vec3 birdPos(0.0,0.0,0.0);
float birdSpeed = 0;
float birdRotation = 0;
//make array later
glm::vec3 bottomPipePos[] = {
    glm::vec3(2.0,bpHeight(gen),0.0), 
    glm::vec3(6.0,bpHeight(gen),0.0), 
    glm::vec3(10.0,bpHeight(gen),0.0)
};
glm::vec3 topPipePos[] = {
    glm::vec3(2.0,bottomPipePos[0].y + tpHeight(gen),0.0), 
    glm::vec3(6.0,bottomPipePos[1].y + tpHeight(gen),0.0), 
    glm::vec3(10.0,bottomPipePos[2].y + tpHeight(gen),0.0)
};
float pipeSpeed = 0.001;

glm::vec3 bgPos[] = {glm::vec3(0.0), glm::vec3(1.0,0.0,0.0)};






void drawbg(unsigned int texture, unsigned int &program, size_t size){
    setUniVec3(program, "birdPos", birdPos* glm::vec3(0.2));
    setUniFloat(program, "linear", 2);
    setUniFloat(program, "quad", 0.8);
    glActiveTexture(GL_TEXTURE0);

    //createing a continuouse scrolling affect with two backgrounds
    //?maybe different images can be used
    glBindTexture(GL_TEXTURE_2D, texture);
    glm::mat4 model = glm::scale(glm::mat4(1.0f), glm::vec3(2.01));
    model = glm::translate(model, bgPos[0]);
    setUniMat4(program,"model",model);
    glDrawArrays(GL_TRIANGLES,0,size);
    model = glm::scale(glm::mat4(1.0f), glm::vec3(2.01));
    model = glm::translate(model, bgPos[1]);
    setUniMat4(program,"model",model);
    glDrawArrays(GL_TRIANGLES,0,size);

    //scrolls the background
    bgPos[0].x < -1.0 ? bgPos[0].x = 1.0 : bgPos[0].x -= 0.0001;
    bgPos[1].x < -1.0 ? bgPos[1].x = 1.0 : bgPos[1].x -= 0.0001;
}

auto checkCollision = []()-> bool{
    for(int i = 0; i < 3; ++i)
        if(birdPos.x + 0.5 >= bottomPipePos[i].x && birdPos.x <= bottomPipePos[i].x + 0.5)
            if(birdPos.y* 0.2 <= (bottomPipePos[i].y + 0.5) || birdPos.y * 0.2 >= (topPipePos[i].y - 0.5))
                return true;
    return false;
};
void updateAndDrawBird(unsigned int texture, unsigned int &program, size_t size){
    glActiveTexture(GL_TEXTURE0);
    
    glBindTexture(GL_TEXTURE_2D, texture);
    glm::mat4 model = glm::scale(glm::mat4(1.0f), glm::vec3(0.2));
    model = glm::translate(model,birdPos);
    model = glm::rotate(model, glm::radians(birdRotation), glm::vec3(0.0,0.0,1.0));
    setUniMat4(program,"model",model);
    glDrawArrays(GL_TRIANGLES,0,size);

    //gravity 
    birdPos.y += birdSpeed;
    if(birdSpeed > -0.025)birdSpeed -= 0.00005;
    //bird rotation
    birdRotation -= 0.125;
    if(birdRotation < -45.0f) birdRotation = -45.0f;
    //pipe and ground collision
    if(birdPos.y <= -5) birdSpeed = 0;
    if(checkCollision()){
        for(int i = 0; i < 3; ++i)
            bottomPipePos[i].x = 2.0f + (i * 4);
    }
}

void flap(){
    birdSpeed = 0.015;
    birdRotation = 45.0f;
}


void updateAndDrawPipes(unsigned int bottom, unsigned int top, unsigned int &program, size_t size){
    setUniFloat(program, "linear", 1.0);
    setUniFloat(program, "quad", 0.25);
    glActiveTexture(GL_TEXTURE0);

    for(int i = 0; i < 3; ++i){
            //draws bottom pipe
        glm::mat4 model = glm::scale(glm::mat4(1.0f), glm::vec3(0.5,1.0,0.5));
        model = glm::translate(model,bottomPipePos[i]);
        setUniMat4(program,"model",model);
        glBindTexture(GL_TEXTURE_2D, bottom);
        glDrawArrays(GL_TRIANGLES,0,size);

        // draws top pipe
        model = glm::scale(glm::mat4(1.0f), glm::vec3(0.5,1.0,0.5));
        model = glm::translate(model,topPipePos[i]);
        setUniMat4(program,"model",model);
        glBindTexture(GL_TEXTURE_2D, top);
        glDrawArrays(GL_TRIANGLES,0,size);

        //moves the pipes
        bottomPipePos[i].x -= pipeSpeed;
        if(bottomPipePos[i].x < -3){
            bottomPipePos[i].x = 3;
            bottomPipePos[i].y = bpHeight(gen);
            topPipePos[i].y = bottomPipePos[i].y + tpHeight(gen);
        }
        topPipePos[i].x = bottomPipePos[i].x;
    }
}
