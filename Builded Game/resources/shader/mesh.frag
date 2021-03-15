#version 450

in vec2 uv;
in vec3 Normal;
in vec3 Position;
in vec3 lightPos;

out vec3 color;

uniform sampler2D myTexture;

void main() {
    //ambient
    float ambientStrength = 0.2;
    vec3 ambient = vec3(ambientStrength);

	// diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - Position);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = vec3(diff);

    //Specular
    float specularStrength = 2.0;
    vec3 viewDir = normalize(-Position);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 128);
    vec3 specular = vec3(specularStrength * spec);  

    color = (specular + diffuse + ambient) * texture(myTexture, uv).rgb;
}
