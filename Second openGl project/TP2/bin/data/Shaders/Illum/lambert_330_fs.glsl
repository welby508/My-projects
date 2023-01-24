// IFT3100H22 ~ lambert_330_fs.glsl

#version 330

// attributs interpolés à partir des valeurs en sortie du shader de sommets
in vec3 surface_position;
in vec3 surface_normal;

// attribut en sortie
out vec4 fragment_color;

// couleurs de réflexion du matériau
uniform vec3 color_ambient;
uniform vec3 color_diffuse;

// position d'une source de lumière
uniform vec3 spot_light_position;
uniform vec3 side_light_position;

// attribut de lumiere ambiente et addition avec materieux
uniform vec3 ambient_light;
vec3 mix_ambient = color_ambient + ambient_light;

// index pour savoir quel calcul d'illu faire selon les lumieres ouverte/fermer
uniform float lights_on;

//fonction qui calcul l'illum selon la position d'une lumiere qui est donner en argument
vec4 calcul_illum(vec3 light_pos)
{
  // re-normaliser la normale après interpolation (n)
  vec3 n = normalize(surface_normal);

  // calculer la direction de la surface vers la lumière (l) et calculer le niveau de réflexion diffuse (n • l)
  vec3 l = normalize(light_pos - surface_position);
  float reflection_diffuse = max(dot(n, l), 0.0);

  vec4 color_mix = vec4(mix_ambient + color_diffuse * reflection_diffuse, 1.0);

  return color_mix;
}

void main()
{
	
   // Condition pour savoir quel calcul d'illum dois etre fais en consequence des lumieres ouverte/fermer
  if (lights_on == 0)
  {
    fragment_color = vec4(mix_ambient, 1.0);
  }

  if (lights_on == 1)
  {
    fragment_color = calcul_illum(side_light_position);
  }

  if (lights_on == 2)
  {
    fragment_color = calcul_illum(spot_light_position);
  }

  if (lights_on == 3)
  {
    fragment_color = calcul_illum(spot_light_position) + calcul_illum(side_light_position) ;
  }
}
