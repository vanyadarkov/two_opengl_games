#version 330

// Input
// TODO(student): Get vertex attributes from each location
layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_coordinate;
layout(location = 3) in vec3 v_color;


// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform vec4 car_position;
uniform float curvature_scale_factor;

// Output
// TODO(student): Output values to fragment shader
out vec3 frag_position;
out vec3 frag_normal;
out vec2 frag_coordinate;
out vec3 frag_color;

void main()
{
    // TODO(student): Send output to fragment shader

    frag_position = v_position;
    frag_normal = v_normal;
    frag_coordinate = v_coordinate;
    frag_color = v_color;

    // TODO(student): Compute gl_Position
    gl_Position = Projection * View * Model * vec4(v_position, 1.0);
    vec4 aux = car_position - gl_Position;
    // asupra coordonatei y a vertexului procesat la moment se aplicat o transformare,
    // care simplu vorbind, ne vizualizeaza coordonata y curenta mai jos sau mai sus
    // in dependenta de coordonatele masinii jucatorului
    gl_Position[1] = gl_Position[1] - length(aux) * length(aux) * curvature_scale_factor;
}
