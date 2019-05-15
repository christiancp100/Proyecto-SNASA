#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Texture.h"
#include "Model.h"

#define PI (3.141592653589793)
#define GRADOSTORADIANES PI / 180

class Nave{
  
public:
    
    Nave(GLchar* path_to_model = (GLchar*)"res/models/SF_Fighter/SciFi_Fighter.obj", glm::vec3 posicion= glm::vec3(2000.0f), glm::vec3 escala = glm::vec3(0.2f), glm::vec3 rotacion= glm::vec3(20.0f)){
        this->posicion = posicion;
        this->rotacion = rotacion;
        this->escala = escala;
        this->anguloGiroX = 0;
        this->anguloGiro = 0;
        this->anguloZ = 0;
        this->velocidad = 0;
        this->radio = 100.f;
        this->nave = new Model(path_to_model);
        srand (time(NULL));

    }
    
    Nave(Model modelo, glm::vec3 posicion= glm::vec3(2000.0f), glm::vec3 escala = glm::vec3(0.2f), glm::vec3 rotacion= glm::vec3(20.0f)){
        this->posicion = posicion;
        this->rotacion = rotacion;
        this->escala = escala;
        this->anguloGiroX = 0;
        this->anguloGiro = 0;
        this->anguloZ = 0;
        this->velocidad = 0;
        this->radio = 300.f;
        this->nave = &modelo;
        srand (time(NULL));
        
    }
    
