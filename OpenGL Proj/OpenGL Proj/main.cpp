#include <glew.h>
#include <freeglut.h>

#include "SystemManager.h"

#pragma comment(lib, "glew32.lib") 

// Main routine.
int main(int* argc, char** argv)
{
	SystemManager* system = new SystemManager(argc, argv);
	system->myself = system;
	system->renderer->myself = system->renderer; //Workaround due to GLUT being a C API
	system->physics->global_dampening = 5.0f;

	system->render_meshes["cube"] = OBJReader("vertex_cube.obj");
	system->render_meshes["cube"]->material = {
		glm::vec4(1.0, 1.0, 1.0, 1.0),
		glm::vec4(1.0, 1.0, 1.0, 1.0),
		glm::vec4(1.0, 1.0, 1.0, 1.0),
		glm::vec4(0.0, 0.0, 0.0, 1.0),
		80.0
	};
	system->render_meshes["cube"]->bitmap_texture = BitMapReader("blank.bmp");

	system->render_meshes["sphere"] = OBJReader("sphere.obj");
	system->render_meshes["sphere"]->material = {
		glm::vec4(1.0, 1.0, 1.0, 1.0),
		glm::vec4(1.0, 1.0, 1.0, 1.0),
		glm::vec4(1.0, 1.0, 1.0, 1.0),
		glm::vec4(0.0, 0.0, 0.0, 1.0),
		80.0
	};
	system->render_meshes["sphere"]->bitmap_texture = BitMapReader("blank.bmp");	
	
	system->render_meshes["plane"] = OBJReader("vertex_plane.obj");
	system->render_meshes["plane"]->material = {
		glm::vec4(1.0, 1.0, 1.0, 1.0),
		glm::vec4(1.0, 1.0, 1.0, 1.0),
		glm::vec4(0.1, 0.1, 0.1, 1.0),
		glm::vec4(0.0, 0.0, 0.0, 1.0),
		16.0
	};
	system->render_meshes["plane"]->bitmap_texture = BitMapReader("grass.bmp");

	system->render_meshes["plane_2"] = OBJReader("vertex_plane.obj");
	system->render_meshes["plane_2"]->material = {
		glm::vec4(1.0, 1.0, 1.0, 1.0),
		glm::vec4(1.0, 1.0, 1.0, 1.0),
		glm::vec4(1.0, 1.0, 1.0, 1.0),
		glm::vec4(0.0, 0.0, 0.0, 1.0),
		160.0
	};
	system->render_meshes["plane_2"]->bitmap_texture = BitMapReader("ice.bmp");

	system->render_meshes["plane_3"] = OBJReader("vertex_plane.obj");
	system->render_meshes["plane_3"]->material = {
		glm::vec4(1.0, 1.0, 1.0, 1.0),
		glm::vec4(1.0, 1.0, 1.0, 1.0),
		glm::vec4(0.1, 0.1, 0.1, 1.0),
		glm::vec4(0.0, 0.0, 0.0, 1.0),
		16.0
	};
	system->render_meshes["plane_3"]->bitmap_texture = BitMapReader("spiral.bmp");

	system->render_meshes["hovercraft"] = OBJReader("Hovertank4.obj");
	system->render_meshes["hovercraft"]->material = {
		glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
		glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
		glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
		glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
		50.0
	};
	system->render_meshes["hovercraft"]->bitmap_texture = BitMapReader("Hovercraft.bmp");
		
	system->render_meshes["skybox"] = OBJReader("SkyBoxOBJ.obj");
	system->render_meshes["skybox"]->material = {
		glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
		glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
		glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
		glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
		0.0,
		false
	};
	system->render_meshes["skybox"]->bitmap_texture = BitMapReader("SkyBoxLayout10.bmp");

	system->render_meshes["light_pole"] = OBJReader("Lightpole.obj");
	system->render_meshes["light_pole"]->material = {
		glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
		glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
		glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
		glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
		24.0
	};
	system->render_meshes["light_pole"]->bitmap_texture = BitMapReader("Lightpole.bmp");

	system->render_meshes["race_track"] = OBJReader("Track.obj");
	system->render_meshes["race_track"]->material = {
		glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
		glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
		glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
		glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
		0.0
	};
	system->render_meshes["race_track"]->bitmap_texture = BitMapReader("Lightpole.bmp");

	system->render_meshes["mill"] = OBJReader("Mill.obj");
	system->render_meshes["mill"]->material = {
		glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
		glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
		glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
		glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
		8.0
	};
	system->render_meshes["mill"]->bitmap_texture = BitMapReader("Mill.bmp");

	system->render_meshes["wheel"] = OBJReader("Wheel.obj");
	system->render_meshes["wheel"]->material = {
		glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
		glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
		glm::vec4(0.1f, 0.1f, 0.1f, 1.0f),
		glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
		8.0
	};
	system->render_meshes["wheel"]->bitmap_texture = BitMapReader("Wheel.bmp");

	Entity* hovercraft = system->CreateEntity();
	hovercraft->AddComponent(eRender);
	((RenderComponent*)hovercraft->components[eRender])->mesh = "hovercraft";
	hovercraft->AddComponent(eCollider);
	system->collider_meshes[hovercraft] = new OBBCollider(hovercraft, glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f), glm::vec3(15.f, 10.f, 25.f));
	system->collider_meshes[hovercraft]->restitution_coefficient = 0.5f;
	system->collider_meshes[hovercraft]->response_type = eLinear;
	hovercraft->AddComponent(eLight);
	((LightComponent*)hovercraft->components[eLight])->Setup(eSpotLight, 1.f, 0.f, 0.f, 25.f, glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec4(1.0f), glm::vec4(1.0f), glm::vec4(0.0f, 25.0f, 10.0f, 0.0f));
	hovercraft->AddComponent(eMotion);
	((MotionComponent*)hovercraft->components[eMotion])->local_dampening = 0.0f;
	((MotionComponent*)hovercraft->components[eMotion])->mass = 20.0f;
	((MotionComponent*)hovercraft->components[eMotion])->inertia = 20.0f;
	system->buffer->SetEntityTransformPosition(hovercraft, glm::vec3(55.f, 10.f, 50.f));
	system->buffer->SetEntityTransformScale(hovercraft, glm::vec3(2.0f, 2.0f, 2.0f));

	Entity* mill = system->CreateEntity();
	mill->AddComponent(eRender);
	((RenderComponent*)mill->components[eRender])->mesh = "mill";
	system->buffer->SetEntityTransformPosition(mill, glm::vec3(300.f, 0.f, 300.f));
	system->buffer->SetEntityTransformRotation(mill, glm::vec3(0, 180.f, 0));

	Entity* wheel = system->CreateEntity();
	wheel->AddComponent(eRender);
	((RenderComponent*)wheel->components[eRender])->mesh = "wheel";
	wheel->AddComponent(eMotion);
	((MotionComponent*)wheel->components[eMotion])->local_dampening = -system->physics->global_dampening;
	((MotionComponent*)wheel->components[eMotion])->rotational_forces.emplace_back(AngularForce(glm::vec3(0.f, 5.f, 0.f), glm::vec3(1.f, 0.f, 0.f), 5.0));
	system->buffer->SetEntityTransformPosition(wheel, glm::vec3(300.f, 100.f, 275.f));
	system->buffer->SetEntityTransformRotation(wheel, glm::vec3(0, 180.f, 0));

	Entity* light_pole_1 = system->CreateEntity();
	light_pole_1->AddComponent(eRender);
	((RenderComponent*)light_pole_1->components[eRender])->mesh = "light_pole";
	light_pole_1->AddComponent(eLight);
	((LightComponent*)light_pole_1->components[eLight])->Setup(ePointLight, 64.f, 0.02f, 0.f, 0.f, glm::vec4(0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec4(1.0f), glm::vec4(1.0f), glm::vec4(1.0f, 48.0f, 1.0f, 1.0f));
	system->buffer->SetEntityTransformPosition(light_pole_1, glm::vec3(0, 0, 100));
	system->buffer->SetEntityTransformScale(light_pole_1, glm::vec3(3.f, 3.f, 3.f));

	Entity* light_pole_2 = system->CreateEntity();
	light_pole_2->AddComponent(eRender);
	((RenderComponent*)light_pole_2->components[eRender])->mesh = "light_pole";
	light_pole_2->AddComponent(eLight);
	((LightComponent*)light_pole_2->components[eLight])->Setup(ePointLight, 64.f, 0.02f, 0.f, 0.f, glm::vec4(0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec4(1.0f), glm::vec4(1.0f), glm::vec4(1.0f, 48.0f, 1.0f, 1.0f));
	system->buffer->SetEntityTransformPosition(light_pole_2, glm::vec3(-300, 0, 500));
	system->buffer->SetEntityTransformScale(light_pole_2, glm::vec3(3.f, 3.f, 3.f));

	Entity* col_cube_2 = system->CreateEntity();
	col_cube_2->AddComponent(eRender);
	((RenderComponent*)col_cube_2->components[eRender])->mesh = "cube";
	col_cube_2->AddComponent(eMotion);
	((MotionComponent*)col_cube_2->components[eMotion])->mass = 10.0f;
	((MotionComponent*)col_cube_2->components[eMotion])->inertia = 10.0f;
	col_cube_2->AddComponent(eCollider);
	system->collider_meshes[col_cube_2] = new SphereCollider(col_cube_2, glm::vec3(0, 0, 0), 7.5f);
	system->collider_meshes[col_cube_2]->response_type = eComposition;
	system->buffer->SetEntityTransformPosition(col_cube_2, glm::vec3(200.f, 10.f, -100.f));

	Entity* col_cube_3 = system->CreateEntity();
	col_cube_3->AddComponent(eRender);
	((RenderComponent*)col_cube_3->components[eRender])->mesh = "cube";
	col_cube_3->AddComponent(eMotion);
	((MotionComponent*)col_cube_3->components[eMotion])->mass = 10.0f;
	((MotionComponent*)col_cube_3->components[eMotion])->inertia = 10.0f;
	col_cube_3->AddComponent(eCollider);
	system->collider_meshes[col_cube_3] = new OBBCollider(col_cube_3, glm::vec3(0.f, 0.f, 0.f),glm::vec3(0.f,0.f,0.f), glm::vec3(5.f, 5.f, 5.f));
	system->collider_meshes[col_cube_3]->response_type = eComposition;
	system->buffer->SetEntityTransformPosition(col_cube_3, glm::vec3(200.f, 10.f, -50.f));

	Entity* col_cube_4 = system->CreateEntity();
	col_cube_4->AddComponent(eRender);
	((RenderComponent*)col_cube_4->components[eRender])->mesh = "cube";
	col_cube_4->AddComponent(eMotion);
	((MotionComponent*)col_cube_4->components[eMotion])->mass = 10.0f;
	((MotionComponent*)col_cube_4->components[eMotion])->inertia = 10.0f;
	col_cube_4->AddComponent(eCollider);
	system->buffer->SetEntityTransformScale(col_cube_4, glm::vec3(1, 1, 1));
	system->collider_meshes[col_cube_4] = new AABBCollider(col_cube_4, glm::vec3(0, 0, 0), glm::vec3(5, 5, 5));
	system->collider_meshes[col_cube_4]->response_type = eComposition;
	system->buffer->SetEntityTransformPosition(col_cube_4, glm::vec3(200.f, 10.f, 0.f));

	Entity* track = system->CreateEntity();
	track->AddComponent(eRender);
	((RenderComponent*)track->components[eRender])->mesh = "race_track";
	system->buffer->SetEntityTransformPosition(track, glm::vec3(0.f, 1.f, 0.f));
	system->buffer->SetEntityTransformScale(track, glm::vec3(8.0f, 8.0f, 8.0f));

	Entity* plane_1 = system->CreateEntity();
	plane_1->AddComponent(eRender);
	((RenderComponent*)plane_1->components[eRender])->mesh = "plane";
	system->buffer->SetEntityTransformPosition(plane_1, glm::vec3(0.f, 0.f, 0.f));
	system->buffer->SetEntityTransformScale(plane_1, glm::vec3(15.0f, 1.0f, 15.0f));

	Entity* plane_2 = system->CreateEntity();
	plane_2->AddComponent(eRender);
	((RenderComponent*)plane_2->components[eRender])->mesh = "plane_2";
	plane_2->AddComponent(eMotion);
	plane_2->AddComponent(eCollider);
	system->collider_meshes[plane_2] = new AABBCollider(plane_2, glm::vec3(0, 0, 0), glm::vec3(100, 50, 100), false);
	system->collider_meshes[plane_2]->dampen = -20.0f;
	system->collider_meshes[plane_2]->response_type = eDampen;
	system->buffer->SetEntityTransformPosition(plane_2, glm::vec3(-400.f, 1.f, -100.f));
	system->buffer->SetEntityTransformScale(plane_2, glm::vec3(2.0f, 1.0f, 2.0f));
	
	Entity* plane_3 = system->CreateEntity();
	plane_3->AddComponent(eRender);
	((RenderComponent*)plane_3->components[eRender])->mesh = "plane_3";
	plane_3->AddComponent(eMotion);
	plane_3->AddComponent(eCollider);
	system->collider_meshes[plane_3] = new AABBCollider(plane_3, glm::vec3(0, 0, 0), glm::vec3(100, 50, 100), false);
	system->collider_meshes[plane_3]->dampen = 50.0f;
	system->collider_meshes[plane_3]->response_type = eDampen;
	system->buffer->SetEntityTransformPosition(plane_3, glm::vec3(-400.f, 1.f, 100.f));
	system->buffer->SetEntityTransformScale(plane_3, glm::vec3(2.0f, 1.0f, 2.0f));


	/*
	Entity* plane_4 = system->CreateEntity();
	plane_4->AddComponent(eRender);
	((RenderComponent*)plane_4->components[eRender])->mesh = "plane";
	system->buffer->SetEntityTransformPosition(plane_4, glm::vec3(0.f, 0.f, 200.f));
	system->buffer->SetEntityTransformScale(plane_4, glm::vec3(2.0f, 1.0f, 2.0f));

	Entity* plane_5 = system->CreateEntity();
	plane_5->AddComponent(eRender);
	((RenderComponent*)plane_5->components[eRender])->mesh = "plane";
	system->buffer->SetEntityTransformPosition(plane_5, glm::vec3(0.f, 0.f, -200.f));
	system->buffer->SetEntityTransformScale(plane_5, glm::vec3(2.0f, 1.0f, 2.0f));

	Entity* plane_6 = system->CreateEntity();
	plane_6->AddComponent(eRender);
	((RenderComponent*)plane_6->components[eRender])->mesh = "plane";
	system->buffer->SetEntityTransformPosition(plane_6, glm::vec3(200.f, 0.f, 200.f));
	system->buffer->SetEntityTransformScale(plane_6, glm::vec3(2.0f, 1.0f, 2.0f));

	Entity* plane_7 = system->CreateEntity();
	plane_7->AddComponent(eRender);
	((RenderComponent*)plane_7->components[eRender])->mesh = "plane";
	system->buffer->SetEntityTransformPosition(plane_7, glm::vec3(-200.f, 0.f, 200.f));
	system->buffer->SetEntityTransformScale(plane_7, glm::vec3(2.0f, 1.0f, 2.0f));

	Entity* plane_8 = system->CreateEntity();
	plane_8->AddComponent(eRender);
	((RenderComponent*)plane_8->components[eRender])->mesh = "plane";
	system->buffer->SetEntityTransformPosition(plane_8, glm::vec3(200.f, 0, -200.f));
	system->buffer->SetEntityTransformScale(plane_8, glm::vec3(2.0f, 1.0f, 2.0f));

	Entity* plane_9 = system->CreateEntity();
	plane_9->AddComponent(eRender);
	((RenderComponent*)plane_9->components[eRender])->mesh = "plane";
	system->buffer->SetEntityTransformPosition(plane_9, glm::vec3(-200.f, 0, -200.f));
	system->buffer->SetEntityTransformScale(plane_9, glm::vec3(2.0f, 1.0f, 2.0f));*/

	Entity* skybox = system->CreateEntity();
	skybox->AddComponent(eRender);
	((RenderComponent*)skybox->components[eRender])->mesh = "skybox";
	system->buffer->SetEntityTransformPosition(skybox, glm::vec3(0, 0, 0));
	system->buffer->SetEntityTransformScale(skybox, glm::vec3(1.0f, 1.0f, 1.0f));

	Entity* z_cube = system->CreateEntity();
	z_cube->AddComponent(eRender);
	((RenderComponent*)z_cube->components[eRender])->mesh = "cube";
	//z_cube->AddComponent(eCollider);
	//system->collider_meshes[z_cube] = new AABBCollider(z_cube, glm::vec3(0, 0, 0), glm::vec3(5, 5, 5));
	system->buffer->SetEntityTransformPosition(z_cube, glm::vec3(0, 35, 10));

	Entity* x_cube = system->CreateEntity();
	x_cube->AddComponent(eRender);
	((RenderComponent*)x_cube->components[eRender])->mesh = "cube";
	//x_cube->AddComponent(eCollider);
	//system->collider_meshes[x_cube] = new AABBCollider(x_cube, glm::vec3(0, 0, 0), glm::vec3(5, 5, 5));
	system->buffer->SetEntityTransformPosition(x_cube, glm::vec3(10, 35, 0));

	Entity* y_cube = system->CreateEntity();
	y_cube->AddComponent(eRender);
	((RenderComponent*)y_cube->components[eRender])->mesh = "cube";
	//y_cube->AddComponent(eCollider);
	//system->collider_meshes[y_cube] = new AABBCollider(y_cube, glm::vec3(0, 0, 0), glm::vec3(5, 5, 5));
	system->buffer->SetEntityTransformPosition(y_cube, glm::vec3(0, 45, 0));

	Entity* light_1 = system->CreateEntity();
	light_1->AddComponent(eLight);
	((LightComponent*)light_1->components[eLight])->Setup(eDirectionalLight, 0.0f, 0.2f, 0.6f, 0.f, glm::vec4(0.0f), glm::vec4(1.0f, 0.8f, 0.8f, 1.0f), glm::vec4(1.0f), glm::vec4(0.2f),glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	
	Entity* test_move_1 = system->CreateEntity();
	test_move_1->AddComponent(eRender);
	((RenderComponent*)test_move_1->components[eRender])->mesh = "cube";
	test_move_1->AddComponent(eMotion);
	test_move_1->AddComponent(eCollider);
	system->collider_meshes[test_move_1] = new SphereCollider(test_move_1, glm::vec3(0, 0, 0), 5);
	system->buffer->SetEntityTransformPosition(test_move_1, glm::vec3(300, 10, 300));

	Entity* test_1 = system->CreateEntity();
	test_1->AddComponent(eRender);
	((RenderComponent*)test_1->components[eRender])->mesh = "cube";
	test_1->AddComponent(eMotion);
	test_1->AddComponent(eCollider);
	system->collider_meshes[test_1] = new SphereCollider(test_1, glm::vec3(0, 0, 0), 5);
	system->buffer->SetEntityTransformPosition(test_1, glm::vec3(300, 10, 350));

	Entity* test_move_2 = system->CreateEntity();
	test_move_2->AddComponent(eRender);
	((RenderComponent*)test_move_2->components[eRender])->mesh = "cube";
	test_move_2->AddComponent(eMotion);
	test_move_2->AddComponent(eCollider);
	system->collider_meshes[test_move_2] = new SphereCollider(test_move_2, glm::vec3(0, 0, 0), 5);
	system->buffer->SetEntityTransformPosition(test_move_2, glm::vec3(320, 10, 300));

	Entity* test_2 = system->CreateEntity();
	test_2->AddComponent(eRender);
	((RenderComponent*)test_2->components[eRender])->mesh = "cube";
	test_2->AddComponent(eMotion);
	test_2->AddComponent(eCollider);
	system->collider_meshes[test_2] = new AABBCollider(test_2, glm::vec3(0, 0, 0), glm::vec3(5));
	system->buffer->SetEntityTransformPosition(test_2, glm::vec3(320, 10, 350));

	Entity* test_move_3 = system->CreateEntity();
	test_move_3->AddComponent(eRender);
	((RenderComponent*)test_move_3->components[eRender])->mesh = "cube";
	test_move_3->AddComponent(eMotion);
	test_move_3->AddComponent(eCollider);
	system->collider_meshes[test_move_3] = new SphereCollider(test_move_3, glm::vec3(0, 0, 0), 5);
	system->buffer->SetEntityTransformPosition(test_move_3, glm::vec3(340, 10, 300));

	Entity* test_3 = system->CreateEntity();
	test_3->AddComponent(eRender);
	((RenderComponent*)test_3->components[eRender])->mesh = "cube";
	test_3->AddComponent(eMotion);
	test_3->AddComponent(eCollider);
	system->collider_meshes[test_3] = new OBBCollider(test_3, glm::vec3(0), glm::vec3(0), glm::vec3(5));
	system->buffer->SetEntityTransformPosition(test_3, glm::vec3(340, 10, 350));

	Entity* test_move_4 = system->CreateEntity();
	test_move_4->AddComponent(eRender);
	((RenderComponent*)test_move_4->components[eRender])->mesh = "cube";
	test_move_4->AddComponent(eMotion);
	test_move_4->AddComponent(eCollider);
	system->collider_meshes[test_move_4] = new AABBCollider(test_move_4, glm::vec3(0, 0, 0), glm::vec3(5));
	system->buffer->SetEntityTransformPosition(test_move_4, glm::vec3(360, 10, 300));

	Entity* test_4 = system->CreateEntity();
	test_4->AddComponent(eRender);
	((RenderComponent*)test_4->components[eRender])->mesh = "cube";
	test_4->AddComponent(eMotion);
	test_4->AddComponent(eCollider);
	system->collider_meshes[test_4] = new AABBCollider(test_4, glm::vec3(0, 0, 0), glm::vec3(5));
	system->buffer->SetEntityTransformPosition(test_4, glm::vec3(360, 10, 350));

	Entity* test_move_5 = system->CreateEntity();
	test_move_5->AddComponent(eRender);
	((RenderComponent*)test_move_5->components[eRender])->mesh = "cube";
	test_move_5->AddComponent(eMotion);
	test_move_5->AddComponent(eCollider);
	system->collider_meshes[test_move_5] = new AABBCollider(test_move_5, glm::vec3(0, 0, 0), glm::vec3(5));
	system->buffer->SetEntityTransformPosition(test_move_5, glm::vec3(380, 10, 300));

	Entity* test_5 = system->CreateEntity();
	test_5->AddComponent(eRender);
	((RenderComponent*)test_5->components[eRender])->mesh = "cube";
	test_5->AddComponent(eMotion);
	test_5->AddComponent(eCollider);
	system->collider_meshes[test_5] = new OBBCollider(test_5, glm::vec3(0, 0, 0), glm::vec3(0), glm::vec3(5));
	system->buffer->SetEntityTransformPosition(test_5, glm::vec3(380, 10, 350));

	Entity* test_move_6 = system->CreateEntity();
	test_move_6->AddComponent(eRender);
	((RenderComponent*)test_move_6->components[eRender])->mesh = "cube";
	test_move_6->AddComponent(eMotion);
	test_move_6->AddComponent(eCollider);
	system->collider_meshes[test_move_6] = new OBBCollider(test_move_6, glm::vec3(0, 0, 0), glm::vec3(0), glm::vec3(5));
	system->buffer->SetEntityTransformPosition(test_move_6, glm::vec3(400, 10, 300));

	Entity* test_6 = system->CreateEntity();
	test_6->AddComponent(eRender);
	((RenderComponent*)test_6->components[eRender])->mesh = "cube";
	test_6->AddComponent(eMotion);
	test_6->AddComponent(eCollider);
	system->collider_meshes[test_6] = new OBBCollider(test_6, glm::vec3(0, 0, 0), glm::vec3(0), glm::vec3(5));
	system->buffer->SetEntityTransformPosition(test_6, glm::vec3(400, 10, 350));

	system->buffer->SetEntityTransformRotation(test_move_1, glm::vec3(0, 0, 0));
	system->buffer->SetEntityTransformRotation(test_move_2, glm::vec3(0, 0, 0));
	system->buffer->SetEntityTransformRotation(test_move_3, glm::vec3(0, 0, 0));
	system->buffer->SetEntityTransformRotation(test_move_4, glm::vec3(0, 0, 0));
	system->buffer->SetEntityTransformRotation(test_move_5, glm::vec3(0, 0, 0));
	system->buffer->SetEntityTransformRotation(test_move_6, glm::vec3(0, 0, 0));

	/*Entity* light_2 = system->CreateEntity();
	light_2->AddComponent(eLight);
	((LightComponent*)light_2->components[eLight])->Setup(ePointLight, 5, 0.2, 0.05, 0, glm::vec4(0.0f), glm::vec4(0.0f, 1.0f, 1.0f, 1.0f), glm::vec4(1.0f), glm::vec4(1.0f), glm::vec4(0.0f, 35.0f, 0.0f, 1.0f));
	
	Entity* light_3 = system->CreateEntity();
	light_3->AddComponent(eLight);
	((LightComponent*)light_3->components[eLight])->Setup(eSpotLight, 1, 0, 0.05, 65, glm::vec4(0.0f, -1.0f, 0.0f, 1.0f), glm::vec4(1.0f, 0.0f, 1.0f, 1.0f), glm::vec4(1.0f), glm::vec4(1.0f), glm::vec4(35.0f, 50.0f, 35.0f, 1.0f));
	light_3->AddComponent(eMotion);
	//system->buffer->SetEntityTransformPosition(light_3, glm::vec3(0, 0, -75));
	//((MotionComponent*)light_3->components[eMotion])->linear_forces.push_back(LinearForce(glm::vec3(0.0f, 0.0f, 5.0f), -1.0f,  true));*/

	Entity* wall_1 = system->CreateEntity();
	wall_1->AddComponent(eRender);
	((RenderComponent*)wall_1->components[eRender])->mesh = "cube";
	wall_1->AddComponent(eMotion);
	wall_1->AddComponent(eCollider);
	system->collider_meshes[wall_1] = new AABBCollider(wall_1, glm::vec3(0, 0, 0), glm::vec3(1.f, 20.f, 20.f));
	system->collider_meshes[wall_1]->response_type = eStatic;
	system->buffer->SetEntityTransformPosition(wall_1, glm::vec3(-380, 10, 400));

	Entity* wall_2 = system->CreateEntity();
	wall_2->AddComponent(eRender);
	((RenderComponent*)wall_2->components[eRender])->mesh = "cube";
	wall_2->AddComponent(eMotion);
	wall_2->AddComponent(eCollider);
	system->collider_meshes[wall_2] = new AABBCollider(wall_2, glm::vec3(0, 0, 0), glm::vec3(1.f, 20.f, 20.f));
	system->collider_meshes[wall_2]->response_type = eStatic;
	system->buffer->SetEntityTransformPosition(wall_2, glm::vec3(-420, 10, 400));

	Entity* wall_3 = system->CreateEntity();
	wall_3->AddComponent(eRender);
	((RenderComponent*)wall_3->components[eRender])->mesh = "cube";
	wall_3->AddComponent(eMotion);
	wall_3->AddComponent(eCollider);
	system->collider_meshes[wall_3] = new AABBCollider(wall_3, glm::vec3(0, 0, 0), glm::vec3(20.f, 20.f, 1.f));
	system->collider_meshes[wall_3]->response_type = eStatic;
	system->buffer->SetEntityTransformPosition(wall_3, glm::vec3(-400, 10, 420));

	Entity* wall_4 = system->CreateEntity();
	wall_4->AddComponent(eRender);
	((RenderComponent*)wall_3->components[eRender])->mesh = "cube";
	wall_4->AddComponent(eMotion);
	wall_4->AddComponent(eCollider);
	system->collider_meshes[wall_4] = new AABBCollider(wall_4, glm::vec3(0, 0, 0), glm::vec3(20.f, 20.f, 1.f));
	system->collider_meshes[wall_4]->response_type = eStatic;
	system->buffer->SetEntityTransformPosition(wall_4, glm::vec3(-400, 10, 380));

	Entity* floor_1 = system->CreateEntity();
	floor_1->AddComponent(eRender);
	((RenderComponent*)floor_1->components[eRender])->mesh = "cube";
	floor_1->AddComponent(eMotion);
	floor_1->AddComponent(eCollider);
	system->collider_meshes[floor_1] = new AABBCollider(floor_1, glm::vec3(0, 0, 0), glm::vec3(20.f, 1.f, 20.f));
	system->collider_meshes[floor_1]->response_type = eStatic;
	system->buffer->SetEntityTransformPosition(floor_1, glm::vec3(-400, 1, 400));

	Entity* ball_creator = system->CreateEntity();
	ball_creator->AddComponent(eRender);
	((RenderComponent*)ball_creator->components[eRender])->mesh = "cube";
	ball_creator->AddComponent(eScript);
	((ScriptComponent*)ball_creator->components[eScript])->script = new BallSpawnScript();
	system->buffer->SetEntityTransformPosition(ball_creator, glm::vec3(-400.f, 50.f, 400.f));

	Entity* camera_1 = system->CreateEntity();
	camera_1->AddComponent(eCamera);

	((CameraComponent*)camera_1->components[eCamera])->focus_entity = hovercraft;
	((CameraComponent*)camera_1->components[eCamera])->up_direction = glm::vec3(0.0f, 1.0f, 0.0f);
	((CameraComponent*)camera_1->components[eCamera])->offset_direction = glm::vec3(0.0f, 0.45f, -0.75f);
	((CameraComponent*)camera_1->components[eCamera])->offset_magnitude = 25*3.0f;
	((CameraComponent*)camera_1->components[eCamera])->relative = true;
	((CameraComponent*)camera_1->components[eCamera])->follow_entity = true;
	((CameraComponent*)camera_1->components[eCamera])->focus_on_entity = true;

	Entity* camera_2 = system->CreateEntity();
	camera_2->AddComponent(eCamera);

	((CameraComponent*)camera_2->components[eCamera])->focus_position = glm::vec3(0.0f, 0.0f, 0.0f);
	((CameraComponent*)camera_2->components[eCamera])->focus_entity = hovercraft;
	((CameraComponent*)camera_2->components[eCamera])->offset_direction = glm::vec3(0.0f, 1.0f, -0.00001f);
	((CameraComponent*)camera_2->components[eCamera])->offset_magnitude = 150.0f;
	((CameraComponent*)camera_2->components[eCamera])->up_direction = glm::vec3(0.0f, 1.0f, 0.0f);
	((CameraComponent*)camera_2->components[eCamera])->relative = false;
	((CameraComponent*)camera_2->components[eCamera])->follow_entity = false;
	((CameraComponent*)camera_2->components[eCamera])->focus_on_entity = true;

	Entity* camera_3 = system->CreateEntity();
	camera_3->AddComponent(eCamera);

	((CameraComponent*)camera_3->components[eCamera])->focus_entity = hovercraft;
	((CameraComponent*)camera_3->components[eCamera])->up_direction = glm::vec3(0.0f, 1.0f, 0.0f);
	((CameraComponent*)camera_3->components[eCamera])->offset_direction = glm::vec3(0.0f, 1.0f, -0.00001f);
	((CameraComponent*)camera_3->components[eCamera])->offset_magnitude = 150.0f;
	((CameraComponent*)camera_3->components[eCamera])->relative = true;
	((CameraComponent*)camera_3->components[eCamera])->follow_entity = true;
	((CameraComponent*)camera_3->components[eCamera])->focus_on_entity = true;

	Entity* camera_4 = system->CreateEntity();
	camera_4->AddComponent(eCamera);

	((CameraComponent*)camera_4->components[eCamera])->focus_entity = hovercraft;
	((CameraComponent*)camera_4->components[eCamera])->up_direction = glm::vec3(0.0f, 1.0f, 0.0f);
	((CameraComponent*)camera_4->components[eCamera])->offset_direction = glm::vec3(0.5f, 0.1f, 0.45f);
	((CameraComponent*)camera_4->components[eCamera])->offset_magnitude = 25 * 3.0f;
	((CameraComponent*)camera_4->components[eCamera])->relative = true;
	((CameraComponent*)camera_4->components[eCamera])->follow_entity = true;
	((CameraComponent*)camera_4->components[eCamera])->focus_on_entity = true;

	Entity* camera_5 = system->CreateEntity();
	camera_5->AddComponent(eCamera);
	camera_5->AddComponent(eMotion);
	((MotionComponent*)camera_5->components[eMotion])->local_dampening = 100.0f;

	((CameraComponent*)camera_5->components[eCamera])->focus_entity = camera_5;
	((CameraComponent*)camera_5->components[eCamera])->up_direction = glm::vec3(0.0f, 1.0f, 0.0f);
	((CameraComponent*)camera_5->components[eCamera])->offset_direction = glm::vec3(0.0f, 0.0f, -0.1f);
	((CameraComponent*)camera_5->components[eCamera])->offset_magnitude = 1;
	((CameraComponent*)camera_5->components[eCamera])->relative = true;
	((CameraComponent*)camera_5->components[eCamera])->follow_entity = true;
	((CameraComponent*)camera_5->components[eCamera])->focus_on_entity = true;

	std::vector<Command*> space_command =
	{
		new SetVelocityCommand(0, glm::vec3(1.0, 1.0, 1.0)),
		new SetVelocityCommand(0, glm::vec3(1.0, 1.0, 1.0), false, false)
	};

	std::vector<Command*> camera_1_command =
	{
		new SetMainCameraCommand(camera_1),
		new SetInputEntityCommand(hovercraft)
	};

	std::vector<Command*> camera_2_command =
	{
		new SetMainCameraCommand(camera_2),
		new SetInputEntityCommand(hovercraft)
	};

	std::vector<Command*> camera_3_command =
	{
		new SetMainCameraCommand(camera_3),
		new SetInputEntityCommand(hovercraft)
	};

	std::vector<Command*> camera_4_command =
	{
		new SetMainCameraCommand(camera_4),
		new SetInputEntityCommand(hovercraft)
	};

	std::vector<Command*> camera_5_command =
	{
		new SetMainCameraCommand(camera_5),
		new SetInputEntityCommand(camera_5)
	};

	std::vector<Command*> w_command =
	{
		new CheckInputEntityCommand(hovercraft, new ForceCommand(glm::vec3(0.0f, 0.0f, 300.0f), -1.0f, false, true)),
		new CheckInputEntityCommand(camera_5, new SetVelocityCommand(50.0f, glm::vec3(0.0f, 0.0f, 1.0f)))
	};

	std::vector<Command*> s_command =
	{
		new CheckInputEntityCommand(hovercraft, new ForceCommand(glm::vec3(0.0f, 0.0f,-300.0f), -1.0f, false, true)),
		new CheckInputEntityCommand(camera_5, new SetVelocityCommand(50.0f, glm::vec3(0.0f, 0.0f, -1.0f)))
	};

	std::vector<Command*> a_command =
	{
		new CheckInputEntityCommand(hovercraft, new ForceCommand(glm::vec3(100.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 2.0f), false, false)),
		new CheckInputEntityCommand(camera_5, new SetVelocityCommand(50.0f, glm::vec3(0.0f, 1.0f, 0.0f), false, false))
	};

	std::vector<Command*> d_command =
	{
		new CheckInputEntityCommand(hovercraft, new ForceCommand(glm::vec3(-100.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 2.0f), false, false)),
		new CheckInputEntityCommand(camera_5, new SetVelocityCommand(50.0f, glm::vec3(0.0f, -1.0f, 0.0f), false, false))
	};

	std::vector<Command*> col_test_command =
	{
		new SetEntityVelocityCommand(test_move_1, 50.0f, glm::vec3(0.0f, 0.0f, 1.0f), true, true),
		new SetEntityVelocityCommand(test_move_2, 50.0f, glm::vec3(0.0f, 0.0f, 1.0f), true, true),
		new SetEntityVelocityCommand(test_move_3, 50.0f, glm::vec3(0.0f, 0.0f, 1.0f), true, true),
		new SetEntityVelocityCommand(test_move_4, 50.0f, glm::vec3(0.0f, 0.0f, 1.0f), true, true),
		new SetEntityVelocityCommand(test_move_5, 50.0f, glm::vec3(0.0f, 0.0f, 1.0f), true, true),
		new SetEntityVelocityCommand(test_move_6, 50.0f, glm::vec3(0.0f, 0.0f, 1.0f), true, true),
	};

	system->input_focus_entity = hovercraft;
	system->AddKeyInput('W', new MultiCommand(w_command));
	system->AddKeyInput('S', new MultiCommand(s_command));
	system->AddKeyInput('A', new MultiCommand(a_command));
	system->AddKeyInput('D', new MultiCommand(d_command));
	system->AddKeyInput('Q', new CheckInputEntityCommand(camera_5, new SetVelocityCommand(50.0f, glm::vec3(0.0f, 1.0f, 0.0f))));
	system->AddKeyInput('E', new CheckInputEntityCommand(camera_5, new SetVelocityCommand(50.0f, glm::vec3(0.0f, -1.0f, 0.0f))));
	system->AddKeyInput(' ', new MultiCommand(space_command));
	system->AddToggleKeyInput('F', new ToggleFloatCommand(&((LightComponent*)hovercraft->components[eLight])->intensity, 0.0f, 1.f));
	system->AddToggleKeyInput('Z', new ToggleFloatCommand(&((LightComponent*)light_1->components[eLight])->intensity, 0.0f, 0.5f));
	system->AddToggleKeyInput('X', new ToggleBoolCommand(system->physics->verlet_integration));
	system->AddToggleKeyInput('C', new ToggleBoolCommand(system->render_meshes["skybox"]->material.lighting));
	system->AddToggleKeyInput('V', new ToggleBoolCommand(system->renderer->draw_debug));
	system->AddToggleKeyInput('B', new ToggleCullOrientationCommand(false));
	system->AddToggleKeyInput('N', new ToggleMSAACommand(true));
	system->AddToggleKeyInput('M', new ToggleFloatCommand(&system->colliders->main_oct_tree->subdivide_threshold, 4.f, 8.f));
	system->AddKeyInput('H', new MultiCommand(col_test_command));
	system->AddToggleKeyInput('J', new ToggleBoolCommand(((BallSpawnScript*)((ScriptComponent*)ball_creator->components[eScript])->script)->set));

	system->AddKeyInput('1', new MultiCommand(camera_1_command));
	system->AddKeyInput('2', new MultiCommand(camera_2_command));
	system->AddKeyInput('3', new MultiCommand(camera_3_command));
	system->AddKeyInput('4', new MultiCommand(camera_4_command));
	system->AddKeyInput('5', new MultiCommand(camera_5_command));

	system->main_camera = camera_1;
	system->renderer->skybox = skybox;
	
	
	system->Start();

	return 0;
}  