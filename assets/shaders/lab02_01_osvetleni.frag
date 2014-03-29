#version 130 

//light
uniform vec3 lightPosition;

//material
uniform float shininess;

uniform float mode;

in vec3 gouraudColor;

in vec3 eyePosition, eyeNormal;

out vec4 fragColor;

void main()
{
    vec3 N = normalize(eyeNormal);
    vec3 L = normalize(lightPosition-eyePosition);

    //Blin-Phong model
    vec3 lightColor = vec3(0.1, 0.0, 0.0);
    float diffuse = dot(L, N);
    if(diffuse >= 0.0)
    {
        lightColor += vec3(diffuse, 0.0, 0.0);

        if(mode > 0)
        {
            //Phong model
            vec3 V = normalize(-eyePosition);
            vec3 R = reflect(-L, N);
            float specular = pow(dot(R, V), shininess);

            if(specular >= 0.0)
            {
                lightColor += vec3(specular);
            }
        }
        else
        {
            //Blinn-Phong model
            vec3 V = normalize(-eyePosition);
            vec3 H = normalize(L + V);
            float specular = pow(dot(N, H), shininess);

            if(specular >= 0.0)
            {
                lightColor += vec3(specular);
            }
        }
    }

    if(abs(mode) < 0.5)
        fragColor = vec4(gouraudColor, 1.0);
    else
        fragColor = vec4(lightColor, 1.0);
}
