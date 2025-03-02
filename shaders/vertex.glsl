#version 130

in vec3 position;
in vec2 texcoord;

uniform mat4 projection;
uniform mat4 modelView;

out vec2 texCoord;

void main() {
    gl_Position = projection * modelView * vec4(position, 1.0);
    texCoord = texcoord;
}
