/*
    static.frag
    by Spatial
    05 July 2013
*/

// A single iteration of Bob Jenkins' One-At-A-Time hashing algorithm.
uint hash( uint x ) {
    x += ( x << 10u );
    x ^= ( x >>  6u );
    x += ( x <<  3u );
    x ^= ( x >> 11u );
    x += ( x << 15u );
    return x;
}

// Compound versions of the hashing algorithm I whipped together.
uint hash( uvec2 v ) { return hash( v.x ^ hash(v.y)                         ); }
uint hash( uvec3 v ) { return hash( v.x ^ hash(v.y) ^ hash(v.z)             ); }
uint hash( uvec4 v ) { return hash( v.x ^ hash(v.y) ^ hash(v.z) ^ hash(v.w) ); }

// Construct a float with half-open range [0:1] using low 23 bits.
// All zeroes yields 0.0, all ones yields the next smallest representable value below 1.0.
float floatConstruct( uint m ) {
    const uint ieeeMantissa = 0x007FFFFFu; // binary32 mantissa bitmask
    const uint ieeeOne      = 0x3F800000u; // 1.0 in IEEE binary32

    m &= ieeeMantissa;                     // Keep only mantissa bits (fractional part)
    m |= ieeeOne;                          // Add fractional part to 1.0

    float  f = uintBitsToFloat( m );       // Range [1:2]
    return f - 1.0;                        // Range [0:1]
}

// Pseudo-random value in half-open range [0:1].
float random( float x ) { return floatConstruct(hash(floatBitsToUint(x))); }
float random( vec2  v ) { return floatConstruct(hash(floatBitsToUint(v))); }
float random( vec3  v ) { return floatConstruct(hash(floatBitsToUint(v))); }
float random( vec4  v ) { return floatConstruct(hash(floatBitsToUint(v))); }

#define M_PI 3.1415926535897932384626433832795

vec2 rot2(vec2 v, float angle) {
    float c = cos(angle);
    float s = sin(angle);
    
    return vec2(c * v.x + s * v.y, -s * v.x + c * v.y);
}

vec2 polar(vec2 v) {
    v = 2. * v - 1.;
    float theta = atan(v.y, v.x);
    float r = sqrt(dot(v, v));
    return vec2(r, theta / (2. * M_PI) + .5);
}

void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
    vec3 col = vec3(0, 0, 0);
    
    // Normalized pixel coordinates (from 0 to 1)
    vec2 uv = fragCoord / iResolution.y;
    //uv *= 2.;
    //uv = fract(uv);

    //uv = rot2(uv, iTime * .1);
    uv = polar(uv);
    uv *= vec2(10, 800);
    
    
    uv.x = -10. * (random(vec2(floor(uv.y), 1)) + .8) * iTime + ((random(vec2(floor(uv.y), 2)) * 2. - 1.) * .5 + 1.) * uv.x;
   	vec2 id = floor(uv);
    uv = fract(uv);
    
    col.r = random(id) < .1 ? uv.x : 0.0;
    
    
    //col.xy = uv;
    // Output to screen
    fragColor = vec4(col,1.0);
}