#version 130

uniform sampler2D texture;
uniform float iSampleCount;
uniform float sampleOffset;
uniform float colorR;
uniform float colorG;
uniform float colorB;

void main()
{
	float sampleDiv = iSampleCount * iSampleCount;
	float sampleCount = floor((iSampleCount - 1.0) / 2.0);
	vec4 color = texture2D(texture, gl_TexCoord[0].xy);
	
	int i, y;
	vec4 sum = vec4(0);
	for (i = int(-sampleCount); i <= int(sampleCount); i++)
	{
		for (y = int(-sampleCount); y <= int(sampleCount); y++)
		{
			vec2 offset = vec2(i, y) * sampleOffset;
			sum += texture2D(texture, gl_TexCoord[0].xy + offset);
		}
	}
	vec4 finalColor = (sum / (sampleDiv)) + color;
	if (finalColor.a > 0.50) {
		gl_FragColor = vec4(colorR, colorG, colorB, 1.0);
	}
	else {
		gl_FragColor = finalColor;
	}
}