#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "track.h"

vector3_t change_coordinates(vector3_t a)
{
vector3_t result={a.z,a.y,a.x};
return result;
}

void track_transform_primitives(track_point_t (*track_curve)(float distance),primitive_t* primitives,uint32_t num_primitives,int diagonal)
{
	for(int i=0;i<num_primitives;i++)
	{
		for(int j=0;j<3;j++)
		{
		track_point_t track_point=track_curve(primitives[i].vertices[j].z);
			if(diagonal)track_point.position.x+=0.75*sqrt(6);
		track_point.position.y-=0.75;
		track_point.position.z-=0.75*sqrt(6);
		primitives[i].vertices[j]=change_coordinates(vector3_add(track_point.position,vector3_add(vector3_mult(track_point.normal,primitives[i].vertices[j].y),vector3_mult(track_point.binormal,primitives[i].vertices[j].x))));
		primitives[i].normals[j]=change_coordinates(vector3_add(vector3_mult(track_point.tangent,primitives[i].normals[j].z),vector3_add(vector3_mult(track_point.normal,primitives[i].normals[j].y),vector3_mult(track_point.binormal,primitives[i].normals[j].x))));

		}
	}
}



void framebuffer_render_track_section(framebuffer_t* framebuffer,context_t* context,track_section_t* track_section,track_type_t* track_type,int extrude_behind)
{
float num_meshes=(int)floor(0.5+track_section->length/track_type->length);
float scale=track_section->length/(num_meshes*track_type->length);
float length=scale*track_type->length;

	if(extrude_behind)num_meshes++;//TODO make extruded section straight

uint32_t primitives_per_mesh=mesh_count_primitives(&(track_type->mesh));
primitive_t* primitives=malloc(((size_t)num_meshes)*primitives_per_mesh*sizeof(primitive_t));
	for(int i=0;i<num_meshes;i++)
	{
	transform_t transform={{1.0,0.0,0.0,0.0,1.0,0.0,0.0,0.0,scale},{0.0,0.0,(i-(extrude_behind?1:0))*length}};
	mesh_get_primitives(&(track_type->mesh),primitives+i*primitives_per_mesh);
	transform_primitives(transform,primitives+i*primitives_per_mesh,primitives_per_mesh);
	}
track_transform_primitives(track_section->curve,primitives,((uint32_t)num_meshes)*primitives_per_mesh,track_section->flags&TRACK_DIAGONAL);
transform_primitives(context->projection,primitives,((uint32_t)num_meshes)*primitives_per_mesh);
framebuffer_from_primitives(framebuffer,context,primitives,((uint32_t)num_meshes)*primitives_per_mesh);
free(primitives);
}
