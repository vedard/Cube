uniform sampler2D tex;
varying vec4 light;
uniform bool underwater;
uniform bool underlava;
uniform bool hurt;

uniform sampler2D tex0;
uniform float border; // 0.01
uniform float circle_radius; // 0.5
uniform vec4 circle_color; // vec4(1.0, 1.0, 1.0, 1.0)
uniform vec2 circle_center; // vec2(0.5, 0.5)


void main()
{
    vec4 texel;
    
	texel = texture2D(tex,gl_TexCoord[0].st);

	if(underwater)
	{

    	texel.rgb *= light.rgb * 0.9;
		texel.a *= light.a * 0.9;
		texel.r -= 0.20;
		texel.g -= 0.20;
		texel.b -= 0.05;
	}
	if(underlava)
	{
		texel.rgb *= light.rgb * 0.9;
		texel.a *= light.a * 0.7;
		texel.r -= 0.1;
		texel.g -= 0.25;
		texel.b -= 0.50;
	}
	if(hurt)
	{
		vec2 uv = gl_TexCoord[0].xy;
  
		vec4 bkg_color = texture2D(tex0,uv * vec2(1.0, -1.0));
 
		// Offset uv with the center of the circle.
		uv -= circle_center;
  
		float dist =  sqrt(dot(uv, uv));
		if ( (dist > (circle_radius+border)) || (dist < (circle_radius-border)) )
			gl_FragColor = bkg_color;
		else 
			gl_FragColor = circle_color;
	}
    gl_FragColor = texel;
}
