#pragma once

#include "Texture.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define PI (3.141592653589793)
#define GRADOSTORADIANES PI / 180


class Astro{

  
public:
    glm::vec3 posicion;
    float distancia;
    float velocidad_trans;
    float angulo_trans;
    float velocidad_rot;
    float angulo_rot;
    float tamano;
    
    Astro(float distancia, float velocidad_trans, float angulo_trans, float velocidad_rot, float angulo_rot, float tamano, GLchar* pathToTexture){
        this->distancia = distancia;
        this->velocidad_trans = velocidad_trans;
        this->angulo_trans = angulo_trans;
        this->velocidad_rot = velocidad_rot;
        this->angulo_rot = angulo_rot;
        this->tamano = tamano;
        this->pathToTexture = pathToTexture;
        this->esfera = Sphere(20.0f, 36, 18);
        this->radio = 20 * tamano;
        this->posicion =glm::vec3(this->distancia*  sin(this->angulo_trans * GRADOSTORADIANES), this->distancia*cos(this->angulo_trans*GRADOSTORADIANES),0.0f);
    }
    void cargarTexturaAstro(){
        this->textura = CargarTexturas::cargarTexturas(this->pathToTexture);
    }
    
    void dibujarAstro(glm::mat4 model, Shader shader, GLuint modelLoc, float deltaTime){
        this->angulo_trans += this->velocidad_trans * deltaTime;
        if(this->angulo_trans > 360) this->angulo_trans -= 360;
        this->angulo_rot += this->velocidad_rot * deltaTime;
        if(this->angulo_rot > 360) this->angulo_rot -= 360;
        this->posicion =glm::vec3(this->distancia*  sin(this->angulo_trans * GRADOSTORADIANES), this->distancia*cos(this->angulo_trans*GRADOSTORADIANES),0.0f);
        model = glm::mat4(1);
        model = glm::translate(model, this->posicion);
        model = glm::rotate(model, (float)(this->angulo_rot), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, glm::vec3(this->tamano));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glBindTexture(GL_TEXTURE_2D, this->textura); 
        esfera.draw();
    }
    
    float getRadio(){
        return this->radio;
    }
    
    glm::vec3 getPosicion(){
        return this->posicion;
    }

private:
    GLchar* pathToTexture;
    Sphere esfera;
    GLuint textura;
    float radio;
};
