#version 330 core

uniform vec3 lightColor;

in vec3 normal;
in vec3 fragPos;
out vec4 FragColor;

void main() {
	FragColor = vec4(lightColor, 1.0f);
}