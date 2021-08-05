#version 330 core

in vec3 ourColor;
in vec2 ourTexCoord;

out vec4 FragColor;

uniform sampler2D ourTexture1;
uniform sampler2D ourTexture2;

void main()
{
	vec4 texel = texture(ourTexture1, ourTexCoord);
	if (texel.xyz == vec3(0.0f, 0.0f, 0.0f))
		FragColor = vec4(ourColor, 1.0f);
	else
		FragColor = mix(texture(ourTexture1, ourTexCoord), texture(ourTexture2, ourTexCoord), 0.2f);
}
