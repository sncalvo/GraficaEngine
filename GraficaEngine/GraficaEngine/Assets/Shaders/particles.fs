#version 460 core

in float quad_side;
in flat int type;
in vec2 tex_coords;
out vec4 FragColor;

uniform sampler2D particle_tex;

float sqr2 = 1.41421356237;

void main()
{
    float distance_from_center = abs(distance(tex_coords, vec2(0.5))) * 2.0;
    if (type > 0) {
        FragColor = vec4(vec3(1.0), int(distance_from_center < 1) * 0.3);
    } else if (type < 0) {
        vec4 tex = texture(particle_tex, tex_coords);
        FragColor = tex;
    } else {
        FragColor = vec4(vec3(0.9), (1.0 - distance_from_center));
    }

/*     FragColor = vec4(vec3(5.0), (1.0 - normalized_distance)); */

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