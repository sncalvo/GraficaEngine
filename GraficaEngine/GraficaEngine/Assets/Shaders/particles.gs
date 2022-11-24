#version 460 core

layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

out vec2 tex_coords;
out vec3 center;
out vec3 point_position;
out float quad_side;

uniform mat4 projection;

float cyclingCoord(float x) {
    return mod(x, 2.0) - 1;
}

vec4 cycling(vec4 d) {
    vec3 v = d.xyz / d.w;
    return vec4(cyclingCoord(v.x),v.y,cyclingCoord(v.z), 1.0);
}

void main() {
    quad_side = 0.01;
    vec4 position = gl_in[0].gl_Position;
    center = position.xyz / position.w;
    for(float i=-1;i<2;i=i+2) {
        for(float j=-1;j<2;j=j+2) {
            tex_coords = vec2(clamp(i, 0.0, 1.0), clamp(j, 0.0, 1.0));
            vec4 offset = vec4(quad_side * i, quad_side * j, position.z, 1.0);
            gl_Position = position + vec4(offset.xy, 0.0,0.0);    // 1:bottom-left
            point_position = gl_Position.xyz / gl_Position.w;

            vec4 true_offset = projection * offset;
            gl_Position = cycling(projection * position) + vec4((true_offset.xy / true_offset.w), 0.0, 0.0);
            EmitVertex();   
        }
    }
}  