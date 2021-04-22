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

#define countof(x) (sizeof(x) / sizeof((x)[0]))

int main() {
  glfwInit();
  glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 1);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
  glfwWindowHint(GLFW_RESIZABLE, 0);
  GLFWwindow * window = glfwCreateWindow(700, 700, "", NULL, NULL);
  glfwMakeContextCurrent(window);

  ReiContext   context = {};
  ReiContext * ctx     = &context;
  reiCreateContext((ReiTypeProcedureGetProcAddress)glfwGetProcAddress, &context);

  typedef struct {
    unsigned char r, g, b, a;
  } color;

  const color colors[] = {
    {  0,   0,   0, 255},
    {255,   0,   0, 255},
    {  0, 255,   0, 255},
    {  0,   0, 255, 255},
    {255,   0, 255, 255},
  };

#define _ 0

  const int image1[] = {
    _,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,
    _,1,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,2,_,
    _,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,
    _,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,
    _,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,
    _,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,
    _,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,
    _,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,
    _,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,
    _,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,
    _,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,
    _,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,
    _,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,
    _,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,
    _,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,
    _,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,
    _,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,
    _,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,
    _,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,
    _,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,
    _,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,
    _,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,
    _,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,
    _,3,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,4,_,
    _,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,
  };

  const int image2[] = {
    _,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,
    _,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,
    _,_,1,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,2,_,_,
    _,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,
    _,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,
    _,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,
    _,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,
    _,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,
    _,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,
    _,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,
    _,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,
    _,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,
    _,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,
    _,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,
    _,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,
    _,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,
    _,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,
    _,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,
    _,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,
    _,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,
    _,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,
    _,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,
    _,_,3,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,4,_,_,
    _,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,
    _,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,
  };

  color texture1[countof(image1)];
  for (int i = 0; i < countof(image1); i += 1) {
    texture1[i] = colors[image1[i]];
  }

  color texture2[countof(image2)];
  for (int i = 0; i < countof(image2); i += 1) {
    texture2[i] = colors[image2[i]];
  }

  char vp_string[] =
    "!!ARBvp1.0"
    "TEMP R0;"
    "MAD R0.xy, vertex.position, 0.5, 0.5;"
    "MOV result.texcoord[0].x, R0;"
    "ADD result.texcoord[0].y,-R0, 1;"
    "MOV result.position, vertex.position;"
    "END";

  char fp_string[] =
    "!!ARBfp1.0"
    "TEX result.color, fragment.texcoord[0], texture[0], 2D;"
    "END";

  ReiHandleProgram vp_program = reiCreateProgram(ctx);
  ReiHandleProgram fp_program = reiCreateProgram(ctx);

  reiBindProgram(ctx, REI_PROGRAM_BINDING_VERTEX, vp_program);
  reiProgramInitialize(ctx, REI_PROGRAM_BINDING_VERTEX, sizeof(vp_string)-1, vp_string);
  reiBindProgram(ctx, REI_PROGRAM_BINDING_VERTEX, 0);

  reiBindProgram(ctx, REI_PROGRAM_BINDING_FRAGMENT, fp_program);
  reiProgramInitialize(ctx, REI_PROGRAM_BINDING_FRAGMENT, sizeof(fp_string)-1, fp_string);
  reiBindProgram(ctx, REI_PROGRAM_BINDING_FRAGMENT, 0);

  ReiHandleTexture texture1Handle = reiCreateTexture(ctx);

  reiSetActiveTextureSlot(ctx, 0);
  reiBindTextureToActiveTextureSlot(ctx, REI_TEXTURE_BINDING_2D, texture1Handle);
  reiTextureSetStateMipmap(ctx, REI_TEXTURE_BINDING_2D, 0);
  reiTextureSetStateSampler(ctx, REI_TEXTURE_BINDING_2D, REI_SAMPLER_FILTERING_NEAREST, REI_SAMPLER_FILTERING_NEAREST, REI_SAMPLER_BEHAVIOR_OUTSIDE_TEXTURE_COORDINATE_REPEAT, REI_SAMPLER_BEHAVIOR_OUTSIDE_TEXTURE_COORDINATE_REPEAT, 1);
  reiTextureDefineAndCopyFromCpu(ctx, REI_TEXTURE_BINDING_2D,
    0,
    REI_TEXTURE_TEXEL_FORMAT_RGBA,
    (int)sqrtf(countof(image1)), // Assuming square texture
    (int)sqrtf(countof(image1)), // Assuming square texture
    REI_TEXTURE_TEXEL_FORMAT_RGBA,
    REI_TEXTURE_TEXEL_TYPE_UBYTE,
    4,
    texture1);
  reiBindTextureToActiveTextureSlot(ctx, REI_TEXTURE_BINDING_2D, 0);

  ReiHandleTexture texture2Handle = reiCreateTexture(ctx);

  reiSetActiveTextureSlot(ctx, 0);
  reiBindTextureToActiveTextureSlot(ctx, REI_TEXTURE_BINDING_2D, texture2Handle);
  reiTextureSetStateMipmap(ctx, REI_TEXTURE_BINDING_2D, 0);
  reiTextureSetStateSampler(ctx, REI_TEXTURE_BINDING_2D, REI_SAMPLER_FILTERING_NEAREST, REI_SAMPLER_FILTERING_NEAREST, REI_SAMPLER_BEHAVIOR_OUTSIDE_TEXTURE_COORDINATE_REPEAT, REI_SAMPLER_BEHAVIOR_OUTSIDE_TEXTURE_COORDINATE_REPEAT, 1);
  reiTextureDefineAndCopyFromCpu(ctx, REI_TEXTURE_BINDING_2D,
    0,
    REI_TEXTURE_TEXEL_FORMAT_RGBA,
    (int)sqrtf(countof(image2)), // Assuming square texture
    (int)sqrtf(countof(image2)), // Assuming square texture
    REI_TEXTURE_TEXEL_FORMAT_RGBA,
    REI_TEXTURE_TEXEL_TYPE_UBYTE,
    4,
    texture2);
  reiBindTextureToActiveTextureSlot(ctx, REI_TEXTURE_BINDING_2D, 0);

  ReiHandleCommandList list_clear = reiCreateCommandList(ctx);
  reiCommandListSet(ctx, list_clear);
    reiCommandSetViewport(ctx, 0, 0, 700, 700);
    reiCommandSetScissor(ctx, 0, 0, 700, 700);
    reiCommandClear(ctx, REI_CLEAR_COLOR_BIT, 0.f, 0, 0.f, 0.f, 0.f, 1.f);
  reiCommandListEnd(ctx);

  ReiMeshState mesh_state = {};
  mesh_state.programVertex                                  = vp_program;
  mesh_state.programFragment                                = fp_program;
  mesh_state.rasterizationDepthClampEnable                  = 0;
  mesh_state.rasterizationCullMode                          = REI_CULL_MODE_NONE;
  mesh_state.rasterizationFrontFace                         = REI_FRONT_FACE_COUNTER_CLOCKWISE;
  mesh_state.rasterizationDepthBiasEnable                   = 0;
  mesh_state.rasterizationDepthBiasConstantFactor           = 0;
  mesh_state.rasterizationDepthBiasSlopeFactor              = 0;
  mesh_state.multisampleEnable                              = 0;
  mesh_state.multisampleAlphaToCoverageEnable               = 0;
  mesh_state.multisampleAlphaToOneEnable                    = 0;
  mesh_state.depthTestEnable                                = 0;
  mesh_state.depthTestDepthWriteEnable                      = 0;
  mesh_state.depthTestDepthCompareOp                        = REI_COMPARE_OP_GREATER_OR_EQUAL;
  mesh_state.stencilTestEnable                              = 0;
  mesh_state.stencilTestFrontStencilTestFailOp              = REI_STENCIL_OP_KEEP;
  mesh_state.stencilTestFrontStencilTestPassDepthTestPassOp = REI_STENCIL_OP_KEEP;
  mesh_state.stencilTestFrontStencilTestPassDepthTestFailOp = REI_STENCIL_OP_KEEP;
  mesh_state.stencilTestFrontCompareOp                      = REI_COMPARE_OP_NEVER;
  mesh_state.stencilTestBackStencilTestFailOp               = REI_STENCIL_OP_KEEP;
  mesh_state.stencilTestBackStencilTestPassDepthTestPassOp  = REI_STENCIL_OP_KEEP;
  mesh_state.stencilTestBackStencilTestPassDepthTestFailOp  = REI_STENCIL_OP_KEEP;
  mesh_state.stencilTestBackCompareOp                       = REI_COMPARE_OP_NEVER;
  mesh_state.stencilTestFrontAndBackCompareMask             = 0;
  mesh_state.stencilTestFrontAndBackWriteMask               = 0;
  mesh_state.stencilTestFrontAndBackReference               = 0;
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
  ReiHandleCommandList list1 = reiCreateCommandList(ctx);
  reiCommandListSet(ctx, list1);
    reiSetActiveTextureSlot(ctx, 0);
    reiBindTextureToActiveTextureSlot(ctx, REI_TEXTURE_BINDING_2D, texture1Handle);
    reiCommandMeshSetState(ctx, &mesh_state);
    reiCommandMeshSet(ctx);
    reiCommandMeshPosition(ctx, -1, 3, 0, 1);
    reiCommandMeshPosition(ctx, -1,-1, 0, 1);
    reiCommandMeshPosition(ctx,  3,-1, 0, 1);
    reiCommandMeshEnd(ctx);
  reiCommandListEnd(ctx);

  ReiHandleCommandList list2 = reiCreateCommandList(ctx);
  reiCommandListSet(ctx, list2);
    reiSetActiveTextureSlot(ctx, 0);
    reiBindTextureToActiveTextureSlot(ctx, REI_TEXTURE_BINDING_2D, texture2Handle);
    reiCommandMeshSetState(ctx, &mesh_state);
    reiCommandMeshSet(ctx);
    reiCommandMeshPosition(ctx, -1, 3, 0, 1);
    reiCommandMeshPosition(ctx, -1,-1, 0, 1);
    reiCommandMeshPosition(ctx,  3,-1, 0, 1);
    reiCommandMeshEnd(ctx);
  reiCommandListEnd(ctx);

  unsigned frame = 0;

  for (; glfwWindowShouldClose(window) == 0;) {
    glfwPollEvents();
    reiSubmitCommandLists(ctx, 1, &list_clear);
    frame % 2 ? reiSubmitCommandLists(ctx, 1, &list1) : reiSubmitCommandLists(ctx, 1, &list2);
    glfwSwapBuffers(window);
    frame += 1;
  }

  glfwDestroyWindow(window);
  glfwTerminate();
}
