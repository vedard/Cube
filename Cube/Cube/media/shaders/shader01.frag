uniform sampler2D tex;
varying vec4 light;
uniform bool underwater;
uniform bool underlava;

void main()
{
    vec4 texel;
    
	texel = texture2D(tex,gl_TexCoord[0].st);

	if (!underwater && !underlava)
	{
    	texel.rgb *= light.rgb * 0.9;
		texel.a *= light.a * 0.9;
	}
	else if(underwater)
	{

    		texel.rgb *= light.rgb * 0.9;
		texel.a *= light.a * 0.9;
		texel.r -= 0.20;
		texel.g -= 0.20;
		texel.b -= 0.05;
	}
	else if(underlava)
	{
		texel.rgb *= light.rgb * 0.9;
		texel.a *= light.a * 0.7;
		texel.r -= 0.1;
		texel.g -= 0.25;
		texel.b -= 0.50;
	}
    gl_FragColor = texel;
}