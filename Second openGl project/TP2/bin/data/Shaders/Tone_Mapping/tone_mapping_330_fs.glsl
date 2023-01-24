// IFT3100H21 ~ tone_mapping_330_fs.glsl

#version 330

// attribut en entrée
in vec2 surface_texcoord;

// attribut en sortie
out vec4 fragment_color;

// attributs uniformes
uniform sampler2D image;

// facteur d'exposition
uniform float tone_mapping_exposure;

// mode de mappage tonal 
uniform int tone_mapping_style;

// facteur gamma
uniform float tone_mapping_gamma;

// mappage tonal de la couleur hdr vers ldr (reinhard tone mapping)
vec3 tone_mapping_reinhard(vec3 x)
{
  return x / (x + vec3(1.0));
}

// mappage tonal de la couleur hdr vers ldr (approximation de la courbe du ACES filmic tone mapping)
vec3 tone_mapping_aces_filmic(vec3 x)
{
  float a = 2.51f;
  float b = 0.03f;
  float c = 2.43f;
  float d = 0.59f;
  float e = 0.14f;
  return clamp((x * (a * x + b)) / (x * (c * x + d) + e), 0.0, 1.0);
}

// fonction pour le tone_mapping_aces_hill
vec3 mulInput(vec3 color) {
	float a = 0.59719 * color.r + 0.35458 * color.g + 0.04823 * color.b,
		  b = 0.07600 * color.r + 0.90834 * color.g + 0.01566 * color.b,
		  c = 0.02840 * color.r + 0.13383 * color.g + 0.83777 * color.b;
	return vec3(a, b, c);
}
vec3 mulOutput(vec3 color) {
	float a =  1.60475 * color.r - 0.53108 * color.g - 0.07367 * color.b,
		  b = -0.10208 * color.r + 1.10813 * color.g - 0.00605 * color.b,
		  c = -0.00327 * color.r - 0.07276 * color.g + 1.07602 * color.b;
	return vec3(a, b, c);
}

//mappage tonal aces filmic de Stephen Hill
vec3 tone_mapping_aces_hill(vec3 x)
{
	x = mulInput(x);
	vec3 a = x * (x + 0.0245786) - 0.000090537;
	vec3 b = x * (0.983729 * x + 0.4329510) + 0.238081;
	vec3 Cout = a/b;
	
	Cout = mulOutput(Cout);
	Cout = clamp(Cout, 0.0, 1.0);
	return Cout;
}

//converstion d'une couleur vers SRGB
float toSRGB(float value) {
	if (value < 0.0031308) {
		return 12.92 * value;
	}
	return 1.055 * pow(value, 0.41666) - 0.055;
}

//mappage tonal qui converti les couleur dans un espace SRGB
vec3 tone_mapping_Srgb(vec3 x)
{
	vec3 Cout = vec3(toSRGB(x.r), toSRGB(x.g), toSRGB(x.b));
	Cout = clamp(Cout, 0.0, 1.0);
	return Cout;
}

//mappage tonal avec l'agorithme de Jim Hejl et Richard Burgess-Dawson
vec3 tone_mapping_HBD(vec3 Cin)
{
	vec3 x  = max(vec3(0.0), Cin - 0.004),
	Cout = (x * (6.2 * x + 0.5)) / (x * (6.2 * x + 1.7) + 0.06);
	Cout = clamp(Cout, 0.0, 1.0);
	return Cout;
}

void main()
{	
  // variable qui va contenir la couleur à corriger

  vec3 color;

  // échantillonner la texture et verifier pour ne pas utiliser les pixels qui ont de la transparence
  vec4 texture_samplet = texture(image, surface_texcoord).rgba;
  if (texture_samplet.a == 0)
  {
	discard;
  }
  
  vec3 texture_sample = texture_samplet.rgb;
 
	
  // valeur initiale de la couleur
  color = texture_sample;

  // conversion de la couleur de l'espace gamma vers l'espace linéaire
  color = pow(color, vec3(tone_mapping_gamma));

  // ajustement de l'exposition
  color = vec3(1.0) - exp(-color * tone_mapping_exposure);

  // mappage tonal de la couleur hdr vers ldr selon le type choisi
  if (tone_mapping_style == 1)
    color = tone_mapping_aces_filmic(color);
  if (tone_mapping_style == 2)
    color = tone_mapping_reinhard(color);
  if (tone_mapping_style == 3)
	color = tone_mapping_aces_hill(color);
  if (tone_mapping_style == 4)
	color = tone_mapping_Srgb(color);	
  if (tone_mapping_style == 5)
	color = tone_mapping_HBD(color);	

  // conversion de couleur de l'espace linéaire vers l'espace gamma
  color = pow(color, vec3(1.0 / tone_mapping_gamma));

  // couleur du fragment
  fragment_color = vec4(color, 1.0);
}
