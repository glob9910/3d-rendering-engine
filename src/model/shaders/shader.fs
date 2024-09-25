#version 330 core

in vec2 TexCoord;
in vec3 FragPos;
in vec3 Normal;

out vec4 FragColor;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Material material;
uniform Light light;

uniform sampler2D texture1;
uniform sampler2D texture2;

uniform vec3 viewPos;

void main()
{
    /* FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.2); */
    
    // ambient light
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoord));
    
    // diffuse light
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoord));
    
    // specular light
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoord));

    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}