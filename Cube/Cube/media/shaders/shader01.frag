uniform sampler2D tex;
varying vec4 light;
uniform bool underwater;

void main()
{
    vec4 texel;
    texel = texture2D(tex,gl_TexCoord[0].st);

    texel *= light;

	if(underwater)
	{
		texel.r -= 0.20;
		texel.g -= 0.20;
		texel.b -= 0.05;
	}
    gl_FragColor = texel;
}
