# TODO

_The scene should consist of 3 quadric surfaces and / or implicit surfaces (spheres, cubes) that are placed in a room that is closed on all sides except the one with the camera. The walls should be Lambertian reflectors. Two area light sources should be present in the scene._

Each step is an addition to the prevoius step.

### 1. A Monte-Carlo raytracer
-	Scene
	-	~~View plane~~
	- ~~Camera~~
	- ~~Shapes~~
	- ~~Light sources~~
		- Two area light sources in the ceiling
	- ~~Intransparent object~~
- Monte Carlo Ray Tracing
	- ~~Shadow rays~~
	- ~~BRDF~~
	- ~~Random ray~~
	- Russian roulette
		- ~~Absorption and number of reflections~~
	- ~~Intersection~~
	- Specular reflection and refraction
		- ~~Direct and Indirect Illumination~~

### 2. Models intransparent objects using Lambertian reflection
- ~~Lambertian reflections~~

### 3. Improve results in step 1 and 2
- Refactor if needed to implement step 4 and 5

### 4. Models one intransparent Oren-Nayar reflector and one transparent reflector
- Intransparent Oren-Nayar reflector
- Transparent reflector
	- Glas sphere

### 5. Photon mapping
- Photon Mapping
	- Photon tracing
	- Photon storage
	- Radiance estimate

### Bonus: Parallelization
- OpenMP