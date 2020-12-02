#version 450

in vec2 uv;
in vec3 Normal;
in vec3 Position;

out vec3 color;

uniform sampler2D myTexture;
uniform mat4 Camera;

void main() {
    vec3 lightPos = vec3(2.0, 1.0, 2.0);

    //ambient
    float ambientStrength = 0.3;
    vec3 ambient = vec3(ambientStrength);

	// diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - Position);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = vec3(diff);

    // View space
    vec3 Position2 =  vec3(Camera * vec4(Position, 1));
    vec3 viewPos2 = vec3(0, 0, 0);
    vec3 norm2 = normalize(vec3(Camera * vec4(Normal, 1)));
    vec3 lightPos2 = vec3(Camera * vec4(lightPos, 1));
    vec3 lightDir2 = normalize(lightPos2 - Position2);

    //Specular
    float specularStrength = 2;
    vec3 viewDir = normalize(viewPos2 - Position2);
    vec3 reflectDir = reflect(-lightDir2, norm2);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 128);
    vec3 specular = vec3(specularStrength * spec);  

    color = (specular + diffuse + ambient) * texture(myTexture, uv).rgb;
}
