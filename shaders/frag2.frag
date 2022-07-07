#version 460 core

in vec2 FragPos;

out vec4 FragColor;

uniform int maxIterations;
uniform vec2 Z0;
uniform dvec2 Zn;

uniform float zoom;


vec2 cmul(vec2 l, vec2 r)
{
    return vec2(l.x * r.x - l.y * r.y, l.x * r.y + l.y * r.x);
}

void main()
{
    vec2 dc = FragPos * zoom;
    vec2 dz = vec2(0.0);
    vec2 z = vec2(0.0);
    vec2 c = Z0;

    for(int i = 0; i < maxIterations; i++)
    {
        dz = cmul(2.0*z + dz, dz) + dc;
        z = cmul(z, z) + c;

        float l = dot(dz, dz);
        if(l > 4.0)
        {
            float badness = (float(i) / maxIterations);
            vec3 cl = vec3(1.0) - badness;
            cl *= vec3(1.0 + sin(dz.x) / l, 1- cos(dz.y) / l, 1.0);
            FragColor = vec4(cl, 1.0);
            return;
        }
    }
    FragColor = vec4(0.0, 0.0, 0.0, 1.0);
}