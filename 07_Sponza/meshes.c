#if 0
rm -f meshes.o
cc -std=c99 -c meshes.c
exit
#endif

const int mesh_texture_indices[] = {
#include "sponza_mesh_headers/mesh_texture_indices.h"
};

const float mesh_vertices[] = {
#include "sponza_mesh_headers/mesh_vertices.h"
};

const float mesh_uvs[] = {
#include "sponza_mesh_headers/mesh_uvs.h"
};

const int submesh_vertex_begin[] = {
#include "sponza_mesh_headers/submesh_vertex_begin.h"
};

const int submesh_vertex_end[] = {
#include "sponza_mesh_headers/submesh_vertex_end.h"
};
