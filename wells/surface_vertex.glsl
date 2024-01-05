#version 330				// Shader 3.3
precision highp float;		// normal floats, makes no difference on desktop computers

struct Light {
	vec3 La, Le;
	vec4 wLightPos;
};

uniform mat4 MVP; // MVP, Model, Model-inverse
uniform mat4 M;
uniform mat4 Minv;
uniform Light light;
uniform vec3 wEye; // Position of eye
uniform mat4 lightVP; // Light View * Light Projection

layout(location = 0) in vec3 vtxPos;
layout(location = 1) in vec3 vtxNorm;
layout(location = 2) in vec3 color;

out vec3 wNormal; // normal in world space
out vec3 wView; // view in world space
out vec3 wLight; // light dir in world space
out vec3 baseColor;
out vec4 lPosition; // fragment position in light space

void main() {
	gl_Position = vec4(vtxPos, 1) * MVP; // to NDC
	vec4 wPos = vec4(vtxPos, 1) * M; // position in world space (homogeneous coords)
	wLight = light.wLightPos.xyz/light.wLightPos.w - wPos.xyz/wPos.w;
	wView = wEye - wPos.xyz/wPos.w;
	wNormal = normalize( (transpose(Minv) * vec4(vtxNorm, 0)).xyz );
	baseColor = color;
	lPosition = wPos * lightVP;
}
