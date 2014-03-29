#version 130 

in vec3 position, normal;

uniform mat4 mvp, mv;
uniform mat3 mn;

//light
uniform vec3 lightPosition;

//material
uniform float shininess;

uniform float mode;

out vec3 gouraudColor;

out vec3 eyePosition, eyeNormal;

void main()
{
    gl_Position = mvp*vec4(position,1);

    eyePosition = (mv*vec4(position,1)).xyz;
    eyeNormal = normalize(mn*normal);
    
    vec3 N = eyeNormal;
    vec3 L = normalize(lightPosition-eyePosition);

    //Lighting
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

    gouraudColor = lightColor;
}
