#version 330 core

in vec3 ourColor;
in vec2 ourTexCoord;

out vec4 FragColor;

uniform sampler2D ourTexture;

void main()
{
	vec4 texel = texture(ourTexture, ourTexCoord);
	if (texel.xyz == vec3(0.0f, 0.0f, 0.0f))
		FragColor = vec4(ourColor, 1.0f);
	else
		FragColor = texture(ourTexture, ourTexCoord) * vec4(ourColor, 1.0f);
}
