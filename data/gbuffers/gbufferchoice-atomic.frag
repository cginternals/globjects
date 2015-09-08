#version 420

uniform int choice;
uniform sampler2D postprocessedSource;
uniform sampler2D colorSource;
uniform sampler2D normalSource;
uniform sampler2D worldCoordSource;
uniform sampler2D depthSource;

uniform unsigned int frameCount;

layout (binding = 0, offset = 0) uniform atomic_uint atomicCounter;

uniform float nearZ;
uniform float farZ;

layout (location = 0) out vec4 fragColor;

in vec2 v_uv;

void main()
{
    uint counter = atomicCounterIncrement(atomicCounter);
    
    vec4 color;
    if (counter > frameCount)
    {
        color = vec4(0.0, 0.0, 0.0, 1.0);
        // discard;
    }
    else
    {
        switch (choice)
        {
            case 1:
                color = texture(colorSource, v_uv);
            break;
            case 2:
                color = vec4((texture(normalSource, v_uv).rgb + 1.0) * 0.5, 1.0);
            break;
            case 3:
                color = vec4(texture(worldCoordSource, v_uv).rgb * 0.5 + 0.5, 1.0);
            break;
            case 4:
                float depth = texture(depthSource, v_uv).r;
                
                float d = (nearZ*farZ/4.0) / (farZ-depth*(farZ-nearZ));
                
                color = vec4(vec3(d), 1.0);
            break;
            default:
                color = texture(postprocessedSource, v_uv);
            break;
        }
        
        color = mix(vec4(1.0, 0.0, 0.0, 1.0), color, clamp(float(frameCount - counter) / 1000.0, 0.0, 1.0));
    }
    
    fragColor = color;
}
