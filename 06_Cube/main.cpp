#if 0
rm -f a.out
cc -std=c99 main.c ../rei/rei.c ../glfw/lib/libglfw.so.3 -lGL -lm
LD_LIBRARY_PATH=../glfw/lib/ ./a.out
exit
#endif

#include "../rei/rei.h"
#define GLFW_INCLUDE_NONE
#include "../glfw/include/glfw3.h"

#include <stdio.h>
#include <math.h>
#ifdef __linux__
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#endif

#define STB_IMAGE_IMPLEMENTATION
#define STBI_NO_LINEAR
#define STBI_NO_HDR
#define STBI_NO_SIMD
#include "stb_image.h"

const float mesh_vertices[] = {
#include "mesh_vertices.h"
};

const float mesh_normals[] = {
#include "mesh_normals.h"
};

#define countof(x) (sizeof(x) / sizeof((x)[0]))

int main() {
  glfwInit();
  glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 1);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
  glfwWindowHint(GLFW_RESIZABLE, 0);
  glfwWindowHint(GLFW_SAMPLES, 4);
  GLFWwindow * window = glfwCreateWindow(700, 700, "", NULL, NULL);
  glfwMakeContextCurrent(window);

  ReiContext   context = {};
  ReiContext * ctx     = &context;
  reiCreateContext((ReiTypeProcedureGetProcAddress)glfwGetProcAddress, &context);

  char vp_skybox_string[] =
    "!!ARBvp1.0"
    "TEMP pos;"
    "TEMP R0;"
    "MOV pos, vertex.position;"
    "MUL R0.xyzw, pos.xzzx, program.local[0].xyxy;"
    "ADD pos.x, R0.x,-R0.y;"
    "ADD pos.z, R0.z, R0.w;"
    "MUL R0.xyzw, pos.yzzy, program.local[0].zwzw;"
    "ADD pos.y, R0.x,-R0.y;"
    "ADD pos.z, R0.z, R0.w;"
    "MOV result.texcoord[0], vertex.position;"
    "MOV result.position.xyw, pos.xyzz;"
    "MOV result.position.z, 0.1;"
    "END";

  char fp_skybox_string[] =
    "!!ARBfp1.0"
    "TEX result.color, fragment.texcoord[0], texture[0], CUBE;"
    "END";

  char vp_string[] =
    "!!ARBvp1.0"
    "TEMP pos;"
    "TEMP R0;"
    "ADD pos, vertex.position, -program.local[0];"
    "MUL R0.xyzw, pos.xzzx, program.local[1].xyxy;"
    "ADD pos.x, R0.x,-R0.y;"
    "ADD pos.z, R0.z, R0.w;"
    "MUL R0.xyzw, pos.yzzy, program.local[1].zwzw;"
    "ADD pos.y, R0.x,-R0.y;"
    "ADD pos.z, R0.z, R0.w;"
    "MOV result.texcoord[0], vertex.position;"
    "MOV result.texcoord[1], vertex.normal;"
    "MOV result.position.xyw, pos.xyzz;"
    "MOV result.position.z, 0.1;"
    "END";

  char fp_string[] =
    "!!ARBfp1.0"
    "TEMP R0;"
    "TEMP R1;"
    "ADD R0.xyz, fragment.texcoord[0], -program.local[0];"
    "DP3 R0.w, fragment.texcoord[1], R0;"
    "MUL R1.xyz, fragment.texcoord[1], R0.w;"
    "MAD R0.xyz, -R1, 2, R0;"
    "TEX result.color, R0, texture[0], CUBE;"
    "END";

  ReiHandleProgram vp_skybox_program = reiCreateProgram(ctx);
  ReiHandleProgram fp_skybox_program = reiCreateProgram(ctx);
  ReiHandleProgram vp_program        = reiCreateProgram(ctx);
  ReiHandleProgram fp_program        = reiCreateProgram(ctx);

  reiBindProgram(ctx, REI_PROGRAM_BINDING_VERTEX, vp_skybox_program);
  reiProgramInitialize(ctx, REI_PROGRAM_BINDING_VERTEX, sizeof(vp_skybox_string)-1, vp_skybox_string);
  reiBindProgram(ctx, REI_PROGRAM_BINDING_VERTEX, 0);

  reiBindProgram(ctx, REI_PROGRAM_BINDING_FRAGMENT, fp_skybox_program);
  reiProgramInitialize(ctx, REI_PROGRAM_BINDING_FRAGMENT, sizeof(fp_skybox_string)-1, fp_skybox_string);
  reiBindProgram(ctx, REI_PROGRAM_BINDING_FRAGMENT, 0);

  reiBindProgram(ctx, REI_PROGRAM_BINDING_VERTEX, vp_program);
  reiProgramInitialize(ctx, REI_PROGRAM_BINDING_VERTEX, sizeof(vp_string)-1, vp_string);
  reiBindProgram(ctx, REI_PROGRAM_BINDING_VERTEX, 0);

  reiBindProgram(ctx, REI_PROGRAM_BINDING_FRAGMENT, fp_program);
  reiProgramInitialize(ctx, REI_PROGRAM_BINDING_FRAGMENT, sizeof(fp_string)-1, fp_string);
  reiBindProgram(ctx, REI_PROGRAM_BINDING_FRAGMENT, 0);

