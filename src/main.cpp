#include "dwgSimpleGraphics.h"
#include <vector>
#include <stdio.h>
#include <fstream>
#include <ctime>
#include <algorithm>

#define OPT_IMPL 0

float RandomFloat(float a, float b)
{
	float random = ((float)rand()) / (float)RAND_MAX;
	float diff = b - a;
	float r = random * diff;
	return a + r;
}

Vector3 RandomColor()
{
	float x = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	float y = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	float z = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	Vector3 color = Vector3(x, y, z);
	return color;
}

Vector3 RandomCubeVector(float parameter)
{
	float x = RandomFloat(-parameter, parameter);
	float y = RandomFloat(-parameter, parameter);
	float z = RandomFloat(-parameter, parameter);
	return Vector3(x, y, z);
}
bool IsAbroad(Vector3& position, float border)
{
	if (position.getX() > border)
		position = Vector3(border, position.getY(), position.getZ());
	else if(position.getX() < -border)
		position = Vector3(-border, position.getY(), position.getZ());
	else if (position.getY() > border)
		position = Vector3(position.getX(), border, position.getZ());
	else if (position.getY() < -border)
		position = Vector3(position.getX(), -border, position.getZ());
	else if (position.getZ() > border)
		position = Vector3(position.getX(), position.getY(), border);
	else if (position.getZ() < -border)
		position = Vector3(position.getX(), position.getY(), -border);
		

	if (position.getX() >= border || position.getX() <= -border || position.getY() >= border || position.getY() <= -border || position.getZ() >= border || position.getZ() <= -border)
	{
		return true;
	}
	else
	{
		return false;
	}
}


bool IsColliding(Vector3 ball_1, Vector3 ball_2, float radius)
{
	Vector3 d = ball_1 - ball_2;
	float len = length(d);
	if (len < 2 * radius)
	{
		return true;
	}
	else
	{
		return false;
	}
}

Vector3 Reflect(Vector3 vectorA,Vector3 velA, Vector3 vectorB)
{
	Vector3 n = vectorA - vectorB;
	return 2.0f * dot(-velA, n) * n + velA;
}


class Planet
{
public:
	Vector3 position;
	Vector3 color;
	Vector3 direction;
	float speed = 1.0f;

	Planet(Vector3 pos, Vector3 col, Vector3 vel)
	{
		position = pos;
		color = col;
		direction = vel;
	}

	Planet()
	{
		
	}
};

//int partition(int* arr, int start, int end)
//{
//	int index = 0, pivotElement = arr[end], pivotIndex;
//	int* temp = new int[end - start + 1];
//	for (int i = start; i <= end; i++)
//	{
//		if (arr[i] < pivotElement)
//		{
//			temp[index] = arr[i];
//			index++;
//		}
//	}
//	temp[index] = pivotElement;
//	index++;
//	for (int i = start; i < end; i++)
//	{
//		if (arr[i] > pivotElement)
//		{
//			temp[index] = arr[i];
//			index++;
//		}
//	}
//
//	index = 0;
//	for (int i = start; i <= end; i++)
//	{
//		if (arr[i] == pivotElement)
//		{
//			pivotIndex = i;
//		}
//		arr[i] = temp[index];
//		index++;
//	}
//	return pivotIndex;
//}
//
//void quickSort(int* arr, int start, int end)
//{
//	if (start < end)
//	{
//		int partitionIndex = partition(arr, start, end);
//		quickSort(arr, start, partitionIndex);
//		quickSort(arr, partitionIndex + 1, end);
//	}
//	return;
//}

bool comparePlanetByX(const Planet& a, const Planet& b)
{
	return a.position.getX() < b.position.getX();
}




