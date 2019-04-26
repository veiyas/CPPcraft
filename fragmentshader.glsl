#version 330 core

uniform float time;
uniform sampler2D tex;

in vec3 interpolatedNormal;
in vec2 st;

out vec4 color;

void main() {
     vec4 texcolor = texture(tex, st);
     vec3 nNormal = normalize(interpolatedNormal);
     float diffuse = max(0.0, nNormal.z);
     //Only ambient light for now
     color = texcolor;

//     color = texcolor * diffuse;
}
