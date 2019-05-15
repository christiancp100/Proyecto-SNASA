#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Texture.h"
#include "Model.h"

#define PI (3.141592653589793)
#define GRADOSTORADIANES PI / 180

class Misil{

public:
    
    Misil( glm::vec3 posicion= glm::vec3(0.0f), glm::vec3 escala = glm::vec3(2.0f)){
        this->posicion = posicion;
        this->escala = escala;
        this->velocidad = 100;
        this->cuerpo = Sphere(5.0f, 36, 18);
        this->radio = 10.f;
        
        //this->cargarTexturaMisil();
    }
    
    Misil(GLuint codigo_textura, glm::vec3 posicion= glm::vec3(0.0f), glm::vec3 escala = glm::vec3(2.0f)){
        this->posicion = posicion;
        this->escala = escala;
        this->velocidad = 100;
        this->cuerpo = Sphere(5.0f, 36, 18);
        this->textura = codigo_textura;
        this->radio = 10.f;
        //this->cargarTexturaMisil();
    }
    
    
    void disparar(glm::mat4 view, glm::mat4 projection, glm::mat4 model){
        this->shader.Use();
        glUniformMatrix4fv( glGetUniformLocation( this->shader.Program, "projection" ), 1, GL_FALSE, glm::value_ptr( projection ) );
        glUniformMatrix4fv( glGetUniformLocation( this->shader.Program, "view" ), 1, GL_FALSE, glm::value_ptr( view ) );
        // Luces de los astros
        unsigned int lightPosLoc = glGetUniformLocation( this->shader.Program, "light.position" );
        glUniform3f(lightPosLoc, 0, 0, 0);
        unsigned int colorLoc = glGetUniformLocation(this->shader.Program, "objectColor");
        glUniform3f(colorLoc, 0.0, 1.0f, 1.0f);
        GLint viewPosLoc = glGetUniformLocation( this->shader.Program, "viewPos" );
        glUniform3f( lightPosLoc, this->posicion.x, this->posicion.y, this->posicion.z );
        glUniform3f( viewPosLoc,  this->posicion.x, this->posicion.y, this->posicion.z );
        // Set lights properties
        glUniform3f( glGetUniformLocation( this->shader.Program, "light.ambient" ), 0.9f, 0.9f, 0.9f );
        glUniform3f( glGetUniformLocation( this->shader.Program, "light.diffuse" ), 0.9f, 0.9f, 0.9f );
        glUniform3f( glGetUniformLocation( this->shader.Program, "light.specular" ), 1.0f, 1.0f, 1.0f );
        glUniform1f( glGetUniformLocation( this->shader.Program, "light.constant" ), 1.0f );
        glUniform1f( glGetUniformLocation( this->shader.Program, "light.linear" ), 1.0f );
        glUniform1f( glGetUniformLocation( this->shader.Program, "light.quadratic" ), 1.0f );
        // Set material properties
        glUniform1f( glGetUniformLocation( this->shader.Program, "material.shininess"), 172.0f );
        model = glm::mat4(1);
        model = glm::rotate(model, (float)(0), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::translate( model, this->posicion - glm::vec3(200.0f, 400.f, 0.0f));
        model = glm::scale( model, this->escala);
        glUniformMatrix4fv( glGetUniformLocation( this->shader.Program, "model" ), 1, GL_FALSE, glm::value_ptr( model ) );
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glBindTexture(GL_TEXTURE_2D, this->textura);
        //this->misil.Draw( this->shader );
        this->cuerpo.draw();
        model = glm::mat4(1);
        model = glm::rotate(model, (float)(0), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::translate( model, this->posicion - glm::vec3(-200.0f, 400.f, 0.0f));
        model = glm::scale( model, this->escala);
        glUniformMatrix4fv( glGetUniformLocation( this->shader.Program, "model" ), 1, GL_FALSE, glm::value_ptr( model ) );
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glBindTexture(GL_TEXTURE_2D, this->textura);
        //this->misil.Draw( this->shader );
        this->cuerpo.draw();

    }
    
    void cargarTexturaMisil(){
        this->textura = CargarTexturas::cargarTexturas(this->pathToTexture);
    }
    
    glm::vec3 getPosicion(){
        return this->posicion;
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
    
    void setAnguloGiro(float angulo){
        this->anguloGiro = angulo;
    }
    
    float getVelocidad(){
        return this->velocidad;
    }
    
    void setVelocidad( float velocidad){
        this->velocidad = velocidad;
    }
    
    
    void incrementarPosicion(glm::vec3 incremento){
        this->posicion += incremento;
    }
    
    void setPosicion(glm::vec3 pos){
        this->posicion = pos;
    }
    
    bool detectaColisiones(glm::vec3 obj, float radio) {
        if (this->distaciaDosObjetos(obj) <= this->radio + radio)
        return true;
        else
        return false;
    }
    
    
private:
    
    //Model misil = Model((GLchar*)"res/models/aim120/aim120.obj");
    Sphere cuerpo;
    glm::vec3 posicion;
    glm::vec3 escala;
    float anguloGiro;
    float velocidad;
    Shader shader = Shader("res/shaders/misil.vert", "res/shaders/misil.frag");
    GLuint textura;
    GLchar* pathToTexture = (GLchar*)"res/TEXTURAS/missile.jpg";
    float radio;
    
    float distaciaDosObjetos(glm::vec3 b) {
        return (sqrt((this->posicion.x - b.x) * (this->posicion.x - b.x) + (this->posicion.y - b.y) * (this->posicion.y - b.y) + (this->posicion.z - b.z) * (this->posicion.z - b.z)));
    }
};
