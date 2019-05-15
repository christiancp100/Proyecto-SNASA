#version 330 core

in vec2 TexCoords;

out vec4 color;

in vec3 Normal;
in vec3 FragPos;
in vec2 Texcoord;

uniform sampler2D texture_diffuse;

uniform vec3 viewPos;
uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 objectColor;

void main(){
    float ambientI = .6;
    vec3 ambient = ambientI  * lightColor;
    
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    
    float specularStrength = .5;
    vec3 viewDir = normalize(viewPos - FragPos);
    //diferencia angulos entre lightdir y reflect dir < 15 radianes
    vec3 reflectDir = reflect(-lightDir, norm);
    
    float spec = pow(max(dot(viewDir, reflectDir), 1.0), 128);
    vec3 specular = specularStrength * spec * lightColor;
    
    vec3 result = (ambient + diffuse + specular) * objectColor;
    
    color = texture(texture_diffuse, TexCoords) * vec4(result, 1);
}
