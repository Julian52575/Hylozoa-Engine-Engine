$input v_texcoord0

uniform vec4 u_lightPos;     // x, y (screen space), z, w
uniform vec4 u_lightColor;   // r, g, b, intensity
uniform vec4 u_screenSize;   // width, height

void main()
{
    // 1. Calculate pixel position in screen space
    vec2 pixelPos = v_texcoord0 * u_screenSize.xy;

    // 2. Calculate distance from light center
    float dist = distance(pixelPos, u_lightPos.xy);

    // 3. Calculate attenuation
    // Note: u_lightColor.w is your intensity. Adjust the multiplier (150.0) as needed.
    float radius = u_lightColor.w * 150.0;
    
    // Smooth attenuation
    float atten = 1.0 - smoothstep(0.0, radius, dist);

    // 4. Output Result
    // Use Pre-multiplied alpha if your blend mode expects it, 
    // or just (color * atten, atten) for standard adding.
    vec3 finalColor = u_lightColor.rgb * atten;
    
    gl_FragColor = vec4(finalColor, atten);
    
    // DEBUG: Uncomment the line below to test if the quad is rendering at all (Red Square)
    // gl_FragColor = vec4(u_screenSize.x / 2000.0, 0.0, 0.0, 1.0);
}