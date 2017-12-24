#version 330 core
out vec4 FragColor;

in vec3 Normal;  
in vec3 FragPos;  
uniform int numberLights;
uniform vec3 lightPos[20]; 
uniform vec3 viewPos; 
uniform vec3 lightColor;
uniform vec3 objectColor;
uniform float constant;
uniform float linear;
uniform float quadratic;
vec3 CalcPointLight(vec3 light, vec3 normal, vec3 fragPos, vec3 viewDir);
void main()
{
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 norm = normalize(Normal);
	vec3 result;
	if(constant > 0){
		for(int i = 0; i < numberLights; i++){
			result += CalcPointLight(lightPos[i], norm, FragPos, viewDir);
		}
	}else{
		result = objectColor;
	}
    FragColor = vec4(result, 1.0);
} 


// calculates the color when using a point light.
vec3 CalcPointLight(vec3 light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 8.0);
    // attenuation
    float distance = length(light - fragPos);
    float attenuation = 1.0 / (constant + linear * distance + quadratic * (distance * distance));    
    // combine results
    vec3 l_ambient = objectColor * 0.05;
    vec3 l_diffuse = lightColor * diff * 0.5;
    vec3 l_specular = lightColor * spec;
	l_diffuse *= attenuation;
    return (l_ambient + l_diffuse + l_specular);
}

