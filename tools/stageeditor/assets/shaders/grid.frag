#version 450 core

#extension GL_NV_fragment_shader_barycentric : require

out vec4 fragColor;

in flat ivec2 uv;
in float enabler;

uniform sampler2D grid;

vec3 calculate_wireframe(in float Thickness, in float Falloff)
{
	const vec3 BaryCoord = gl_BaryCoordNV;

	const vec3 dBaryCoordX = dFdxFine(BaryCoord);
	const vec3 dBaryCoordY = dFdyFine(BaryCoord);
	const vec3 dBaryCoord  = sqrt(dBaryCoordX*dBaryCoordX + dBaryCoordY*dBaryCoordY);

	const vec3 dFalloff   = dBaryCoord * Falloff;
	const vec3 dThickness = dBaryCoord * Thickness;

	const vec3 Remap = smoothstep(dThickness, dThickness + dFalloff, BaryCoord);
	return Remap;
}

void main()
{
	const vec3 Remap = calculate_wireframe(0.25, 1.0);
	const float ClosestEdge = min(min(Remap.x, Remap.y), Remap.z);
	fragColor = vec4(1.0, 0.5, 0.5, mix(0.5, 1-(enabler == 0.f ? ClosestEdge : Remap.y), 1-texelFetch(grid, uv, 0).r));
}
