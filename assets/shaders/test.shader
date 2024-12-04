#shader vertex
#version 300 es

layout (location = 0) in vec4 pos;
layout (location = 1) in vec3 col;
layout (location = 2) in vec2 aTexCoord;

out vec3 color;
out vec2 texCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    gl_Position = projection * view * model * pos;
    color = col;
    texCoord = aTexCoord;
}

#shader fragment
#version 300 es
precision mediump float;

in vec3 color;
in vec2 texCoord;

out vec4 frag_color;

uniform sampler2D tex;

void main() {
    frag_color = texture(tex, texCoord) * vec4(color, 1.0);
    // frag_color = vec4(color, 1.0);
}

