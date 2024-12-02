#shader vertex
#version 300 es
in vec4 pos;
void main() {
    gl_Position = pos;
}

#shader fragment
#version 300 es
precision mediump float;
out vec4 frag_color;
void main() {
    frag_color = vec4(1.0, 0.0, 0.0, 1.0);
}

