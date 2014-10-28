uniform sampler2D tex;
varying vec4 light;

void main()
{
    vec4 texel;
    texel = texture2D(tex,gl_TexCoord[0].st);

    texel *= light;

    gl_FragColor = texel;
}
