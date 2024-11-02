#version 330

in vec4 vertexColor;
in vec2 texCoord;
out vec4 color;

struct DirectionalLight {
  vec3 color;
  float ambientIntensity;
};

uniform sampler2D theTexture;
uniform DirectionalLight directionalLight;

void main() {
    vec4 ambientColor = vec4(directionalLight.color, 1.0f) * directionalLight.ambientIntensity;
    color = texture(theTexture, texCoord) * ambientColor;
}