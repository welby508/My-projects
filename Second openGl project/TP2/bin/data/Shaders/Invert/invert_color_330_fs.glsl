// IFT3100H22 ~ image_filter_330_fs.glsl

#version 330

// attribut en entrée
in vec2 surface_texcoord;

// attribut en sortie
out vec4 fragment_color;

// attributs uniformes
uniform sampler2D image;
uniform vec3 tint;
uniform float factor;

void main()
{
	vec4 texture_samplet = texture(image, surface_texcoord).rgba;
	if(texture_samplet.a == 0)
	{
		discard;
	}
		

  // échantillonner la texture
  vec3 texture_sample = texture(image, surface_texcoord).rgb;


  // couleur finale du fragment
  fragment_color = vec4(1.0 - texture_sample.r,1.0 -texture_sample.g,1.0 -texture_sample.b, 1.0);
}