// entry point for the app (using WinMain, so no console appears, just the rendering window)
int WinMain()
{
	// init window and rendering with given width, height, and title of the window
	if (!dwgInitApp(1600, 900, "DwG - Game Math"))
		return 1;
	// origin

	Vector3 zero = Vector3(0.0f, 0.0f, 0.0f);

	//earth

	Vector3 earth_pos = Vector3(1.0f, 1.0f, -1.0f);
	Vector3 acceleration = Vector3(-0.1);
	Vector3 velocity = Vector3(0.0f, 0.3f, 0.0f);

	Vector3 moon_pos = Vector3(1.5f, 1.5f, -1.5f);
	Vector3 moon_velocity = Vector3(0.0f, 0.3f, 0.0f);
	Vector3 moon_acceleration = Vector3(-0.2);

	Vector3 sun_pos = zero;
	

	const float gravity = 9.81f;
	/* 
	p - pozycja[m]
	v - prêdkoœæ [m/s]
	a - przyspieszenie [m/s^2]


	m = m/s * s
	p = v * t
	v = a * t

	prêdkoœæ - kierunek + wartoœæ (szybkoœæ - skalar)
	*/ 

	// how many planets
	const int planets_num = 6000;
	Planet myPlanets[planets_num];
	float random_range = 40.0f;
	float random_dir = 0.3f;
	float border =	39.0f;
	float r = .2f;
	static std::ofstream g_log("log.txt");
	
	for(int i = 0; i < planets_num; i++)
	{
		myPlanets[i] = Planet(RandomCubeVector(random_range), RandomColor(), RandomCubeVector(random_dir));
	}
	
	
	//myPlanets[0].position.getX();
	//quickSort(myPlanets, 0, planets_num);


	// main game loop, each iteration is a single frame
	while (!dwgShouldClose())
	{

		const double globalTime = dwgGlobalTime();	// global time - time since the start of the app
		const float dt = dwgDeltaTime();			// delta time - time since last frame
		g_log << dt * 1000.0f << std::endl;
		//time startTime = get_time();
		// x,y,z basis vectors
		dwgDebugLine(zero, Vector3(1.0f, 0.0f, 0.0f), Vector3(1.0f, 0.0f, 0.0f));
		dwgDebugLine(zero, Vector3(0.0f, 1.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f));
		dwgDebugLine(zero, Vector3(0.0f, 0.0f, 1.0f), Vector3(0.0f, 0.0f, 1.0f));

		//for (Planet& p : myPlanets)
		std::sort(std::begin(myPlanets), std::end(myPlanets), comparePlanetByX);


		for (int i = 0; i < planets_num; i++)
		{
			Planet& p = myPlanets[i];
			p.color = Vector3(1.0f, 1.0f, 1.0f);
			if (IsAbroad(p.position, border))
			{
				p.direction = -p.direction;
			}
			p.position += normalize(p.direction) * p.speed * dt;
			dwgDebugSphere(p.position, Vector3(r, r, r), p.color);

#if OPT_IMPL == 0
			for(int j = i + 1; j < planets_num; j++)
			{
				Planet& k = myPlanets[j];
				if (abs(k.position.getX() - p.position.getX()) <= 2* r)
				{
					Vector3 diff = p.position - k.position;

					if (length(diff) <= 2 * r)
					{

						p.position = p.position + (normalize(diff) * (2 * r - length(diff)) / 2);
						k.position = k.position + (normalize(-diff) * (2 * r - length(-diff)) / 2);
						p.color = Vector3(1.0f, 0.0f, 0.0f);
						Vector3 ref = Reflect(k.position, k.direction, p.position);
						k.direction = ref;
						Vector3 ref2 = Reflect(p.position, p.direction, k.position);
						p.direction = ref2;
						// resolve(?) collision
					}
				}
				else
				{
					break;
				}
			}
#elif OPT_IMPL == 1
			for (int j = i + 1; j < planets_num; j++)
			{
				Planet& l = myPlanets[j];
				Vector3 diff = p.position - l.position;
				if (IsColliding(p.position, l.position, r) && length(diff) > 0.0f)
				{
					p.position = p.position + (normalize(diff) * (2*r - length(diff))/2);
					l.position = l.position + (normalize(-diff) * (2*r - length(-diff))/2);
					l.color = Vector3(1.0f, 0.0f, 0.0f);
					Vector3 ref = Reflect(l.position, l.direction, p.position);
					l.direction = ref;
					Vector3 ref2 = Reflect(p.position,p.direction, l.position);
					p.direction = ref2;
					

				}
				else
				{
					//l.color = Vector3(0.0f, 1.0f, 1.0f);
				}
			} 
#endif
		}
		
		// physics
		/*Vector3 earth_direction = normalize(sun_pos - earth_pos);
		acceleration = earth_direction * 0.1f;
		velocity += acceleration * dt;
		earth_pos += velocity * dt;
		dwgDebugSphere(earth_pos, Vector3(.1f, .1f, .1f), Vector3(1.0f, 0.0f, 1.0f));*/

		/*Vector3 moon_direction = normalize(earth_pos - moon_pos);
		moon_acceleration = moon_direction * 0.5f;
		moon_velocity += moon_acceleration * dt;
		moon_pos += moon_velocity * dt * 0.5f;
		dwgDebugSphere(moon_pos, Vector3(0.1f, 0.1f, 0.1f), Vector3(1.0f, 0.0f, 0.0f));*/
		

		


		//sphere coordinates x,y,z
		Vector3 step_x = Vector3(earth_pos.getX(), 0.0f, 0.0f);
		Vector3 step_y = Vector3(0.0f, earth_pos.getY(), 0.0f);
		Vector3 step_z = Vector3(0.0f, 0.0f, earth_pos.getZ());

		// drawing steps from origin to sphere_pos
		/*
		dwgDebugLine(zero, step_x, Vector3(1.0f, 1.0f, .0f));
		dwgDebugLine(step_x, step_x + step_y, Vector3(1.0f, 0.0f, 1.0f));
		dwgDebugLine(step_x + step_y, step_x + step_y + step_z, Vector3(0.0f, 1.0f, 1.0f));
		*/

		dwgDebugSphere(sun_pos, Vector3(.1f, .1f, .1f), Vector3(1.0f, 1.0f, 0.0f));

		// prepare camera
		const Point3 eye = { border * 1.5f * sinf(globalTime * 0.4f), border * 1.5f * cosf(globalTime * 0.4f), 3.0f};				// eye position
		const Point3 at = { 0.0f, 0.0f, 0.0f };					// what we're looking at
		const Vector3 up = { 0.0f, 0.0f, 1.0f };				// where's the up direction for camera
		const Matrix4 camera = Matrix4::lookAt(eye, at, up);	// create camera view matrix
		const float fov = 120.0f;								// field of view - angle of view of the camera (for perspective)

		// draw scene
		dwgRender(camera, fov);
	}

	// release rendering and window app
	dwgReleaseApp();

	return 0;
}



