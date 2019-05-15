#version 330 core

struct Light
{
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;
    
    float constant;
    float linear;
    float quadratic;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;
in vec2 Texcoord;

uniform sampler2D tex;
//uniform sampler2D tex2;

uniform vec3 viewPos;
uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 objectColor;


uniform Light light;

void main(){
    float ambientI = .3;
    vec3 ambient = ambientI  * lightColor;

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    float specularStrength = .3;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);

    float spec = pow(max(dot(viewDir, reflectDir), 1.0), 128);
    vec3 specular = specularStrength * spec * lightColor;
    //Luz focal
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = (light.cutOff - light.outerCutOff);
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    diffuse  *= intensity;
    specular *= intensity;

    
    vec3 result = (ambient + diffuse + specular) * objectColor;

    //vec3 result = objectColor;
    
    //FragColor = mix(texture(tex, Texcoord),texture(tex2, Texcoord), 0.6) * vec4(result, 1);
    FragColor = texture(tex, Texcoord) * vec4(result, 1);
}
