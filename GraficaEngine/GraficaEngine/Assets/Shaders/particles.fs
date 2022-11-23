#version 460 core

in float quad_side;
in vec2 tex_coords;
in vec3 center;
in vec3 point_position;

out vec4 FragColor;

uniform sampler2D particle_tex;

float sqr2 = 1.41421356237;

void main()
{
    vec4 texture_value = texture(particle_tex, tex_coords);
    float distance_from_center = abs(distance(center, point_position));
    float normalized_distance = distance_from_center / quad_side;

    FragColor = vec4(vec3(0.6), (1.0 - normalized_distance));

    // Use a texture
/*     FragColor = vec4(texture_value.rgb, (1.0 - normalized_distance) * texture_value.a); */

/*     vec3 lol = point_position - center;
    float distance_from_center = abs(dot(lol, lol));
    float normalized_distance = distance_from_center / (quad_side * quad_side);

    if(normalized_distance > 1.0) {
        normalized_distance = 0.0;
    }
    FragColor = vec4(vec3(1.0), 1.0 - normalized_distance); */
} 