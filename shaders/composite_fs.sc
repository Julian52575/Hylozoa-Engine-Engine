$input v_texcoord0
#include <bgfx_shader.sh>

SAMPLER2D(s_texColor, 0); // Contenu du monde
SAMPLER2D(s_texLight, 1); // Contenu de la Lightmap

uniform vec4 u_ambientColor; //rgb = couleur ambiante, a = intensité

void main()
{
    vec2 flippedUV = vec2(v_texcoord0.x, 1.0 - v_texcoord0.y);

    vec4 worldColor = texture2D(s_texColor, flippedUV);
    vec4 lightColor = texture2D(s_texLight, flippedUV);

    vec3 ambient = u_ambientColor.rgb * u_ambientColor.a;
 
    vec3 finalRGB = (worldColor.rgb * ambient) + lightColor.rgb;

    gl_FragColor = vec4(finalRGB, worldColor.a);
}