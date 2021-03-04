#version 450 core

in GSOUT
{
	flat vec3 normal;
	flat int type;
	vec3 localCoord;
} fs_in;

uniform sampler2DArray terrain;
uniform int blockThreshold;

uniform vec3 lightDirs[6] = vec3[](
	vec3(0, 1, 0),
	vec3(1, 0, 0),
	vec3(-1, 0, 0),
	vec3(0, 0, 1),
	vec3(0, 0, -1),
	vec3(0, -1, 0)
);

uniform float lightStrengths[6] = float[](
	1.0,
	0.95,
	0.9,
	0.85,
	0.8,
	0.75
);

layout (location = 0) out vec4 fragColor;

vec2 extract(in vec3 coords, in vec3 mask)
{
	return mix(
		mix(coords.xy, coords.xz, float(abs(mask.y) > 0.5)),
		coords.yz,
		float(abs(mask.x) > 0.5)
	);
}

void main()
{
	if (fs_in.type <= blockThreshold)
	{
		discard;
		return;
	}

	vec3 col= vec3(0.0);
	vec3 normal = normalize(fs_in.normal);
	vec2 texCoord = extract(fs_in.localCoord, normal) * 0.5 + 0.5;
	vec3 terrainColor = texture(terrain, vec3(texCoord, (fs_in.type - 1) / 4)).rgb;

	for (int i = 0; i < 6; ++i)
	{
		vec3 L = lightDirs[i];
		float lambertTerm = dot(normal, L);
		col += max(lambertTerm, 0.0) * terrainColor * lightStrengths[i];
	}

	fragColor = vec4(col, 1.0);
}