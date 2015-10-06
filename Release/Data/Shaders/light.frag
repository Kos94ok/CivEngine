#version 130

uniform sampler2D tex_main;
uniform sampler2D tex_light;

void main()
{
	vec4 texColor = texture(tex_main, gl_TexCoord[0].xy);
	vec4 texMod = texture(tex_light, gl_TexCoord[0].xy);
	
	gl_FragColor = texColor * texMod;
}