#version 330 core

in vec2 TexCoord;

out vec4 color;

uniform vec3 addCoeffs;
uniform vec2 initPoint;
uniform int maxIter;

void main()
{
    vec3 mclr;
    vec2 c, z;
    float cl;
    int i;
    
    c = initPoint;
    z.x = 4.0f*TexCoord.x - 2.0f;
    z.y = 3.0f*TexCoord.y - 1.5f;
    for(i = 1; i < maxIter; ++i)
    {
        float imz = z.x;
        float rez = z.y;
        
        if(rez*rez + imz*imz > 4.0f)
        {
            break;
        }
        
        //z = pow(z, 2) + c;
        z.x = rez*rez - imz*imz;
        z.y = 2*rez*imz;
        z = z + c;
    }
    
    cl = sqrt(float(i)/maxIter);
    mclr.r = (sin(addCoeffs.r*cl) + 1.0f)/2;
    mclr.g = (sin(addCoeffs.g*cl) + 1.0f)/2;
    mclr.b = (sin(addCoeffs.b*cl) + 1.0f)/2;
    
    color = vec4(mclr, 1.0f);
}