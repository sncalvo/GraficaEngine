#version 460 core

layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

out vec2 tex_coords;
out float quad_side;

uniform mat4 projection;
uniform mat4 view;

void main() {
    mat4 to_cdn_matrix = projection * view;
    quad_side = 0.10;
    vec4 position = gl_in[0].gl_Position;
    vec4 cdn = to_cdn_matrix * gl_in[0].gl_Position;
    vec4 vx = vec4(quad_side, 0.0, 0.0, 0.0);
    vec4 vy = vec4(0.0, quad_side, 0.0, 0.0);


    tex_coords = vec2(0.0);
    vec4 offset = vec4(quad_side, quad_side, 0.0, 0.0);
    gl_Position = position + offset;
    gl_Position = cdn + vx; 
    EmitVertex();   

    tex_coords = vec2(0.0, 1.0);
    gl_Position = position + offset;
    gl_Position = cdn + vy; 
    EmitVertex();   

    tex_coords = vec2(1.0, 0.0);
    gl_Position = position + offset;
    gl_Position = cdn - vy; 
    EmitVertex();   

    tex_coords = vec2(1.0, 1.0);
    gl_Position = position + offset;
    gl_Position = cdn - vx; 
    EmitVertex();   

    EndPrimitive();
}  