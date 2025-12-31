$input v_color0, v_texcoord0 // Reçoit les UVs du vertex shader

#include <bgfx_shader.sh>

SAMPLER2D(s_texColor, 0); // Doit correspondre au nom de l'uniform dans ton C++

void main() {
    // On multiplie la texture par la couleur pour permettre de teinter le sprite
    gl_FragColor = texture2D(s_texColor, v_texcoord0) * v_color0;
}