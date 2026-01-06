$input v_texcoord0

#include <bgfx_shader.sh>

uniform vec4 u_lightColor;    // couleur + intensité (a)

void main()
{
    vec2 uv = v_texcoord0;
    vec2 center = vec2(0.5, 0.5);

    float dist = distance(uv, center);
    float radius = 0.5;

    float falloff = 1.0 - smoothstep(0.0, radius, dist);
    falloff *= falloff;

    gl_FragColor = vec4(u_lightColor.rgb * u_lightColor.w * falloff, falloff * u_lightColor.a);
}
