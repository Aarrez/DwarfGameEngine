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

    float constant;
    float linear;
    float quadratic;

    Light light;
};

uniform Material material;

uniform int nr_dirlights;
uniform DirectionalLight directionallight;

uniform int nr_pointlights;
#define NR_POINT_LIGHTS 16
uniform PointLight pointlight[NR_POINT_LIGHTS];

uniform int nr_spotlights;
#define NR_SPOT_LIGHT 16
uniform SpotLight spotlight[NR_SPOT_LIGHT];

out vec4 FragColor;

//Light stuff
in vec3 Normal;
in vec3 FragPos;
in vec4 FragPosLightSpace;

uniform vec3 viewPos;
uniform vec3 lightPos;
uniform vec3 lightColor;

in vec2 TexCoord;

uniform sampler2D shadowMap;

float ShadowCalculation(vec4 fragPosLightSpace, vec3 normal, vec3 lightDir);
vec3 CalcDirLight(DirectionalLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos ,vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main() {

    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 result = vec3(0.0);
    if(nr_dirlights > 0)
        result = CalcDirLight(directionallight, norm, viewDir);

    for(int i = 0; i < nr_pointlights; i++){
        result += CalcPointLight(pointlight[i], norm, FragPos, viewDir);
    }

    for(int j = 0; j < nr_spotlights; j++){
        result += CalcSpotLight(spotlight[j], norm, FragPos, viewDir);
    }

    FragColor = vec4(result, 1.0);
}

float ShadowCalculation(vec4 fragPosLightSpace, vec3 normal, vec3 lightDir){
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;
    float closestDepth = texture(shadowMap, projCoords.xy).r;
    float currentDepth  = projCoords.z;
    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);
    float shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;
    return shadow;
}

vec3 CalcDirLight(DirectionalLight light, vec3 normal, vec3 viewDir){
    vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(normal, lightDir), 0.0);

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    vec3 ambient = light.light.ambient * vec3(texture(material.diffuse, TexCoord));
    vec3 diffuse = light.light.diffuse * diff * vec3(texture(material.diffuse, TexCoord));
    vec3 specular = light.light.specular * spec * vec3(texture(material.specular, TexCoord));
    float shadow = ShadowCalculation(FragPosLightSpace, normal, lightDir);
    return (ambient + (1.0 - shadow) * (diffuse + specular));
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos ,vec3 viewDir){
    vec3 lightDir = normalize(light.position - fragPos);

    float diff = max(dot(normal, lightDir), 0.0);

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    float dist = length(lightPos - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * dist +
    light.quadratic * (dist * dist));

    vec3 ambient = light.light.ambient * vec3(texture(material.diffuse, TexCoord));
    vec3 diffuse = light.light.diffuse * diff * vec3(texture(material.diffuse, TexCoord));
    vec3 specular = light.light.specular * spec * vec3(texture(material.specular, TexCoord));
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    float shadow = ShadowCalculation(FragPosLightSpace, normal, lightDir);
    return (ambient + (1.0 - shadow) * (diffuse + specular));
}

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir){

    vec3 lightDir = normalize(light.position - fragPos);

    float diff = max(dot(normal, lightDir), 0.0);

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    float lightDistance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * lightDistance +
    light.quadratic * (lightDistance * lightDistance));


    vec3 ambient = light.light.ambient * vec3(texture(material.diffuse, TexCoord));
    vec3 diffuse = light.light.diffuse * diff * vec3(texture(material.diffuse, TexCoord));
    vec3 specular = light.light.specular * spec * vec3(texture(material.specular, TexCoord));

    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutoff - light.outercutoff;
    float intensity = clamp((theta - light.outercutoff)/ epsilon, 0.0, 1.0);

    ambient *= intensity * attenuation;
    diffuse *= intensity * attenuation;
    specular *= intensity * attenuation;
    float shadow = ShadowCalculation(FragPosLightSpace, normal, lightDir);
    return (ambient + (1.0 - shadow) * (diffuse + specular));
}

