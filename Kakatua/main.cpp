#include <iostream>
#include <cmath>
// GLEW
#define GLEW_STATIC
#include <GL/glew.h>
// GLFW
#include <GLFW/glfw3.h>
//Libreria de textura¡
#include "SOIL2/SOIL2.h"
//Audio

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Shader.h"
#include "Camera.h"
#include "Model.h"
#include "Texture.h"
#include "Sphere.h"
#include "Astro.h"
#include "Skybox.h"
#include "Nave.h"
#include "Misil.h"
//Constantes
#define SCALE 20
#define NUMERO_ENEMIGOS 5
unsigned int lightLoc;
//Prototipos
GLFWwindow* iniciarGLFW();
void cargarTexturasPlanetas();
void instanciaPlanetas(Shader astroShader, glm::mat4 projection, glm::mat4 model, glm::mat4 view);
void KeyCallback( GLFWwindow *window, int key, int scancode, int action, int mode );
void MouseCallback( GLFWwindow *window, double xPos, double yPos );
void DoMovement();
void Display();
void cargarInfinito(GLuint *skyboxVAO, GLuint *skyboxVBO, GLuint *texturaInfinito);
void dibujaModelo(Model modelo, Shader shader, glm::mat4 view, glm::mat4 projection, glm::mat4 model, glm::vec3 translate, glm::vec3 scale, glm::vec3 angulosRotacion);
void myTelescopio(glm::mat4 view, float distancia, float angulo, float distanciaObj, float anguloObj, Shader shader);
void dibujaInfinito(glm::mat4 model, glm::mat4 view, glm::mat4 projection);
void instanciaCiudadEstelar();
void iniciarShaders();
void controladorJuego();
bool temporizador(float *time, float initialTime, float deltaTime);
void controladorEnemigos();

vector<Misil*> balas_disparadas;
GLuint textura_balas;
glm::vec3 posicion_ciudad_estelar;
vector<Nave*> naves_enemigas;
// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

//Variables del juego
int puntuacion = 0;
float temperatura;
int velocidad_nave = 20;
//Skybox
extern GLfloat skyboxVertices[];
GLuint skyboxVAO, skyboxVBO, texturaInfinito;

// Camera
Camera  camera( glm::vec3(-3000.0f ) );
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
bool keys[1024];
bool firstMouse = true;
int tipo_camara = 0;
// Deltatime
GLfloat deltaTime = 0.0f;    // Tiempo entre dos frames consecutivos
GLfloat lastFrame = 0.0f;      // Tiempo del ultimo frame
float tiempo_actualizacion = 3.0f;

//Planetas del sistema galactico
Astro Sol(0* SCALE, 0, 0, 0, 0, 65, (GLchar*)"res/TEXTURAS/sol.png");
Astro planeta1(300* SCALE, 60, 20, 20, 0, 25, (GLchar*)"res/TEXTURAS/planeta1.jpg");
Astro planeta2(400* SCALE, -30, 10, 10, 0, 15, (GLchar*)"res/TEXTURAS/planeta2.jpg");
Astro planeta3(800* SCALE, 40, 50, 5, 0, 30, (GLchar*)"res/TEXTURAS/planeta3.jpg");
Astro planeta4(900* SCALE, 10, 230, 40, 0, 22, (GLchar*)"res/TEXTURAS/planeta4.jpg");
Astro planeta5(1200* SCALE, -100, 270, 16, 0, 32, (GLchar*)"res/TEXTURAS/planeta5.png");
Astro planeta6(1400* SCALE, 60, 310, 26, 0, 19, (GLchar*)"res/TEXTURAS/planeta6.png");
Astro planeta7(1700* SCALE, 20, 45, 24, 0, 27, (GLchar*)"res/TEXTURAS/planeta7.png");
Astro planeta8(1900* SCALE, -80, 20, 10, 0, 34, (GLchar*)"res/TEXTURAS/planeta8.png");
Astro planeta9(2100* SCALE, 25, 0, 0, 5, 29, (GLchar*)"res/TEXTURAS/planeta9.png");
Astro planeta10(2270* SCALE, -43, 100, 38, 0, 21, (GLchar*)"res/TEXTURAS/planeta10.png");
vector<Astro*> astros;
//Otros elementos de la escena
Model *ciudadEstelar;
Nave *miNave;
//Matrices de representacion
glm::mat4 model(1);
glm::mat4 view(1);
glm::mat4 projection(1);

