#version 130

uniform sampler2D texture;
uniform float iSampleCount;
uniform float iSampleOffset;
uniform float transpar;

void main()
{
	float sampleOffset = 0.005;
	int sampleDiv = int(iSampleCount * iSampleCount);
	int sampleCount = int(iSampleCount) - 1;
	sampleCount = sampleCount / 2;
	vec4 color = vec4(0, 0, 0, texture2D(texture, gl_TexCoord[0].xy).a);
	
	int i, y;
	vec4 sum = vec4(0);
	for (i = -sampleCount; i <= sampleCount; i++)
	{
		for (y = -sampleCount; y <= sampleCount; y++)
		{
			vec2 offset = vec2(i, y) * sampleOffset;
			sum += vec4(0, 0, 0, texture2D(texture, gl_TexCoord[0].xy + offset).a);
		}
	}

	gl_FragColor = (sum / (sampleDiv)) * transpar;
}