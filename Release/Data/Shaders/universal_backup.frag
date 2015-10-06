#version 130

#ifdef GL_ES
precision highp float;
#endif

uniform sampler2D texMain;
uniform sampler2D texLight;
uniform sampler2D texShadow;
uniform sampler2D texLightMult;
uniform float boostR;
uniform float boostG;
uniform float boostB;
uniform float checkForShadow;
uniform float shadowBrightness;
uniform float ambientLight;
uniform float enableBetterLight;

void main()
{
	float texX = gl_TexCoord[0].x;
	float texY = gl_TexCoord[0].y;
		
	vec2 coord = vec2(texX, texY);
	float texR = texture(texMain, coord).r * boostR;
	float texG = texture(texMain, coord).g * boostG;
	float texB = texture(texMain, coord).b * boostB;
	vec4 boost = vec4(boostR, boostG, boostB, 1.00);
	vec4 finalColor;
	vec4 shadowColor = vec4(0, 0, 0, 0);

	vec4 lightColor = texture(texLight, coord);
	float lightLevel = max(lightColor.r, max(lightColor.g, lightColor.b)) * 0.80;
	
	finalColor = vec4(texR, texG, texB, 1.00);

	// Apply light
	vec4 advLight;
	if (enableBetterLight == 1.0)
	{
		advLight = texture(texLightMult, coord) * (ambientLight + 0.50 * ambientLight);
		finalColor = finalColor * 1.0 * (texture(texLight, coord) + advLight);
	}
	else
	{
		advLight = vec4(0.0, 0.0, 0.0, 0.0);
		finalColor = finalColor * 1.0 * texture(texLight, coord);
	}
	lightLevel += max(advLight.r, max(advLight.g, advLight.b));
	
	// Better shadows
	if (checkForShadow == 1.00)
	{
		vec4 texColor = texture(texMain, gl_TexCoord[0].xy);
		vec4 shadColor = texture(texShadow, gl_TexCoord[0].xy);
		//if (abs(texColor.r - shadColor.r) > 0.02 || abs(texColor.g - shadColor.g) > 0.02 || abs(texColor.b - shadColor.b) > 0.02)
		if (shadColor.r <= 0.25 && shadColor.g <= 0.25 && shadColor.b <= 0.25)
		{
			float md = max(0.0, min(1.0, lightLevel - ambientLight));
			float tr = min(1.0, shadowBrightness * 1.2 + md);
			shadowColor = vec4(tr, tr, tr, 1.0);
			finalColor *= shadowColor;
		}
	}	

	// Flush changes	
	gl_FragColor = finalColor;
}