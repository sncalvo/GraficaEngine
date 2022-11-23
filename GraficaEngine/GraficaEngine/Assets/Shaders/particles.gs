#version 460 core

layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

out vec2 tex_coords;
out vec3 center;
out vec3 point_position;
out float quad_side;

void main() {
    quad_side = 0.2;
    vec4 position = gl_in[0].gl_Position;
    center = position.xyz / position.w;
    for(float i=-1;i<2;i=i+2) {
        for(float j=-1;j<2;j=j+2) {
            tex_coords = vec2(clamp(i, 0.0, 1.0), clamp(j, 0.0, 1.0));
            gl_Position = position + vec4(quad_side * i, quad_side * j, 0.0, 0.0);    // 1:bottom-left
            point_position = gl_Position.xyz / gl_Position.w;
            EmitVertex();   
        }
    }
}  