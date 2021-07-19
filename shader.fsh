#version 330 core

out vec4 FragColor;

in vec3 ourColor;
in vec2 texCoord;

uniform sampler2D ourTexture;
uniform sampler2D texture2;

void main() {
	FragColor = mix(texture(ourTexture, texCoord), texture(texture2, texCoord), 0.5);
}