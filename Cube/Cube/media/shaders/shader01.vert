varying vec4 light;

uniform float gameTime;
attribute float type;



void main()
{
	
    light = gl_Color;
    gl_TexCoord[0] = gl_MultiTexCoord0;

	//Si le vertex est un BTYPE_WATER on fait des vague
	if(type == 16.0 )
	{
		
			float waterHeight = 0.06;
			float waterSpeed = 0.5;
			vec4 newWaterPos = vec4(gl_Vertex.x, gl_Vertex.y, gl_Vertex.z, gl_Vertex.w);

			newWaterPos.y += (sin(gameTime * waterSpeed) * waterHeight) + (sin(gameTime*1.14159 * waterSpeed) * waterHeight * 1.5 * sin(gl_Vertex.x )) - 0.2;
		
			gl_Position = gl_ModelViewProjectionMatrix * newWaterPos;
	
	}
	else
		gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;

	
}
