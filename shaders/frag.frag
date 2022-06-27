#version 460 core

in vec2 FragPos;

out vec4 FragColor;

uniform int maxIterations;
uniform vec2 Z0;

void main()
{
    dvec2 res = Z0;

    for(int i = 0; i < maxIterations; i++)
    {
        double x = res.x;
        double y = res.y;
        res.x = x * x - y * y;
        res.y = 2 * x * y;
        res += FragPos;

        double l = res.x * res.x + res.y * res.y;
        if(l > 4.0)
        {
            double badness = (double(i) / maxIterations);
            dvec3 cl = dvec3(1.0) - badness;
            cl *= dvec3(abs(res.x) / l, abs(res.y) / l, 1.0);
            FragColor = vec4(cl, 1.0);
            return;
        }
    }
    FragColor = vec4(0.0, 0.0, 0.0, 1.0);
}