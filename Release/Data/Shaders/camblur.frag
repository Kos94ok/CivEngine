#version 130

uniform sampler2D texture;
uniform float iSampleCount;
uniform float sampleOffset;
uniform float camVectorX;
uniform float camVectorY;

void main()
{
	float sampleDiv = (iSampleCount * 2 + 1) * (iSampleCount * 2 + 1);
	float sampleCount = iSampleCount;
	float vecMod = max(abs(camVectorX), abs(camVectorY));
	
	vec4 sum = vec4(0);
	for (int i = -int(sampleCount); i <= int(sampleCount); i++)
	{
		for (int y = -int(sampleCount); y <= int(sampleCount); y++)
		{
			//vec2 offset = vec2(-camVectorX, -camVectorY) * vec2(i, y) * sampleOffset;
			vec2 offset = vec2(i, y) * sampleOffset * vecMod;
			sum += texture2D(texture, gl_TexCoord[0].xy + offset);
		}
	}

	gl_FragColor = (sum / (sampleDiv));
}