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
    vec3 postion;
    float constant;
    float linear;
    float quadratic;

    Light light;
};

struct DirectionalLight{
    vec3 direction;

    Light light;
};

struct SpotLight{
    vec3 position;
    vec3 direction;
    float cutoff;
    float outercutoff;

    Light light;
};

uniform Material material;

uniform int nr_dirlights;
uniform DirectionalLight directionallight;

uniform int nr_pointlights;
#define NR_POINT_LIGHTS 16
uniform PointLight pointlight[NR_POINT_LIGHTS];

uniform int nr_spotlights;
uniform SpotLight spotlight;

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
    vec3 lightDir = normalize(-directionallight.direction);
    vec3 ambient = light.ambient * texture(material.diffuse, TexCoord).rgb;
    float diff = max(dot(normal, lightDir), 0.0);

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0,0), material.shininess);

    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoord));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoord));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoord));
    return (ambient + diffuse + specular);
}

vec3 CalcPointLight(Light light, vec3 normal, vec3 viewDir){
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(normal, lightDir));
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    float dist = length(lightPos - FragPos);
    float attenuation = 1.0 / (pointlight.constant + pointlight.linear * dist +
    pointlight.quadratic * (dist * dist));

    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoord));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoord));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoord));
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

vec3 CalcSpotLight(Light light, vec3 normal, vec3 viewDir){

    vec3 lightDir = normalize(lightPos - FragPos);
    float theta = dot(lightDir, normalize(-spotlight.direction));

    if(theta > spotlight.cutoff){

        vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoord));

        vec3 norm = normalize(Normal);
        float diff = max(dot(norm, lightDir));
        vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoord));

        vec3 viewDir = normalize(viewPos - FragPos);
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
        vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoord));

        float lightDist = length(lightPos - FragPos);

        float epsilon = spotlight.cutoff - spotlight.outercutoff;
        float intensity = clamp((theta - spotlight.outercutoff)/ epsilon, 0.0, 1.0);

        diffuse *= intensity;
        specular *= intensity;

        return (specular + diffuse + ambient);
    }
    return vec3(0, 0, 0);
}

void main() {

    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 result = vec3(0.0);
    if(nr_dirlights > 0){
        result = CalcDirLights(directionallight.light, norm, viewDir);
    }

    for(int i = 0; i < nr_pointlights; i++){
        result += CalcPointLights(pointlight[i].light, norm, viewDir);
    }

    FragColor = vec4(result, 1.0);




}