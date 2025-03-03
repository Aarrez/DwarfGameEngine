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

vec4 ColorRed = vec4(1, 0, 0, 1);
vec4 ColorGreen = vec4(0, 1, 0, 1);
vec4 ColorBlue = vec4(0, 0, 1, 1);

vec3 CalcDirLight(Light light, vec3 normal, vec3 viewDir){

    return vec3(1, 1, 1);
}

vec3 CalcPointLight(Light light, vec3 normal, vec3 viewDir){
    return vec3(1, 1, 1);
}

vec3 CalcSpotLight(Light light, vec3 normal, vec3 viewDir){
    float theta, attenuation;
    vec3 lightDir = normalize(lightPos - FragPos);
    theta = dot(lightDir, normalize(-spotlight.direction));
    if(theta > spotlight.cutoff){

    }
    return vec3(1, 1, 1);
}

void main() {

    Light light;
    vec3 lightDir;
    float lightDistance;
    float attenuation;
    float theta;

    if(lightType == 0){
        light = pointlight.light;
        lightDir = normalize(lightPos - FragPos);
        lightDistance = length(lightPos - FragPos);
        attenuation = 1.0 / (pointlight.constant + pointlight.linear * lightDistance +
        pointlight.quadratic * (lightDistance * lightDistance));
    }
    else if(lightType == 1){
        light = spotlight.light;
        lightDir = normalize(lightPos - FragPos);
        theta = dot(lightDir, normalize(-spotlight.direction));
    }
    else if(lightType == 2){
        light = directionallight.light;
        lightDir = normalize(-directionallight.direction);
    }
    if(theta > spotlight.cutoff || lightType != 1){
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

        if(lightType != 2){
            if(lightType == 2){
                ambient *= attenuation;
            }
            diffuse *= attenuation;
            specular *= attenuation;
        }

        FragColor = vec4(ambient + diffuse + specular, 1.0);
    }
    else{
        FragColor = ColorRed;
    }

}