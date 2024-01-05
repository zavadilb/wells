#version 330				// Shader 3.3
precision highp float;		// normal floats, makes no difference on desktop computers

uniform mat4 Minv;
uniform mat4 M;

layout(location = 0) in vec3 vtxPos;
layout(location = 1) in vec3 vtxNorm;
layout(location = 2) in vec3 color;


out VS_OUT {
    vec3 normal;
} vs_out;

void main() {
	gl_Position = vec4(vtxPos, 1.0) * M;
	vs_out.normal = normalize((transpose(Minv) * vec4(vtxNorm, 0)).xyz);
}