//Shaders utilizados
Shader* miNaveShader  = 0;
Shader* ciudadEstelarShader = 0;
Shader* skyboxShader = 0;
Shader* astroShader = 0;
Shader *lightingShader = 0;

 int main( )
{
    //Funcion que inicializa GLFW
    GLFWwindow* window = iniciarGLFW();
    //SHADERS
    iniciarShaders();
    
    
    glActiveTexture( GL_TEXTURE0 );
    glUniform1i( glGetUniformLocation( astroShader->Program, "tex" ), 0 );
    
    cargarInfinito(&skyboxVAO, &skyboxVBO, &texturaInfinito);
    miNave = new Nave();
    ciudadEstelar = new Model((GLchar*)"res/models/ciudad/sc/serpentine city.obj");
    cargarTexturasPlanetas();
    

    textura_balas = CargarTexturas::cargarTexturas((GLchar*)"res/TEXTURAS/missile.jpg");
    
    posicion_ciudad_estelar =  glm::vec3(8000.f, 3000.0f, 5000.0f);
    srand (time(NULL));

    while (naves_enemigas.size() < NUMERO_ENEMIGOS) {
        naves_enemigas.push_back(new Nave((GLchar*)"res/models/Corvette-F3/Corvette-F3.obj", glm::vec3(rand() % 10000)));
    }
    //Bucle
    while ( !glfwWindowShouldClose( window ) )
    {
        // calculamos deltatime entre frames
        GLfloat currentFrame = glfwGetTime( );
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        // Comprobamos si ocurrio algun evento
        glfwPollEvents( );
        // Clear the colorbuffer
        glClearColor( 0.1f, 0.1f, 0.1f, 1.0f );
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        
        /**/
        controladorEnemigos();
        /**/
        Display();
        DoMovement();
        controladorJuego();
        glfwSwapBuffers(window);
    }
    glfwTerminate( );
    return 0;
}

