#version 330			// Shader 3.3
precision highp float;	// normal floats, makes no difference on desktop computers
struct Light {
	vec3 La, Le;
	vec4 wLightPos;
};
struct Material {
	vec3 kd, ks, ka;
	float shininess;
};

uniform sampler2D shadowMap;

uniform vec3 objectColor;		// uniform variable, the color of the primitive
uniform Material material;
uniform Light light;

in vec3 wNormal; //interpolated world sp normal
in vec3 wView; //interpolated world sp view
in vec3 wLight; //interpolated world sp illum dir
in vec3 baseColor;
in vec4 lPosition; // fragment position in light space

out vec4 outColor;		// computed color of the current pixel

// returns 1.0f if the parameter is in shadow, 0.0f if not
float isInShadow(vec4 position) {
	// perspective divide: clip space -> NDC i. e. [-w, w] -> [-1, 1]
	vec3 projCoords = position.xyz / position.w;
	// [-1, 1] -> [0, 1] to easily sample from depth map
	projCoords = projCoords * 0.5 + 0.5;
	float closestDepth = texture(shadowMap, projCoords.xy).r;
	float currentDepth = projCoords.z;
	return currentDepth > closestDepth ? 1.0f : 0.0f;  
}

void main() {
	vec3 N = wNormal;
	vec3 V = normalize(wView);
	//if (dot(N, V) < 0) N = -N;	// prepare for one-sided surfaces
	vec3 L = normalize(wLight);
	vec3 H = normalize(L + V);
	float cost = max(dot(N, L), 0), cosd = max(dot(N, H), 0);
	vec3 baseColorN = normalize(baseColor);
	vec3 radiance =  baseColorN * material.ka * light.La; // ambient part not affected by shadow
	radiance += (material.kd * baseColorN * cost + material.ks * pow(cosd, material.shininess)) * light.Le;
	//radiance += (1.0f - isInShadow(lPosition)) * (material.kd * baseColorN * cost + material.ks * pow(cosd, material.shininess)) * light.Le;
	outColor = vec4(radiance, 1);
}
