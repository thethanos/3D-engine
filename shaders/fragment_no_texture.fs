#version 330 core

uniform sampler2D texture_diffuse0;

uniform vec3 light_pos; 
uniform vec3 light_color;
uniform vec3 object_color;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

out vec4 fragColor;

void main()
{
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light_pos - FragPos);

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * light_color;

    float ambientStrength = 0.2;
    vec3 ambient = ambientStrength * light_color;

    vec3 result = (ambient + diffuse) * object_color;
    fragColor = vec4(result, 1.0);
}