void myTerceraPersona(glm::vec3 posicion, glm::vec3 angulo, float zoom){
    glm::vec3 dondeMira = glm::vec3(posicion.x + 10*cos(angulo.y*GRADOSTORADIANES), posicion.y, posicion.z+10*sin(angulo.y*GRADOSTORADIANES));
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    view = glm::mat4();
    view = glm::lookAt(
       glm::vec3(posicion.x - zoom * cos(angulo.y*GRADOSTORADIANES), posicion.y + 3.0f, posicion.z -zoom*sin(angulo.y*GRADOSTORADIANES) ), dondeMira, glm::vec3( 0.0f, 1.0f, 0.0f)
                       
                       );
    unsigned int viewLoc = glGetUniformLocation(miNave->getShader().Program, "view");
    glUniformMatrix4fv(viewLoc, 2, GL_FALSE, glm::value_ptr(view));
    
    projection = glm::mat4(1);
    projection = glm::perspective(glm::radians(60.0f), (float)SCREEN_WIDTH/(float)SCREEN_HEIGHT, 0.1f, 3000.0f * SCALE);
    unsigned int proyectionLoc = glGetUniformLocation(miNave->getShader().Program, "projection");
    glUniformMatrix4fv(proyectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
    

}

void Display(){
    
    if(tipo_camara == 1){
        view = glm::mat4();
        view = camera.GetViewMatrix();
        projection = glm::perspective(camera.GetZoom( ), (GLfloat)SCREEN_WIDTH/(GLfloat)SCREEN_HEIGHT, 0.1f, 3000.0f*SCALE);
    }else{
        myTerceraPersona(miNave->getPosicion(), miNave->getAngulos(), 4.0f);
    }

    dibujaInfinito(model, view, projection);
    miNave->dibujaModelo(view, projection, model);
    
    miNave->incrementarPosicion(glm::vec3(miNave->getVelocidad()*cos(miNave->getAnguloGiro()*GRADOSTORADIANES),0,  miNave->getVelocidad()*sin(miNave->getAnguloGiro()*GRADOSTORADIANES)));
    
    
    
    for (int i = 0; i < balas_disparadas.size(); i++) {
        if(glm::distance(miNave->getPosicion(), balas_disparadas[i]->getPosicion()) < 15000){
            balas_disparadas[i]->disparar(view, projection, model);
            balas_disparadas[i]->setAnguloGiro(miNave->getAnguloGiro());
            balas_disparadas[i]->incrementarPosicion(glm::vec3(balas_disparadas[i]->getVelocidad()*cos(miNave->getAnguloGiro()*GRADOSTORADIANES),0,  balas_disparadas[i]->getVelocidad()*sin(miNave->getAnguloGiro()*GRADOSTORADIANES)));
        }else{
            balas_disparadas.erase(balas_disparadas.begin() + i);
        }
    }
    
    instanciaCiudadEstelar();
    
    
    instanciaPlanetas(*astroShader,projection, model,view);

}


void controladorEnemigos(){

    for (int i = 0; i< naves_enemigas.size(); i++) {
        if (naves_enemigas[i]->detectaColisiones(posicion_ciudad_estelar, 300.f) || glm::distance(Sol.getPosicion(), naves_enemigas[i]->getPosicion()) > 45000) {
            naves_enemigas.erase(naves_enemigas.begin() + i);
            std::cout << "UNA NAVE ENEMIGA HA ALCANZADO LA CIUDAD ESTELAR" << std::endl;
            std::cout << "¡¡¡HAS PERDIDO!!!" << std::endl;
            puntuacion = 0;
            miNave->setPosicion(glm::vec3(2000));
            temperatura = 13;
            miNave->setVelocidad(0);
            continue;
        }
        
        for (int j = 0; j < balas_disparadas.size(); j++) {
            //if (naves_enemigas[i]->detectaColisiones(balas_disparadas[j]->getPosicion(), 10)) {
            if (balas_disparadas[j]->detectaColisiones(naves_enemigas[i]->getPosicion(), naves_enemigas[i]->getRadio())) {
                naves_enemigas.erase(naves_enemigas.begin() + i);
                puntuacion++;
                std::cout << " HAS DERRIBADO UNA NAVE ENEMIGA " << std::endl;
                std::cout << " -- PUNTUACION " << puntuacion << " --" << std::endl;
                continue;
            }
        }
        
        
        naves_enemigas[i]->setVelocidad(rand() % 40 + 10);
        naves_enemigas[i]->dibujaModelo(view, projection, model);
        naves_enemigas[i]->atacar(view, projection, model, posicion_ciudad_estelar, deltaTime);
    }
}


void controladorJuego(){
    float distancia_sol = glm::distance(miNave->getPosicion(), glm::vec3(0.0f));
    float temperatura;
    
    srand(glfwGetTime());
    if(distancia_sol < 3000){
        temperatura = rand() % 90 + 45;
    }else{
        temperatura = rand() % 40 + 9;
    }
    if (temporizador(&tiempo_actualizacion, 3.0f, deltaTime)) {
        std::cout << "Temperatura: " << temperatura << "ºC" << std:: endl;
    }
    
    if(temperatura > 90){
        std::cout << "HAS PERDIDO, TU NAVE SE HA FUNDIDO" << std::endl;
        puntuacion = 0;
        miNave->setPosicion(glm::vec3(2000));
        temperatura = 13;
        miNave->setVelocidad(0);
    }
    
    for(int i = 0; i < astros.size(); i++){
        
        for (int j=0; j< balas_disparadas.size(); j++) {
            if(balas_disparadas[j]->detectaColisiones(astros[i]->posicion, astros[i]->getRadio())){
                balas_disparadas.erase(balas_disparadas.begin() + j);
            }
        }
        
        if(miNave->detectaColisiones(astros[i]->posicion, astros[i]->getRadio())){
            std::cout << "¡¡¡HAS CHOCADO CONTRA UN PLANETA!!!" << std::endl;
            std::cout << "¡¡¡HAS PERDIDO!!!" << std::endl;
            miNave->setPosicion(glm::vec3(2000.f));
            puntuacion = 0;
            temperatura = 13;
            miNave->setVelocidad(0);
        }
    }
    
    if (puntuacion == NUMERO_ENEMIGOS) {
        std::cout << "ENHORABUENA, LA CIUDAD ESTA SEGURA" << std::endl;
        puntuacion = 0;
    }

}




void DoMovement()
{
    miNave->setVelocidad(velocidad_nave);

    if ( keys[GLFW_KEY_W]) {
        miNave->incrementarPosicion(glm::vec3(0.0f, 45.0f, 0.0f ));
        //miNave->ascenso();
        
    }
    if(keys[GLFW_KEY_S]){
        miNave->incrementarPosicion(glm::vec3(0.0f, -45.0f, 0.0f ));
        //miNave->descenso();
    }
    if(keys[GLFW_KEY_A]){
        miNave->incrementarAngulo(-1.6);
        miNave->incrementarAnguloZ(0);
    }
    if (keys[GLFW_KEY_D]) {
        miNave->incrementarAngulo(1.6f);
        miNave->incrementarAnguloZ(1);
    }
    if ( keys[GLFW_KEY_UP] ){
        camera.ProcessKeyboard( FORWARD, deltaTime );
        //miNave->incrementarVelocidad(.3 );
    }
    if (keys[GLFW_KEY_DOWN] ){
        camera.ProcessKeyboard( BACKWARD, deltaTime );
        //miNave->incrementarVelocidad(-.3 );
    }
    if ( keys[GLFW_KEY_LEFT] ){
        camera.ProcessKeyboard( LEFT, deltaTime );
    }
    if ( keys[GLFW_KEY_RIGHT] ){
        camera.ProcessKeyboard( RIGHT, deltaTime );
    }
    if (keys[GLFW_KEY_C]) {
        switch (tipo_camara) {
            case 0:
                tipo_camara = 1;
                break;
            case 1:
                tipo_camara = 0;
                break;
        }
    }
}

// Is called whenever a key is pressed/released via GLFW
void KeyCallback( GLFWwindow *window, int key, int scancode, int action, int mode )
{
    std::string velocidadStr = "Velocidad: ";
    if ( GLFW_KEY_ESCAPE == key && GLFW_PRESS == action )
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
    
    
    if ( key >= 0 && key < 1024 )
    {
        keys[key] = true;

        if ( action == GLFW_PRESS )
        {
            if(key == GLFW_KEY_I){
                std::cout << "Posicion: " << miNave->getPosicion().z << " N\t" << miNave->getPosicion().x << " W\t" << miNave->getPosicion().y << "Z" << std::endl;
            }
            
            if (key == GLFW_KEY_SPACE) {
                switch (velocidad_nave) {
                    case 0:
                        velocidad_nave = 20;
                        break;
                    case 20:
                        velocidad_nave = 40;
                        break;
                    case 40:
                        velocidad_nave = 80;
                        break;
                    case 80:
                        velocidadStr ="Cambiando a modo galactico. MAXIMA VELOCIDAD: ";
                        velocidad_nave = 200;
                        break;
                    case 200:
                        velocidad_nave = 20;
                        break;
                    default:
                        break;
                }
                std::cout << velocidadStr << velocidad_nave << std::endl;
            }
            if (key == GLFW_KEY_ENTER) {
                if(balas_disparadas.size() < 20){
                    balas_disparadas.push_back(new Misil(textura_balas , miNave->getPosicion()));
                }
            }
        }
        else if ( action == GLFW_RELEASE )
        {
            keys[key] = false;
        }
    }
}

void MouseCallback( GLFWwindow *window, double xPos, double yPos )
{
    if ( firstMouse )
    {
        lastX = xPos;
        lastY = yPos;
        firstMouse = false;
    }
    
    GLfloat xOffset = xPos - lastX;
    GLfloat yOffset = lastY - yPos;  // Reversed since y-coordinates go from bottom to left
    
    lastX = xPos;
    lastY = yPos;
    camera.ProcessMouseMovement( xOffset, yOffset );
}


void instanciaPlanetas(Shader astroShader, glm::mat4 projection, glm::mat4 model, glm::mat4 view){
    astroShader.Use();
    GLuint modelLoc =glGetUniformLocation(astroShader.Program, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(glGetUniformLocation(astroShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(glGetUniformLocation(astroShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
    // Luces de los astros
    unsigned int lightPosLoc = glGetUniformLocation(astroShader.Program, "lightPos");
    glUniform3f(lightPosLoc, 0, 0, 0);
    unsigned int colorLoc = glGetUniformLocation(astroShader.Program, "objectColor");
    glUniform3f(colorLoc, 1.0, 1.0f, 1.0f);
    lightLoc = glGetUniformLocation(astroShader.Program, "lightColor");
    glUniform3f(lightLoc, 1.0, 1.0, 1.0f);
    //Dibujamos astros
    Sol.dibujarAstro(model, astroShader, modelLoc, deltaTime);
    planeta1.dibujarAstro(model, astroShader, modelLoc, deltaTime);
    planeta2.dibujarAstro(model, astroShader, modelLoc, deltaTime);
    planeta3.dibujarAstro(model, astroShader, modelLoc, deltaTime);
    planeta4.dibujarAstro(model, astroShader, modelLoc, deltaTime);
    planeta5.dibujarAstro(model, astroShader, modelLoc, deltaTime);
    planeta6.dibujarAstro(model, astroShader, modelLoc, deltaTime);
    planeta7.dibujarAstro(model, astroShader, modelLoc, deltaTime);
    planeta8.dibujarAstro(model, astroShader, modelLoc, deltaTime);
    planeta9.dibujarAstro(model, astroShader, modelLoc, deltaTime);
    planeta10.dibujarAstro(model, astroShader, modelLoc, deltaTime);
}


void dibujaModelo(Model modelo, Shader shader, glm::mat4 view, glm::mat4 projection, glm::mat4 model, glm::vec3 translate, glm::vec3 scale, glm::vec3 angulosRotacion){
    shader.Use();
    glUniformMatrix4fv( glGetUniformLocation( shader.Program, "projection" ), 1, GL_FALSE, glm::value_ptr( projection ) );
    glUniformMatrix4fv( glGetUniformLocation( shader.Program, "view" ), 1, GL_FALSE, glm::value_ptr( view ) );
    
    // Draw the loaded model
    model = glm::mat4(1);
    model = glm::translate( model, translate ); // Translate it down a bit so it's at the center of the scene
    model = glm::rotate(model, angulosRotacion.x , glm::vec3(1.0f,0.0f,0.0f));
    model = glm::rotate(model, angulosRotacion.y , glm::vec3(0.0f,1.0f,0.0f));
    model = glm::rotate(model, angulosRotacion.z , glm::vec3(0.0f,0.0f,1.0f));
    model = glm::scale( model, scale);    // It's a bit too big for our scene, so scale it down
    glUniformMatrix4fv( glGetUniformLocation( shader.Program, "model" ), 1, GL_FALSE, glm::value_ptr( model ) );
    modelo.Draw( shader );
}

void cargarInfinito(GLuint *skyboxVAO, GLuint *skyboxVBO, GLuint *texturaInfinito){
    glGenVertexArrays(1, skyboxVAO);
    glGenBuffers(1, skyboxVBO);
    glBindVertexArray(*skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, *skyboxVBO);
    glBufferData( GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat),(GLvoid*)0);
    
    vector<const GLchar*> faces;
    faces.push_back("res/images/skybox/right.tga");
    faces.push_back("res/images/skybox/left.tga");
    faces.push_back("res/images/skybox/up.tga");
    faces.push_back("res/images/skybox/down.tga");
    faces.push_back("res/images/skybox/back.tga");
    faces.push_back("res/images/skybox/front.tga");
    *texturaInfinito = CargarTexturas::LoadCubemap(faces);
}

void dibujaInfinito(glm::mat4 model, glm::mat4 view, glm::mat4 projection){
    glDepthFunc(GL_LEQUAL);
    skyboxShader->Use();
    if (tipo_camara == 0) {
        view = glm::mat4(glm::mat3(camera.GetViewMatrix()));
    }else{
        view = glm::mat4(glm::mat3(view));;
    }
    
    glUniformMatrix4fv(glGetUniformLocation(skyboxShader->Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(skyboxShader->Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
    glBindVertexArray(skyboxVAO);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texturaInfinito);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
    glDepthFunc(GL_LESS);

}

void instanciaCiudadEstelar(){
    glm::vec3 dondeMira = glm::vec3(miNave->getPosicion().x + 100*cos(miNave->getAnguloGiro() *GRADOSTORADIANES), miNave->getPosicion().y, miNave->getPosicion().z+100*sin(miNave->getAnguloGiro()*GRADOSTORADIANES));
    
    ciudadEstelarShader->Use();
    glUniformMatrix4fv( glGetUniformLocation( ciudadEstelarShader->Program, "projection" ), 1, GL_FALSE, glm::value_ptr( projection ) );
    glUniformMatrix4fv( glGetUniformLocation( ciudadEstelarShader->Program, "view" ), 1, GL_FALSE, glm::value_ptr( view ) );
    model = glm::mat4(1);
    model = glm::translate( model, posicion_ciudad_estelar );
    model = glm::scale( model, glm::vec3(15.0f));
    glUniformMatrix4fv( glGetUniformLocation( ciudadEstelarShader->Program, "model" ), 1, GL_FALSE, glm::value_ptr( model ) );
    
    GLint lightPosLoc = glGetUniformLocation( ciudadEstelarShader->Program, "light.position" );
    GLint lightSpotdirLoc = glGetUniformLocation( ciudadEstelarShader->Program, "light.direction" );
    GLint lightSpotCutOffLoc = glGetUniformLocation( ciudadEstelarShader->Program, "light.cutOff" );
    GLint lightSpotOuterCutOffLoc = glGetUniformLocation( ciudadEstelarShader->Program, "light.outerCutOff" );
    GLint viewPosLoc = glGetUniformLocation( ciudadEstelarShader->Program, "viewPos" );
    
    lightPosLoc = glGetUniformLocation(ciudadEstelarShader->Program, "lightPos");
    glUniform3f(lightPosLoc, 0, 0, 0);
    unsigned int colorLoc = glGetUniformLocation(ciudadEstelarShader->Program, "objectColor");
    glUniform3f(colorLoc, 1.0, 1.0f, 1.0f);
    lightLoc = glGetUniformLocation(ciudadEstelarShader->Program, "lightColor");
    glUniform3f(lightLoc, 1.0, 1.0, 1.0f);
    
    glUniform3f( lightPosLoc, miNave->getPosicion().x, miNave->getPosicion().y, miNave->getPosicion().z);
    glUniform3f( lightSpotdirLoc,  dondeMira.x, dondeMira.y, dondeMira.z);
    glUniform1f( lightSpotCutOffLoc, glm::cos( glm::radians( 12.5f ) ) );
    glUniform1f( lightSpotOuterCutOffLoc, glm::cos( glm::radians( 17.5f ) ) );
    glUniform3f( viewPosLoc,  miNave->getPosicion().x, miNave->getPosicion().y, miNave->getPosicion().z);
    // Set lights properties
    glUniform3f( glGetUniformLocation( ciudadEstelarShader->Program, "light.ambient" ),   0.1f, 0.1f, 0.1f );
    glUniform3f( glGetUniformLocation( ciudadEstelarShader->Program, "light.diffuse" ), 0.8f, 0.8f, 0.8f );
    glUniform3f( glGetUniformLocation( ciudadEstelarShader->Program, "light.specular" ), 1.0f, 1.0f, 1.0f );
    glUniform1f( glGetUniformLocation( ciudadEstelarShader->Program, "light.constant" ), 1.0f );
    glUniform1f( glGetUniformLocation( ciudadEstelarShader->Program, "light.linear" ), 0.09 );
    glUniform1f( glGetUniformLocation( ciudadEstelarShader->Program, "light.quadratic" ), 0.032 );
    // Set material properties
    glUniform1f( glGetUniformLocation( ciudadEstelarShader->Program, "material.shininess" ), 32.0f );
    ciudadEstelar->Draw( *ciudadEstelarShader );
    
}


GLFWwindow* iniciarGLFW(){
    // Init GLFW
    glfwInit( );
    // Set all the required options for GLFW
    glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
    glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
    glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
    glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );
    glfwWindowHint( GLFW_RESIZABLE, GL_FALSE );
    
    // Create a GLFWwindow object that we can use for GLFW's functions
    GLFWwindow* window = glfwCreateWindow( WIDTH, HEIGHT, "PROYECTO SNASA", nullptr, nullptr );
    
    if ( nullptr == window )
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate( );
        exit(0);
        return NULL;
    }
    
    glfwMakeContextCurrent( window );
    
    glfwGetFramebufferSize( window, &SCREEN_WIDTH, &SCREEN_HEIGHT );
    
    // Set the required callback functions
    glfwSetKeyCallback( window, KeyCallback );
    glfwSetCursorPosCallback( window, MouseCallback );
    
    // GLFW Options
    glfwSetInputMode( window, GLFW_CURSOR, GLFW_CURSOR_DISABLED );
    // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
    glewExperimental = GL_TRUE;
    // Initialize GLEW to setup the OpenGL Function pointers
    if ( GLEW_OK != glewInit( ) )
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return NULL;
    }
    
    // Define the viewport dimensions
    glViewport( 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT );
    
    // OpenGL options
    glEnable( GL_DEPTH_TEST );
    return window;
}


void iniciarShaders(){
    ciudadEstelarShader = new Shader("res/shaders/ciudadEstelarShader.vert", "res/shaders/ciudadEstelarShader.frag");
    skyboxShader = new Shader("res/shaders/skybox.vs", "res/shaders/skybox.frag");
    astroShader = new Shader("res/shaders/shader.vert", "res/shaders/shader.frag");
    lightingShader = new Shader("res/shaders/lighting.vs", "res/shaders/lighting.frag" );

}


bool temporizador(float *time, float initialTime, float deltaTime){
    *time -= deltaTime;
    if(*time <= 0){
        *time = initialTime;
        return true;
    }
    return false;
}

void cargarTexturasPlanetas(){
    glUniform3f(lightLoc, .8, 0.8, 0.8);
    Sol.cargarTexturaAstro();
    glUniform3f(lightLoc, .1, 0.1, 0.1);
    planeta1.cargarTexturaAstro();
    planeta2.cargarTexturaAstro();
    planeta3.cargarTexturaAstro();
    planeta4.cargarTexturaAstro();
    planeta5.cargarTexturaAstro();
    planeta6.cargarTexturaAstro();
    planeta7.cargarTexturaAstro();
    planeta8.cargarTexturaAstro();
    planeta9.cargarTexturaAstro();
    planeta10.cargarTexturaAstro();
    astros.push_back(&planeta1);
    astros.push_back(&planeta2);
    astros.push_back(&planeta3);
    astros.push_back(&planeta4);
    astros.push_back(&planeta5);
    astros.push_back(&planeta6);
    astros.push_back(&planeta7);
    astros.push_back(&planeta8);
    astros.push_back(&planeta9);
    astros.push_back(&planeta10);
    astros.push_back(&Sol);
}
