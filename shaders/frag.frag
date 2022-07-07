#version 460 core

in vec2 FragPos;

out vec4 FragColor;

uniform int maxIterations;
uniform vec2 Z0;

uniform float zoom;

void main()
{
    vec2 res = Z0 + FragPos * zoom;
    vec2 C = Z0 + FragPos * zoom;

    for(int i = 0; i < maxIterations; i++)
    {
        float x = res.x;
        float y = res.y;
        res.x = x * x - y * y;
        res.y = 2 * x * y;
        res += C;


        float l = res.x * res.x + res.y * res.y;
        if(l > 4.0)
        {
            float badness = (float(i) / maxIterations);
            dvec3 cl = dvec3(1.0) - badness;
            cl *= dvec3(abs(res.x) / l, abs(res.y) / l, 1.0);
            FragColor = vec4(cl, 1.0);
            return;
        }
    }
    FragColor = vec4(0.0, 0.0, 0.0, 1.0);
}