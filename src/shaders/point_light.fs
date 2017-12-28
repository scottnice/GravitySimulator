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

vec3 CalcPointLight(vec3 light, vec3 normal, vec3 viewDir);
void main()
{
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 norm = normalize(Normal);
	vec3 result = vec3(0);
	if(constant > 0){
		for(int i = 0; i < numberLights; i++){
			result += CalcPointLight(lightPos[i], norm, viewDir);
		}
	}else{
		result = objectColor;
	}
    FragColor = vec4(result, 1.0);
} 

float blinnPhongSpecular(vec3 normal, vec3 viewDir, vec3 lightDir){
	vec3 halfwayDir = normalize(lightDir + viewDir);
	return pow(max(dot(normal, halfwayDir), 0.0), 16.0);
}

float phongSpecular(vec3 normal, vec3 viewDir, vec3 lightDir){
    vec3 reflectDir = reflect(-lightDir, normal);
	return pow(max(dot(normal, reflectDir), 0.0), 8.0);
}

// calculates the color when using a point light.
vec3 CalcPointLight(vec3 light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(light - FragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    float spec = blinnPhongSpecular(normal, viewDir, lightDir);
    // attenuation
    float distance = length(light - FragPos);
    float attenuation = 1.0 / (constant + linear * distance + quadratic * (distance * distance));    
    // combine results
    vec3 l_ambient = objectColor * 0.5 * attenuation;
    vec3 l_diffuse = objectColor * diff * attenuation;
    vec3 l_specular = lightColor * spec *attenuation;
    return (l_ambient + l_diffuse + l_specular);
}

