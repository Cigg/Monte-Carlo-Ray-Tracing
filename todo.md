# TODO

_The scene should consist of 3 quadric surfaces and / or implicit surfaces (spheres, cubes) that are placed in a room that is closed on all sides except the one with the camera. The walls should be Lambertian reflectors. Two area light sources should be present in the scene._

Each step is an addition to the prevoius step.

### 1. A Monte-Carlo raytracer
-	Cornell Box
	- Scene setup
	- Camera
	-	View plane
	- Light sources
	- Intransparent object
	- Anti-aliasing?
- Monte Carlo Ray Tracing
	- Rendering equation
	- Integration
	- Russian roulette
	- Intersection
	- Specular reflection and refraction
	- Hemisphere sampling

### 2. Models intransparent objects using Lambertian reflection
- Lambertian reflections
	- Diffuse reflection

### 3. Models one intransparent Oren-Nayar reflector and one transparent reflector

- Intransparent Oren-Nayar reflector
- Transparent reflector
	- Glas sphere

### 4. Photon mapping

- Photon Mapping
	- Photon tracing
	- Photon storage
	- Radiance estimate

### Bonus: Parallelization

- OpenMP