#ifdef _WIN32
  FILE * cube_px_file_fd = NULL;
  FILE * cube_nx_file_fd = NULL;
  FILE * cube_py_file_fd = NULL;
  FILE * cube_ny_file_fd = NULL;
  FILE * cube_pz_file_fd = NULL;
  FILE * cube_nz_file_fd = NULL;

  fopen_s(&cube_px_file_fd, "cube/cube_px_right.png",  "rb");
  fopen_s(&cube_nx_file_fd, "cube/cube_nx_left.png",   "rb");
  fopen_s(&cube_py_file_fd, "cube/cube_py_top.png",    "rb");
  fopen_s(&cube_ny_file_fd, "cube/cube_ny_bottom.png", "rb");
  fopen_s(&cube_pz_file_fd, "cube/cube_pz_front.png",  "rb");
  fopen_s(&cube_nz_file_fd, "cube/cube_nz_back.png",   "rb");

  int cube_w = 0, cube_h = 0, cube_bpp = 0;
  void * cube_px = stbi_load_from_file(cube_px_file_fd, &cube_w, &cube_h, &cube_bpp, STBI_rgb_alpha);
  void * cube_nx = stbi_load_from_file(cube_nx_file_fd, &cube_w, &cube_h, &cube_bpp, STBI_rgb_alpha);
  void * cube_py = stbi_load_from_file(cube_py_file_fd, &cube_w, &cube_h, &cube_bpp, STBI_rgb_alpha);
  void * cube_ny = stbi_load_from_file(cube_ny_file_fd, &cube_w, &cube_h, &cube_bpp, STBI_rgb_alpha);
  void * cube_pz = stbi_load_from_file(cube_pz_file_fd, &cube_w, &cube_h, &cube_bpp, STBI_rgb_alpha);
  void * cube_nz = stbi_load_from_file(cube_nz_file_fd, &cube_w, &cube_h, &cube_bpp, STBI_rgb_alpha);
