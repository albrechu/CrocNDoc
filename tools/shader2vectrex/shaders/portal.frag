/**
* My first shader - an attempt to create an abstract portal to another dimension!
*
* Drawing upon the code described by 'phil' at https://www.shadertoy.com/view/ltBXRc
**/


/**
* Applies smooth displacement to the circumference of the circle.
**/
float variation(vec2 v1, vec2 v2, float strength, float speed) {
	return sin(
        dot(normalize(v1), normalize(v2)) * strength + iTime * speed
    ) / 100.;
}

/**
* Draws a circle with smooth variation to its circumference over time. 
* @rad - the radius of the circle
* @width - how thick the circle is
* @index - what circle is currently being drawn? Currently, every odd circle is drawn with opposing displacement for effect
**/
vec3 paintCircle (vec2 uv, vec2 center, float rad, float width, float index) {
    vec2 diff = center-uv;
    float len = length(diff);
    float scale = rad;
	float mult = mod(index, 2.) == 0. ? 1. : -1.; 
    len += variation(diff, vec2(rad*mult, 1.0), 7.0*scale, 2.0);
    len -= variation(diff, vec2(1.0, rad*mult), 7.0*scale, 2.0);
    float circle = smoothstep((rad-width)*scale, (rad)*scale, len) - smoothstep((rad)*scale, (rad+width)*scale, len);
    return vec3(circle);
}

/**
* A ring consists of a wider faded circle with an overlaid white solid inner circle. 
**/
vec3 paintRing(vec2 uv, vec2 center, float radius, float index){
     //paint color circle
    vec3 color = paintCircle(uv, center, radius, 0.075, index);
    //this is where the blue color is applied - change for different mood
    color *= vec3(0.3,0.85,1.0);
    //paint white circle
    color += paintCircle(uv, center, radius, 0.015, index);
    return color;
}


void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
    //define our primary 'variables'
	vec2 uv = fragCoord.xy / iResolution.xy;
    const float numRings = 20.;
    const vec2 center = vec2(0.5);
    const float spacing = 1. / numRings;
    const float slow = 30.;
    const float cycleDur = 1.;
    const float tunnelElongation = .25;
    float radius = mod(iTime/slow, cycleDur);
    vec3 color;

    //this provides the smooth fade black border, which we will mix in later
    float border = 0.25;
    vec2 bl = smoothstep(0., border, uv); // bottom left
    vec2 tr = smoothstep(0., border, 1.-uv); // top right
    float edges = bl.x * bl.y * tr.x * tr.y;

    //push in the left and right sides to make the warp square
    uv.x *= 1.5;
    uv.x -= 0.25; 
    
    //do the work
    for(float i=0.; i<numRings; i++){
   		color += paintRing(uv, center, tunnelElongation*log(mod(radius + i * spacing, cycleDur)), i ); //these are the fast circles
        color += paintRing(uv, center, log(mod(radius + i * spacing, cycleDur)), i); //these are essentially the same but move at a slower pace
    }

    //combined, these create a black fade around the edges of our screen
    color = mix(color, vec3(0.), 1.-edges); 
    color = mix(color, vec3(0.), distance(uv, center));
    //boom!
	fragColor = vec4(color, 1.0);
}
