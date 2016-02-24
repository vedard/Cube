# version 330 compatibility
out vec4 light;
uniform float gameTime;
in float type;

out float atten; 
out float fogFactor; 
out vec3 lightVec, viewVec; 
in vec3 tangent; 

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec3 in_normal;
layout(location = 2) in vec2 in_texcoord;
 
uniform mat4 model_matrix, view_matrix, projection_matrix;
 
out vec3 world_pos;
out vec3 world_normal;
out vec2 texcoord;
out vec4 viewSpace;

void main()
{	
	light = gl_Color;
	gl_Position = ftransform();
	gl_TexCoord[0] = gl_MultiTexCoord0;

	//used for lighting models
	world_pos = (model_matrix * vec4(in_position,1)).xyz;
	world_normal = normalize(mat3(model_matrix) * in_normal);
	texcoord = in_texcoord;
 
	//send it to fragment shader
	viewSpace = view_matrix * model_matrix * vec4(in_position,1);
	gl_Position = projection_matrix * viewSpace;

	//Si le vertex est un BTYPE_WATER ou on fait des vague
	if(type == 16.0 )
	{		
			float waterHeight = 0.06;
			float waterSpeed = 0.5;
			vec4 newPos = vec4(gl_Vertex.x, gl_Vertex.y, gl_Vertex.z, gl_Vertex.w);
			newPos.y += (sin(gameTime * waterSpeed) * waterHeight) + (sin(gameTime*1.14159 * waterSpeed) * waterHeight * 1.5 * sin(gl_Vertex.x )) - 0.2;	
			gl_Position = gl_ModelViewProjectionMatrix * newPos;
	}
	//BTYPE_LAVA
	else if(type == 25.0 )
	{		
			vec4 newPos = vec4(gl_Vertex.x, gl_Vertex.y, gl_Vertex.z, gl_Vertex.w);
			newPos.y -= 0.2;	
			gl_Position = gl_ModelViewProjectionMatrix * newPos;	
	}
	else
		gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;			

}