#else
  int cube_px_file_fd = open("cube/cube_px_right.png",  O_RDONLY);
  int cube_nx_file_fd = open("cube/cube_nx_left.png",   O_RDONLY);
  int cube_py_file_fd = open("cube/cube_py_top.png",    O_RDONLY);
  int cube_ny_file_fd = open("cube/cube_ny_bottom.png", O_RDONLY);
  int cube_pz_file_fd = open("cube/cube_pz_front.png",  O_RDONLY);
  int cube_nz_file_fd = open("cube/cube_nz_back.png",   O_RDONLY);

  struct stat cube_px_file_st = {};
  struct stat cube_nx_file_st = {};
  struct stat cube_py_file_st = {};
  struct stat cube_ny_file_st = {};
  struct stat cube_pz_file_st = {};
  struct stat cube_nz_file_st = {};

  fstat(cube_px_file_fd, &cube_px_file_st);
  fstat(cube_nx_file_fd, &cube_nx_file_st);
  fstat(cube_py_file_fd, &cube_py_file_st);
  fstat(cube_ny_file_fd, &cube_ny_file_st);
  fstat(cube_pz_file_fd, &cube_pz_file_st);
  fstat(cube_nz_file_fd, &cube_nz_file_st);

  unsigned char * cube_px_file_mem = mmap(0, cube_px_file_st.st_size, PROT_READ, MAP_PRIVATE, cube_px_file_fd, 0);
  unsigned char * cube_nx_file_mem = mmap(0, cube_nx_file_st.st_size, PROT_READ, MAP_PRIVATE, cube_nx_file_fd, 0);
  unsigned char * cube_py_file_mem = mmap(0, cube_py_file_st.st_size, PROT_READ, MAP_PRIVATE, cube_py_file_fd, 0);
  unsigned char * cube_ny_file_mem = mmap(0, cube_ny_file_st.st_size, PROT_READ, MAP_PRIVATE, cube_ny_file_fd, 0);
  unsigned char * cube_pz_file_mem = mmap(0, cube_pz_file_st.st_size, PROT_READ, MAP_PRIVATE, cube_pz_file_fd, 0);
  unsigned char * cube_nz_file_mem = mmap(0, cube_nz_file_st.st_size, PROT_READ, MAP_PRIVATE, cube_nz_file_fd, 0);

  int cube_w = 0, cube_h = 0, cube_bpp = 0;
  void * cube_px = stbi_load_from_memory(cube_px_file_mem, cube_px_file_st.st_size, &cube_w, &cube_h, &cube_bpp, STBI_rgb_alpha);
  void * cube_nx = stbi_load_from_memory(cube_nx_file_mem, cube_nx_file_st.st_size, &cube_w, &cube_h, &cube_bpp, STBI_rgb_alpha);
  void * cube_py = stbi_load_from_memory(cube_py_file_mem, cube_py_file_st.st_size, &cube_w, &cube_h, &cube_bpp, STBI_rgb_alpha);
  void * cube_ny = stbi_load_from_memory(cube_ny_file_mem, cube_ny_file_st.st_size, &cube_w, &cube_h, &cube_bpp, STBI_rgb_alpha);
  void * cube_pz = stbi_load_from_memory(cube_pz_file_mem, cube_pz_file_st.st_size, &cube_w, &cube_h, &cube_bpp, STBI_rgb_alpha);
  void * cube_nz = stbi_load_from_memory(cube_nz_file_mem, cube_nz_file_st.st_size, &cube_w, &cube_h, &cube_bpp, STBI_rgb_alpha);
