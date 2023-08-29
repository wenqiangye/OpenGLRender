#version 330 core
out vec4 FragColor;


uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform float shininess;

struct DirLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform DirLight dirLight;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);

struct PointLight {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};


#define NR_POINT_LIGHTS 2
uniform int lightcnt;

uniform PointLight pointLights[NR_POINT_LIGHTS];
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

struct SpotLight {
    vec3 position;
    vec3 direction;

    float cosPhyInner;
    float cosPhyOuter;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

uniform SpotLight spotLight;

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
in vec3 FragPos;
in vec2 TexCoords;
in vec3 Normal;
uniform vec3 viewPos;

void main()
{
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos-FragPos);
    vec3 result = vec3(0.0f,0.0f,0.0f);
    for(int i=0;i<lightcnt;++i) {
        result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);
    }
    FragColor = vec4(result,1.0f);
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);

    float diff = max(dot(normal,lightDir),0.0);

    vec3 reflectDir = reflect(-lightDir,normal);
    float spec = pow(max(dot(viewDir,reflectDir),0.0),shininess);

    vec3 ambient = light.ambient * texture(texture_diffuse1,TexCoords).rgb;
    vec3 diffuse = light.diffuse * diff *texture(texture_diffuse1,TexCoords).rgb;
    vec3 specular = light.specular * spec * texture(texture_specular1,TexCoords).rgb;

    return (ambient+diffuse+specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position-fragPos);

    /* float theta = dot(lightDir,normalize(-light.direction)); */
    /* float epsilon = light.cutOff - light.outerCutOff; */
    /* float intensity = clamp((theta-light.outerCutOff)/epsilon,0.0,1.0); */

    float diff = max(dot(normal,lightDir),0.0);
    vec3 reflectDir = reflect(-lightDir,normal);
    float spec = pow(max(dot(viewDir,reflectDir),0.0), shininess);

    // attenuation
    float distance    = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    vec3 ambient = light.ambient * texture(texture_diffuse1,TexCoords).rgb;
    vec3 diffuse = light.diffuse * diff *texture(texture_diffuse1,TexCoords).rgb;
    vec3 specular = light.specular * spec * texture(texture_specular1,TexCoords).rgb;


    // ambient  *= attenuation; // remove attenuation from ambient, as otherwise at large distances the light would be darker inside than outside the spotlight due the ambient term in the else branch
    diffuse   *= attenuation;
    specular *= attenuation;
    ambient *= attenuation;

    /* diffuse  *= intensity; */
    /* specular *= intensity; */
    vec3 result = ambient + diffuse + specular;
    return result;
}

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
   
    vec3 lightDir = normalize(light.position-fragPos);

    float theta = dot(lightDir,normalize(-light.direction));
    float epsilon = light.cosPhyInner - light.cosPhyOuter;
    float intensity = clamp((theta-light.cosPhyOuter)/epsilon,0.0,1.0);

    float diff = max(dot(normal,lightDir),0.0);
    vec3 reflectDir = reflect(-lightDir,normal);
    float spec = pow(max(dot(viewDir,reflectDir),0.0),shininess);

    // attenuation
    float distance    = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    
    vec3 ambient = light.ambient * texture(texture_diffuse1,TexCoords).rgb;
    vec3 diffuse = light.diffuse * diff *texture(texture_diffuse1,TexCoords).rgb;
    vec3 specular = light.specular * spec * texture(texture_specular1,TexCoords).rgb;


    // ambient  *= attenuation; // remove attenuation from ambient, as otherwise at large distances the light would be darker inside than outside the spotlight due the ambient term in the else branch
    diffuse   *= attenuation;
    specular *= attenuation;
    ambient *= attenuation;

    diffuse  *= intensity;
    specular *= intensity;
    vec3 result = ambient + diffuse + specular;
    return result;
}