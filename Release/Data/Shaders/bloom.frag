#version 130

uniform sampler2D texture;
uniform float iSampleCount;
uniform float sampleOffset;

void main()
{
	float sampleDiv = iSampleCount * iSampleCount;
	float sampleCount = iSampleCount - 1.0;
	sampleCount = floor(sampleCount / 2.0);
	vec4 color = texture2D(texture, gl_TexCoord[0].xy);
	sampleCount = 2.0;
	
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

	gl_FragColor = (sum / (sampleDiv)) + color;
}