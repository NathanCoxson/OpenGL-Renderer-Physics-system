#version 430 core

layout(location = 0) in vec4 vertex_coords;
layout(location = 1) in vec3 vertex_normals;
layout(location = 2) in vec2 texture_coords;
uniform mat4 projection_matrix;
uniform mat4 model_matrix;
uniform mat4 view_matrix;

out vec2 fragment_texture_coords;
out vec3 fragment_normal;
out vec4 fragment_vertex;

void main(void)
{
	fragment_vertex = vertex_coords;
	fragment_normal = vertex_normals;
	fragment_texture_coords = texture_coords;
	gl_Position = projection_matrix * view_matrix * model_matrix * vertex_coords;
}

/*
   //colours_export = vec4(vec3(min(vertex_material.emission + global_ambient + vertex_ambient + vertex_diffuse + vertex_specular, vec4(1.0))), 1.0);

   //normal = normalize(normal_matrix * vertex_normals);
   //light_direction = normalize(vec3(light.coords));
   //eye_direction = -1.0f * normalize(vec3(model_view_matrix * vertex_coords)); 
   //halfway = (length(light_direction + eye_direction) == 0.0f) ? vec3(0.0f) : (light_direction + eye_direction)/length(light_direction + eye_direction);

   normal = normalize(normal_matrix * vertex_normals);
   vec3 fragment_position = vec3(model_matrix * vertex_coords);
   light_direction = light.coords - fragment_position;

   //light_direction = normalize(vec3(light.coords - vertex_coords));

   float brightness = dot(normal, light_direction) / (length(light_direction) * length(normal));
   brightness = clamp(brightness, 0, 1);

   

   reflect_direction = reflect(-light_direction, normal);
   eye_direction = normalize(vec3(model_matrix * vertex_coords)); 
   vertex_ambient = vertex_material.ambient_reflection * light.ambient_colours;
   vertex_diffuse = vec4(brightness * light.diffuse_colours * vertex_ambient);
   //vertex_diffuse = max(dot(light_direction, normal), 0.0) * light.diffuse_colours;
   vertex_specular = pow(max(dot(eye_direction, reflect_direction), 0.0f), vertex_material.shine) * light.specular_colours;

   phongTest = (vertex_ambient + vertex_diffuse);*/


