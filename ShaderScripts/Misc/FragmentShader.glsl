#version 330 core
struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct Light {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight{
    vec3 position;

    Light light;

    float constant;
    float linear;
    float quadratic;
};

struct DirectionalLight{
    vec3 direction;

    Light light;
};

struct SpotLight{
    vec3 position;
    vec3 direction;
    float cutoff;

    Light light;
};

uniform Material material;
uniform DirectionalLight directionallight;
uniform PointLight pointlight;
uniform SpotLight spotlight;

uniform int lightType;

out vec4 FragColor;

//Light stuff
in vec3 Normal;
in vec3 FragPos;

uniform vec3 viewPos;
uniform vec3 lightPos;
uniform vec3 lightColor;

in vec2 TexCoord;

void main() {

    Light light;
    vec3 lightDir;
    if(lightType == 0){
        light = pointlight.light;
        lightDir = normalize(lightPos - FragPos);
    }
    else if(lightType == 1){
        light = spotlight.light;
        lightDir = normalize(lightPos - FragPos);
    }
    else if(lightType == 2){
        light = directionallight.light;
        lightDir = normalize(-directionallight.direction);
    }

    //Ambient
    vec3 ambient = light.ambient * texture(material.diffuse, TexCoord).rgb;

    //Diffuse
    vec3 norm = normalize(Normal);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoord).rgb;

    //Specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * texture(material.specular, TexCoord).rgb;

    FragColor = vec4(ambient + diffuse + specular, 1.0);
}