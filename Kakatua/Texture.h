#pragma once

#define GLEW_STATIC
#include <GL/glew.h>

#include <vector>
#include <iostream>
#include "SOIL2/SOIL2.h"
#include <vector>

class CargarTexturas
{
public:
    static GLuint cargarTexturas(const GLchar *path){
        
            GLuint textureID;
            glGenTextures( 1, &textureID );
            
            int imageWidth, imageHeight;
            
            unsigned char *image = SOIL_load_image( path, &imageWidth, &imageHeight, 0, SOIL_LOAD_RGB );
            if(!image){
                std::cout << "ERROR:IMAGE:NOT:LOADING" << std::endl;
                return 0;
            }
            // Assign texture to ID
            glBindTexture( GL_TEXTURE_2D, textureID );
            glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image );
            glGenerateMipmap( GL_TEXTURE_2D );
            
            // Parameters
            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
            glBindTexture( GL_TEXTURE_2D,  0);
            
            SOIL_free_image_data( image );
            return textureID;
    }
    
    static GLuint LoadCubemap( std::vector<const GLchar * > faces)
    {
        GLuint textureID;
        glGenTextures( 1, &textureID );
        
        int imageWidth, imageHeight;
        unsigned char *image;
        
        glBindTexture( GL_TEXTURE_CUBE_MAP, textureID );
        
        for ( GLuint i = 0; i < faces.size( ); i++ )
        {
            image = SOIL_load_image( faces[i], &imageWidth, &imageHeight, 0, SOIL_LOAD_RGB );
            glTexImage2D( GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image );
            SOIL_free_image_data( image );
        }
        glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
        glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
        glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
        glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
        glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE );
        glBindTexture( GL_TEXTURE_CUBE_MAP, 0);
        
        return textureID;
    }
};

