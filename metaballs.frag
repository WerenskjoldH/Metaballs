uniform int numMetaballs;

uniform float metaballArray[200];

uniform float screenHeight;

uniform float metaballRadiusSquared;

void main()
{
	vec3 color;
	
	vec2 fragPosition = gl_FragCoord.xy;
	fragPosition.y = -1 * fragPosition.y + screenHeight;

	// Using an inverse squared function is extremely simple to explain, as such we use it for the sake of learning. That said, understand that there are more/better options!
	// Artifacts of this method include growth in size of metaballs as more are added to the scene
	float sum = 0.0, dist = 0.0;
	for(int i = 0; i < numMetaballs; i++)
	{	
		dist = distance(fragPosition, vec2(metaballArray[2*i], metaballArray[2*i+1]));
		sum += 100.0 * ((metaballRadiusSquared)/ ((dist*dist) + 0.001));		
	}

	// Background, outline, and center color

	if(sum <= 15)
		color = vec3(0.4, 0.3, 0.5);
	else if(sum <= 25)
		color = vec3(0.5, 0.5, 0.5);
	else if(sum <= 50)
		color = vec3(0, 0, 0);

	gl_FragColor = vec4(color, 1.0);
}