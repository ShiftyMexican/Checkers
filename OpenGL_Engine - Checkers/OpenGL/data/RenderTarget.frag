#version 410

in vec2 vTexCoord;

out vec4 FragColor;

uniform sampler2D diffuse;
uniform sampler2D myTexture;

vec4 Simple()
{
	return texture(diffuse, vTexCoord);
}

vec4 BoxBlur()
{
	vec2 texel = 1.0f / textureSize(diffuse, 0).xy;

	// 9-tap box kernel
	vec4 colour = texture(diffuse, vTexCoord);
	colour += texture(diffuse, vTexCoord + vec2(-texel.x, texel.y));
	colour += texture(diffuse, vTexCoord + vec2(-texel.x, 0));
	colour += texture(diffuse, vTexCoord + vec2(-texel.x, -texel.y));
	colour += texture(diffuse, vTexCoord + vec2(0, texel.y));
	colour += texture(diffuse, vTexCoord + vec2(0, -texel.y));
	colour += texture(diffuse, vTexCoord + vec2(texel.x, texel.y));
	colour += texture(diffuse, vTexCoord + vec2(texel.x, 0));
	colour += texture(diffuse, vTexCoord + vec2(texel.x, -texel.y));

	return colour / 9;
}

vec4 Distort()
{
	vec2 mid = vec2(0.5f);

	float distanceFromCentre = distance(vTexCoord, mid);
	vec2 normalizedCoord = normalize(vTexCoord - mid);
	float bias = distanceFromCentre + sin(distanceFromCentre * 15) * 0.02f;

	vec2 newCoord = mid + bias * normalizedCoord;

	return texture(diffuse, newCoord);
}

void main()
{
	vec4 yourTurn;

	yourTurn = texture(myTexture, vTexCoord);

	FragColor = yourTurn;
};