#endif

  ReiHandleTexture cube = reiCreateTexture(ctx);
  reiSetActiveTextureSlot(ctx, 0);
  reiBindTextureToActiveTextureSlot(ctx, REI_TEXTURE_BINDING_CUBE, cube);
  reiTextureSetStateMipmap(ctx, REI_TEXTURE_BINDING_CUBE, 1);
  reiTextureSetStateSampler(ctx, REI_TEXTURE_BINDING_CUBE, REI_SAMPLER_FILTERING_LINEAR, REI_SAMPLER_FILTERING_LINEAR_MIP_LINEAR, REI_SAMPLER_BEHAVIOR_OUTSIDE_TEXTURE_COORDINATE_CLAMP_TO_EDGE_VALUE, REI_SAMPLER_BEHAVIOR_OUTSIDE_TEXTURE_COORDINATE_CLAMP_TO_EDGE_VALUE, 1);
  reiTextureDefineAndCopyFromCpu(ctx, REI_TEXTURE_BINDING_CUBE_POSITIVE_X, 0, REI_TEXTURE_TEXEL_FORMAT_RGBA, cube_w, cube_h, REI_TEXTURE_TEXEL_FORMAT_RGBA, REI_TEXTURE_TEXEL_TYPE_UBYTE, 4, cube_px);
  reiTextureDefineAndCopyFromCpu(ctx, REI_TEXTURE_BINDING_CUBE_NEGATIVE_X, 0, REI_TEXTURE_TEXEL_FORMAT_RGBA, cube_w, cube_h, REI_TEXTURE_TEXEL_FORMAT_RGBA, REI_TEXTURE_TEXEL_TYPE_UBYTE, 4, cube_nx);
  reiTextureDefineAndCopyFromCpu(ctx, REI_TEXTURE_BINDING_CUBE_POSITIVE_Y, 0, REI_TEXTURE_TEXEL_FORMAT_RGBA, cube_w, cube_h, REI_TEXTURE_TEXEL_FORMAT_RGBA, REI_TEXTURE_TEXEL_TYPE_UBYTE, 4, cube_py);
  reiTextureDefineAndCopyFromCpu(ctx, REI_TEXTURE_BINDING_CUBE_NEGATIVE_Y, 0, REI_TEXTURE_TEXEL_FORMAT_RGBA, cube_w, cube_h, REI_TEXTURE_TEXEL_FORMAT_RGBA, REI_TEXTURE_TEXEL_TYPE_UBYTE, 4, cube_ny);
  reiTextureDefineAndCopyFromCpu(ctx, REI_TEXTURE_BINDING_CUBE_POSITIVE_Z, 0, REI_TEXTURE_TEXEL_FORMAT_RGBA, cube_w, cube_h, REI_TEXTURE_TEXEL_FORMAT_RGBA, REI_TEXTURE_TEXEL_TYPE_UBYTE, 4, cube_pz);
  reiTextureDefineAndCopyFromCpu(ctx, REI_TEXTURE_BINDING_CUBE_NEGATIVE_Z, 0, REI_TEXTURE_TEXEL_FORMAT_RGBA, cube_w, cube_h, REI_TEXTURE_TEXEL_FORMAT_RGBA, REI_TEXTURE_TEXEL_TYPE_UBYTE, 4, cube_nz);
  reiBindTextureToActiveTextureSlot(ctx, REI_TEXTURE_BINDING_CUBE, 0);

  if (cube_px != NULL) { stbi_image_free(cube_px); cube_px = NULL; }
  if (cube_nx != NULL) { stbi_image_free(cube_nx); cube_nx = NULL; }
  if (cube_py != NULL) { stbi_image_free(cube_py); cube_py = NULL; }
  if (cube_ny != NULL) { stbi_image_free(cube_ny); cube_ny = NULL; }
  if (cube_pz != NULL) { stbi_image_free(cube_pz); cube_pz = NULL; }
  if (cube_nz != NULL) { stbi_image_free(cube_nz); cube_nz = NULL; }

#ifdef _WIN32
  if (cube_px_file_fd != NULL) { fclose(cube_px_file_fd); cube_px_file_fd = 0; }
  if (cube_nx_file_fd != NULL) { fclose(cube_nx_file_fd); cube_nx_file_fd = 0; }
  if (cube_py_file_fd != NULL) { fclose(cube_py_file_fd); cube_py_file_fd = 0; }
  if (cube_ny_file_fd != NULL) { fclose(cube_ny_file_fd); cube_ny_file_fd = 0; }
  if (cube_pz_file_fd != NULL) { fclose(cube_pz_file_fd); cube_pz_file_fd = 0; }
  if (cube_nz_file_fd != NULL) { fclose(cube_nz_file_fd); cube_nz_file_fd = 0; }
#else
  if (cube_px_file_mem != NULL) { munmap(cube_px_file_mem, cube_px_file_st.st_size); cube_px_file_mem = NULL; }
  if (cube_nx_file_mem != NULL) { munmap(cube_nx_file_mem, cube_nx_file_st.st_size); cube_nx_file_mem = NULL; }
  if (cube_py_file_mem != NULL) { munmap(cube_py_file_mem, cube_py_file_st.st_size); cube_py_file_mem = NULL; }
  if (cube_ny_file_mem != NULL) { munmap(cube_ny_file_mem, cube_ny_file_st.st_size); cube_ny_file_mem = NULL; }
  if (cube_pz_file_mem != NULL) { munmap(cube_pz_file_mem, cube_pz_file_st.st_size); cube_pz_file_mem = NULL; }
  if (cube_nz_file_mem != NULL) { munmap(cube_nz_file_mem, cube_nz_file_st.st_size); cube_nz_file_mem = NULL; }

  if (cube_px_file_fd != 0) { close(cube_px_file_fd); cube_px_file_fd = 0; }
  if (cube_nx_file_fd != 0) { close(cube_nx_file_fd); cube_nx_file_fd = 0; }
  if (cube_py_file_fd != 0) { close(cube_py_file_fd); cube_py_file_fd = 0; }
  if (cube_ny_file_fd != 0) { close(cube_ny_file_fd); cube_ny_file_fd = 0; }
  if (cube_pz_file_fd != 0) { close(cube_pz_file_fd); cube_pz_file_fd = 0; }
  if (cube_nz_file_fd != 0) { close(cube_nz_file_fd); cube_nz_file_fd = 0; }
