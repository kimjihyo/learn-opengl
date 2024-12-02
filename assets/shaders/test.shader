#shader vertex
#version 300 es
layout (location = 0) in vec4 pos;
layout (location = 1) in vec3 col;

 out vec3 color;

void main() {
    gl_Position = pos;
    color = col;
}

#shader fragment
#version 300 es
precision mediump float;
in vec3 color;
out vec4 frag_color;
void main() {
    frag_color = vec4(color, 1.0);
}

