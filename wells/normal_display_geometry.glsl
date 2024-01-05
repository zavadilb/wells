#version 330
layout (triangles) in;
layout (line_strip, max_vertices = 6) out;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;
uniform float MAGNITUDE;

in VS_OUT {
    vec3 normal;
} gs_in[];

void drawNormal(int i) {
    gl_Position = gl_in[i].gl_Position * V * P;
    EmitVertex();
    
    gl_Position = (gl_in[i].gl_Position + vec4(gs_in[i].normal, 0) * MAGNITUDE) * V * P;
    EmitVertex();
    
    EndPrimitive();
}

void main()
{
    drawNormal(0);
    drawNormal(1);
    drawNormal(2);
}  