    void atacar(glm::mat4 view, glm::mat4 projection, glm::mat4 model, glm::vec3 posicion_a_atacar, float deltaTime){
        this->shader.Use();
        glUniformMatrix4fv( glGetUniformLocation( this->shader.Program, "projection" ), 1, GL_FALSE, glm::value_ptr( projection ) );
        glUniformMatrix4fv( glGetUniformLocation( this->shader.Program, "view" ), 1, GL_FALSE, glm::value_ptr( view ) );
        // Luces de los astros
        unsigned int lightPosLoc = glGetUniformLocation(this->shader.Program, "lightPos");
        glUniform3f(lightPosLoc, 0, 0, 0);
        unsigned int colorLoc = glGetUniformLocation(this->shader.Program, "objectColor");
        glUniform3f(colorLoc, 1.0, 1.0f, 1.0f);
        unsigned int lightLoc = glGetUniformLocation(this->shader.Program, "lightColor");
        glUniform3f(lightLoc, 1.0, 1.0, 1.0f);
        
        
        
        if(rotacion.x != 0)
            rotacion = glm::vec3(0.0f, angulo_entre_vectores(this->posicion, posicion_a_atacar), 0.0f);
        else
            rotacion = glm::vec3(rand() % 3 -1, rand() % 3 -1, rand() % 3 -1);
        this->posicion += this->rotacion * this->velocidad;
        
        model = glm::mat4(1);
        model = glm::translate( model, this->posicion);
        model = glm::rotate(model, (float)(90.0f*GRADOSTORADIANES) , glm::vec3(0.0f,1.0f,0.0f));
        model = glm::rotate(model, (float)(this->anguloGiro*-GRADOSTORADIANES) , glm::vec3(0.0f,1.0f,0.0f));
        model = glm::rotate(model, (float)(this->anguloZ*-GRADOSTORADIANES) , glm::vec3(0.0f,0.0f,1.0f));
        model = glm::rotate(model, (float)(this->anguloGiroX*GRADOSTORADIANES), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::scale( model, this->escala);
        glUniformMatrix4fv( glGetUniformLocation( this->shader.Program, "model" ), 1, GL_FALSE, glm::value_ptr( model ) );
        this->nave->Draw( this->shader );
    }
    
    
    void dibujaModelo(glm::mat4 view, glm::mat4 projection, glm::mat4 model){
        this->shader.Use();
        glUniformMatrix4fv( glGetUniformLocation( this->shader.Program, "projection" ), 1, GL_FALSE, glm::value_ptr( projection ) );
        glUniformMatrix4fv( glGetUniformLocation( this->shader.Program, "view" ), 1, GL_FALSE, glm::value_ptr( view ) );
        // Luces de los astros
        unsigned int lightPosLoc = glGetUniformLocation(this->shader.Program, "lightPos");
        glUniform3f(lightPosLoc, 0, 0, 0);
        unsigned int colorLoc = glGetUniformLocation(this->shader.Program, "objectColor");
        glUniform3f(colorLoc, 1.0, 1.0f, 1.0f);
        unsigned int lightLoc = glGetUniformLocation(this->shader.Program, "lightColor");
        glUniform3f(lightLoc, 1.0, 1.0, 1.0f);
                model = glm::mat4(1);
        model = glm::translate( model, this->posicion);
        model = glm::rotate(model, (float)(90.0f*GRADOSTORADIANES) , glm::vec3(0.0f,1.0f,0.0f));
        model = glm::rotate(model, (float)(this->anguloGiro*-GRADOSTORADIANES) , glm::vec3(0.0f,1.0f,0.0f));
        model = glm::rotate(model, (float)(this->anguloZ*-GRADOSTORADIANES) , glm::vec3(0.0f,0.0f,1.0f));
        model = glm::rotate(model, (float)(this->anguloGiroX*GRADOSTORADIANES), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::scale( model, this->escala);
        glUniformMatrix4fv( glGetUniformLocation( this->shader.Program, "model" ), 1, GL_FALSE, glm::value_ptr( model ) );
        this->nave->Draw( this->shader );
    }
    
    glm::vec3 getAngulos( ){
        return glm::vec3(this->anguloGiroX, this->anguloGiro, this->anguloZ);
    }
    
    float getAnguloZ(){
        return this->anguloZ;
    }
    
    void incrementarAnguloZ(int mode){
        if(mode == 0){
            if(anguloZ <= 15)
            anguloZ += 0.5;
        }
        else if(mode == 1){
            if(anguloZ >= -15)
            anguloZ -= 0.5;
        }
           
    }
    
    void ascenso(){
        if (this->anguloGiroX <= 90) {
            anguloGiroX -= 0.5;
        }
    }
    
    void descenso(){
        if (this->anguloGiroX >= -90) {
            anguloGiroX += 0.5;
        }
    }
    
    float getRadio(){
        return this->radio;
    }
    
    glm::vec3 getPosicion(){
        return this->posicion;
    }
    
    glm::vec3 getRotacion(){
        return this->rotacion;
    }
    
    glm::vec3 getEscala(){
        return this->escala;
    }
    
    Shader getShader(){
        return this->shader;
    }
    
    float getAnguloGiro( ){
        return this->anguloGiro;
    }
    
    float getVelocidad(){
        return this->velocidad;
    }
    
    void setVelocidad( float velocidad){
        this->velocidad = velocidad;
    }
    
    void incrementarAngulo(float incremento){
        this->anguloGiro += incremento;
    }
    
    void incrementarPosicion(glm::vec3 incremento){
        this->posicion += incremento;
    }
    
    void incrementarVelocidad(float incremento){
        if(this->velocidad < 10)
        this->velocidad += incremento;
    }
    
    bool detectaColisiones(glm::vec3 obj, float radio) {
        if (this->distaciaDosObjetos(obj) <= this->radio + radio)
        return true;
        else
        return false;
    }
    
    void setPosicion(glm::vec3 posicion) {
        this->posicion = posicion;
    }
    
private:
    Model *nave;
    glm::vec3 posicion;
    glm::vec3 escala;
    glm::vec3 rotacion;
    float anguloGiroX;
    float anguloGiro;
    float anguloZ;
    float velocidad;
    Shader shader = Shader("res/shaders/modelLoading.vs", "res/shaders/modelLoading.frag");
    float radio;
    
    
    float distaciaDosObjetos(glm::vec3 b) {
        return (sqrt((this->posicion.x - b.x) * (this->posicion.x - b.x) + (this->posicion.y - b.y) * (this->posicion.y - b.y) + (this->posicion.z - b.z) * (this->posicion.z - b.z)));
    }
    
    float angulo_entre_vectores(glm::vec3 a, glm::vec3 b){
        if(glm::length(a) * glm::length(b) == 0){
            return glm::acos( glm::dot(a, b) / (glm::length(a) * glm::length(b)) );
        }else{
            return 0;
        }
    }
    
};