#endif

  ReiHandleCommandList list = reiCreateCommandList(ctx);
  reiCommandListSet(ctx, list);
    reiCommandSetViewport(ctx, 0, 0, 700, 700);
    reiCommandSetScissor(ctx, 0, 0, 700, 700);
    reiCommandClear(ctx, REI_CLEAR_DEPTH_BIT | REI_CLEAR_COLOR_BIT, 0.f, 0, 0.15f, 0.15f, 0.35f, 1.f);
    reiSetActiveTextureSlot(ctx, 0);
    reiBindTextureToActiveTextureSlot(ctx, REI_TEXTURE_BINDING_CUBE, cube);
    ReiMeshState mesh_skybox_state = {};
    mesh_skybox_state.programVertex                                  = vp_skybox_program;
    mesh_skybox_state.programFragment                                = fp_skybox_program;
    mesh_skybox_state.rasterizationDepthClampEnable                  = 0;
    mesh_skybox_state.rasterizationCullMode                          = REI_CULL_MODE_NONE;
    mesh_skybox_state.rasterizationFrontFace                         = REI_FRONT_FACE_COUNTER_CLOCKWISE;
    mesh_skybox_state.rasterizationDepthBiasEnable                   = 0;
    mesh_skybox_state.rasterizationDepthBiasConstantFactor           = 0;
    mesh_skybox_state.rasterizationDepthBiasSlopeFactor              = 0;
    mesh_skybox_state.multisampleEnable                              = 0;
    mesh_skybox_state.multisampleAlphaToCoverageEnable               = 0;
    mesh_skybox_state.multisampleAlphaToOneEnable                    = 0;
    mesh_skybox_state.depthTestEnable                                = 0;
    mesh_skybox_state.depthTestDepthWriteEnable                      = 0;
    mesh_skybox_state.depthTestDepthCompareOp                        = REI_COMPARE_OP_GREATER_OR_EQUAL;
    mesh_skybox_state.stencilTestEnable                              = 0;
    mesh_skybox_state.stencilTestFrontStencilTestFailOp              = REI_STENCIL_OP_KEEP;
    mesh_skybox_state.stencilTestFrontStencilTestPassDepthTestPassOp = REI_STENCIL_OP_KEEP;
    mesh_skybox_state.stencilTestFrontStencilTestPassDepthTestFailOp = REI_STENCIL_OP_KEEP;
    mesh_skybox_state.stencilTestFrontCompareOp                      = REI_COMPARE_OP_NEVER;
    mesh_skybox_state.stencilTestFrontCompareMask                    = 0;
    mesh_skybox_state.stencilTestFrontWriteMask                      = 0;
    mesh_skybox_state.stencilTestFrontReference                      = 0;
    mesh_skybox_state.stencilTestBackStencilTestFailOp               = REI_STENCIL_OP_KEEP;
    mesh_skybox_state.stencilTestBackStencilTestPassDepthTestPassOp  = REI_STENCIL_OP_KEEP;
    mesh_skybox_state.stencilTestBackStencilTestPassDepthTestFailOp  = REI_STENCIL_OP_KEEP;
    mesh_skybox_state.stencilTestBackCompareOp                       = REI_COMPARE_OP_NEVER;
    mesh_skybox_state.stencilTestBackCompareMask                     = 0;
    mesh_skybox_state.stencilTestBackWriteMask                       = 0;
    mesh_skybox_state.stencilTestBackReference                       = 0;
    mesh_skybox_state.blendLogicOpEnable                             = 0;
    mesh_skybox_state.blendLogicOp                                   = REI_LOGIC_OP_CLEAR;
    mesh_skybox_state.blendConstants[0]                              = 0;
    mesh_skybox_state.blendConstants[1]                              = 0;
    mesh_skybox_state.blendConstants[2]                              = 0;
    mesh_skybox_state.blendConstants[3]                              = 0;
    mesh_skybox_state.outputColorWriteEnableR                        = 1;
    mesh_skybox_state.outputColorWriteEnableG                        = 1;
    mesh_skybox_state.outputColorWriteEnableB                        = 1;
    mesh_skybox_state.outputColorWriteEnableA                        = 1;
    mesh_skybox_state.outputColorBlendEnable                         = 0;
    mesh_skybox_state.outputColorBlendColorFactorSource              = REI_BLEND_FACTOR_ZERO;
    mesh_skybox_state.outputColorBlendColorFactorTarget              = REI_BLEND_FACTOR_ZERO;
    mesh_skybox_state.outputColorBlendColorOp                        = REI_BLEND_OP_ADD;
    mesh_skybox_state.outputColorBlendAlphaFactorSource              = REI_BLEND_FACTOR_ZERO;
    mesh_skybox_state.outputColorBlendAlphaFactorTarget              = REI_BLEND_FACTOR_ZERO;
    mesh_skybox_state.outputColorBlendAlphaOp                        = REI_BLEND_OP_ADD;
    reiCommandMeshSetState(ctx, &mesh_skybox_state);
    reiCommandMeshSet(ctx);
    reiCommandMeshPosition(ctx, -1,-1, 1, 1); reiCommandMeshPosition(ctx, -1,-1,-1, 1); reiCommandMeshPosition(ctx,  1,-1,-1, 1);
    reiCommandMeshPosition(ctx,  1,-1,-1, 1); reiCommandMeshPosition(ctx,  1,-1, 1, 1); reiCommandMeshPosition(ctx, -1,-1, 1, 1);
    reiCommandMeshPosition(ctx, -1, 1,-1, 1); reiCommandMeshPosition(ctx, -1,-1,-1, 1); reiCommandMeshPosition(ctx, -1,-1, 1, 1);
    reiCommandMeshPosition(ctx, -1,-1, 1, 1); reiCommandMeshPosition(ctx, -1, 1, 1, 1); reiCommandMeshPosition(ctx, -1, 1,-1, 1);
    reiCommandMeshPosition(ctx,  1,-1,-1, 1); reiCommandMeshPosition(ctx,  1, 1,-1, 1); reiCommandMeshPosition(ctx,  1, 1, 1, 1);
    reiCommandMeshPosition(ctx,  1, 1, 1, 1); reiCommandMeshPosition(ctx,  1,-1, 1, 1); reiCommandMeshPosition(ctx,  1,-1,-1, 1);
    reiCommandMeshPosition(ctx, -1, 1,-1, 1); reiCommandMeshPosition(ctx, -1, 1, 1, 1); reiCommandMeshPosition(ctx,  1, 1, 1, 1);
    reiCommandMeshPosition(ctx,  1, 1, 1, 1); reiCommandMeshPosition(ctx,  1, 1,-1, 1); reiCommandMeshPosition(ctx, -1, 1,-1, 1);
    reiCommandMeshPosition(ctx, -1,-1, 1, 1); reiCommandMeshPosition(ctx,  1,-1, 1, 1); reiCommandMeshPosition(ctx,  1, 1, 1, 1);
    reiCommandMeshPosition(ctx,  1, 1, 1, 1); reiCommandMeshPosition(ctx, -1, 1, 1, 1); reiCommandMeshPosition(ctx, -1,-1, 1, 1);
    reiCommandMeshPosition(ctx, -1,-1,-1, 1); reiCommandMeshPosition(ctx, -1, 1,-1, 1); reiCommandMeshPosition(ctx,  1,-1,-1, 1);
    reiCommandMeshPosition(ctx,  1,-1,-1, 1); reiCommandMeshPosition(ctx, -1, 1,-1, 1); reiCommandMeshPosition(ctx,  1, 1,-1, 1);
    reiCommandMeshEnd(ctx);
    ReiMeshState mesh_state = {};
    mesh_state.programVertex                                  = vp_program;
    mesh_state.programFragment                                = fp_program;
    mesh_state.rasterizationDepthClampEnable                  = 0;
    mesh_state.rasterizationCullMode                          = REI_CULL_MODE_BACK;
    mesh_state.rasterizationFrontFace                         = REI_FRONT_FACE_CLOCKWISE;
    mesh_state.rasterizationDepthBiasEnable                   = 0;
    mesh_state.rasterizationDepthBiasConstantFactor           = 0;
    mesh_state.rasterizationDepthBiasSlopeFactor              = 0;
    mesh_state.multisampleEnable                              = 1;
    mesh_state.multisampleAlphaToCoverageEnable               = 0;
    mesh_state.multisampleAlphaToOneEnable                    = 0;
    mesh_state.depthTestEnable                                = 1;
    mesh_state.depthTestDepthWriteEnable                      = 1;
    mesh_state.depthTestDepthCompareOp                        = REI_COMPARE_OP_GREATER_OR_EQUAL;
    mesh_state.stencilTestEnable                              = 0;
    mesh_state.stencilTestFrontStencilTestFailOp              = REI_STENCIL_OP_KEEP;
    mesh_state.stencilTestFrontStencilTestPassDepthTestPassOp = REI_STENCIL_OP_KEEP;
    mesh_state.stencilTestFrontStencilTestPassDepthTestFailOp = REI_STENCIL_OP_KEEP;
    mesh_state.stencilTestFrontCompareOp                      = REI_COMPARE_OP_NEVER;
    mesh_state.stencilTestFrontCompareMask                    = 0;
    mesh_state.stencilTestFrontWriteMask                      = 0;
    mesh_state.stencilTestFrontReference                      = 0;
    mesh_state.stencilTestBackStencilTestFailOp               = REI_STENCIL_OP_KEEP;
    mesh_state.stencilTestBackStencilTestPassDepthTestPassOp  = REI_STENCIL_OP_KEEP;
    mesh_state.stencilTestBackStencilTestPassDepthTestFailOp  = REI_STENCIL_OP_KEEP;
    mesh_state.stencilTestBackCompareOp                       = REI_COMPARE_OP_NEVER;
    mesh_state.stencilTestBackCompareMask                     = 0;
    mesh_state.stencilTestBackWriteMask                       = 0;
    mesh_state.stencilTestBackReference                       = 0;
    mesh_state.blendLogicOpEnable                             = 0;
    mesh_state.blendLogicOp                                   = REI_LOGIC_OP_CLEAR;
    mesh_state.blendConstants[0]                              = 0;
    mesh_state.blendConstants[1]                              = 0;
    mesh_state.blendConstants[2]                              = 0;
    mesh_state.blendConstants[3]                              = 0;
    mesh_state.outputColorWriteEnableR                        = 1;
    mesh_state.outputColorWriteEnableG                        = 1;
    mesh_state.outputColorWriteEnableB                        = 1;
    mesh_state.outputColorWriteEnableA                        = 1;
    mesh_state.outputColorBlendEnable                         = 0;
    mesh_state.outputColorBlendColorFactorSource              = REI_BLEND_FACTOR_ZERO;
    mesh_state.outputColorBlendColorFactorTarget              = REI_BLEND_FACTOR_ZERO;
    mesh_state.outputColorBlendColorOp                        = REI_BLEND_OP_ADD;
    mesh_state.outputColorBlendAlphaFactorSource              = REI_BLEND_FACTOR_ZERO;
    mesh_state.outputColorBlendAlphaFactorTarget              = REI_BLEND_FACTOR_ZERO;
    mesh_state.outputColorBlendAlphaOp                        = REI_BLEND_OP_ADD;
    reiCommandMeshSetState(ctx, &mesh_state);
    reiCommandMeshSet(ctx);
    for (int i = 0, mesh_vertex_count = countof(mesh_vertices) / 3; i < mesh_vertex_count; i += 1) {
      reiCommandMeshNormal(ctx, mesh_normals[i * 3 + 0],
                                mesh_normals[i * 3 + 1],
                                mesh_normals[i * 3 + 2]);
      reiCommandMeshPosition(ctx, mesh_vertices[i * 3 + 0],
                                  mesh_vertices[i * 3 + 1],
                                  mesh_vertices[i * 3 + 2], 1);
    }
    reiCommandMeshEnd(ctx);
  reiCommandListEnd(ctx);

  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  float pos_x = 0;
  float pos_y = 0;
  float pos_z = -2.f;
  float rot_x = 0;
  float rot_y = 0;

  glfwPollEvents();

  double start_mouse_x = 0;
  double start_mouse_y = 0;
  glfwGetCursorPos(window, &start_mouse_x, &start_mouse_y);
  double mouse_x_prev = start_mouse_x;
  double mouse_y_prev = start_mouse_y;

  for (; glfwWindowShouldClose(window) == 0;) {
    glfwPollEvents();

    double mouse_x = 0;
    double mouse_y = 0;
    glfwGetCursorPos(window, &mouse_x, &mouse_y);

    float mouse_disp_x = (float)(mouse_x_prev - mouse_x) * 0.0035f;
    float mouse_disp_y = (float)(mouse_y - mouse_y_prev) * 0.0035f;

    rot_x += mouse_disp_x;
    rot_y += mouse_disp_y;

    float move_vec_x = 0;
    float move_vec_y = 0;
    float move_vec_z = 0;

    move_vec_x += glfwGetKey(window, GLFW_KEY_D);
    move_vec_x -= glfwGetKey(window, GLFW_KEY_A);

    move_vec_y += glfwGetKey(window, GLFW_KEY_E);
    move_vec_y -= glfwGetKey(window, GLFW_KEY_Q);

    move_vec_z += glfwGetKey(window, GLFW_KEY_W);
    move_vec_z -= glfwGetKey(window, GLFW_KEY_S);

    float move_vec_len = sqrtf(move_vec_x*move_vec_x + move_vec_y*move_vec_y + move_vec_z*move_vec_z);
    if (move_vec_len != 0) {
      move_vec_x /= move_vec_len;
      move_vec_y /= move_vec_len;
      move_vec_z /= move_vec_len;
    }

    {
      float rot_cos = cosf(rot_y);
      float rot_sin = sinf(rot_y);
      float pos_y = move_vec_y;
      float pos_z = move_vec_z;
      move_vec_y = pos_y * rot_cos - pos_z * rot_sin;
      move_vec_z = pos_z * rot_cos + pos_y * rot_sin;
    }

    {
      float rot_cos = cosf(rot_x);
      float rot_sin = sinf(rot_x);
      float pos_x = move_vec_x;
      float pos_z = move_vec_z;
      move_vec_x = pos_x * rot_cos - pos_z * rot_sin;
      move_vec_z = pos_z * rot_cos + pos_x * rot_sin;
    }

    pos_x += move_vec_x * 0.025;
    pos_y += move_vec_y * 0.025;
    pos_z += move_vec_z * 0.025;

    reiBindProgram(ctx, REI_PROGRAM_BINDING_VERTEX, vp_skybox_program);
    reiSetProgramLocalValue(ctx, REI_PROGRAM_BINDING_VERTEX, 0, cosf(-rot_x), sinf(-rot_x), cosf(-rot_y), sinf(-rot_y));
    reiBindProgram(ctx, REI_PROGRAM_BINDING_VERTEX, 0);

    reiBindProgram(ctx, REI_PROGRAM_BINDING_VERTEX, vp_program);
    reiSetProgramLocalValue(ctx, REI_PROGRAM_BINDING_VERTEX,   0, pos_x, pos_y, pos_z, 0);
    reiSetProgramLocalValue(ctx, REI_PROGRAM_BINDING_FRAGMENT, 0, pos_x, pos_y, pos_z, 0);
    reiSetProgramLocalValue(ctx, REI_PROGRAM_BINDING_VERTEX,   1, cosf(-rot_x), sinf(-rot_x), cosf(-rot_y), sinf(-rot_y));
    reiBindProgram(ctx, REI_PROGRAM_BINDING_VERTEX, 0);

    reiSubmitCommandLists(ctx, 1, &list);

    glfwSwapBuffers(window);

    mouse_x_prev = mouse_x;
    mouse_y_prev = mouse_y;
  }

  glfwDestroyWindow(window);
  glfwTerminate();
}
