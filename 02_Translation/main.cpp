#if 0
rm -f a.out
cc -std=c99 main.c ../rei/rei.c ../glfw/lib/libglfw.so.3 -lGL
LD_LIBRARY_PATH=../glfw/lib/ ./a.out
exit
#endif

#include "../rei/rei.h"
#define GLFW_INCLUDE_NONE
#include "../glfw/include/glfw3.h"

#include <stdio.h>

const float mesh_vertices[] = {
#include "mesh_vertices.h"
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

  char vp_string[] =
    "!!ARBvp1.0"
    "MOV result.color, vertex.color;"
    "ADD result.position.xyw, vertex.position.xyzz, -program.local[0].xyzz;"
    "MOV result.position.z, 0.1;"
    "END";

  char fp_string[] =
    "!!ARBfp1.0"
    "MOV result.color, fragment.color;"
    "END";

  ReiHandleProgram vp_program = reiCreateProgram(ctx);
  ReiHandleProgram fp_program = reiCreateProgram(ctx);

  reiBindProgram(ctx, REI_PROGRAM_BINDING_VERTEX, vp_program);
  reiProgramInitialize(ctx, REI_PROGRAM_BINDING_VERTEX, sizeof(vp_string)-1, vp_string);
  reiBindProgram(ctx, REI_PROGRAM_BINDING_VERTEX, 0);

  reiBindProgram(ctx, REI_PROGRAM_BINDING_FRAGMENT, fp_program);
  reiProgramInitialize(ctx, REI_PROGRAM_BINDING_FRAGMENT, sizeof(fp_string)-1, fp_string);
  reiBindProgram(ctx, REI_PROGRAM_BINDING_FRAGMENT, 0);

  ReiHandleCommandList list = reiCreateCommandList(ctx);
  reiCommandListSet(ctx, list);
    reiCommandSetViewport(ctx, 0, 0, 700, 700);
    reiCommandSetScissor(ctx, 0, 0, 700, 700);
    reiCommandClear(ctx, REI_CLEAR_DEPTH_BIT | REI_CLEAR_COLOR_BIT, 0.f, 0, 0.f, 0.f, 0.05f, 1.f);
    ReiMeshState mesh_state = {};
    mesh_state.programVertex                                  = vp_program;
    mesh_state.programFragment                                = fp_program;
    mesh_state.rasterizationDepthClampEnable                  = 0;
    mesh_state.rasterizationCullMode                          = REI_CULL_MODE_NONE;
    mesh_state.rasterizationFrontFace                         = REI_FRONT_FACE_COUNTER_CLOCKWISE;
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
    reiCommandMeshSetState(ctx, &mesh_state);
    reiCommandMeshSet(ctx);
    for (int i = 0, mesh_vertices_count = countof(mesh_vertices) / 3; i < mesh_vertices_count; i += 1) {
      reiCommandMeshColor(ctx, i * 0.00025f, 0, 0.1f, 1);
      reiCommandMeshPosition(ctx, mesh_vertices[i * 3 + 0],
                                  mesh_vertices[i * 3 + 1],
                                  mesh_vertices[i * 3 + 2], 1);
    }
    reiCommandMeshEnd(ctx);
  reiCommandListEnd(ctx);

  float pos_x = 0;
  float pos_y = 0;
  float pos_z = -2.f;

  for (; glfwWindowShouldClose(window) == 0;) {
    glfwPollEvents();

    pos_x += glfwGetKey(window, GLFW_KEY_D) * 0.025f;
    pos_x -= glfwGetKey(window, GLFW_KEY_A) * 0.025f;

    pos_y += glfwGetKey(window, GLFW_KEY_E) * 0.025f;
    pos_y -= glfwGetKey(window, GLFW_KEY_Q) * 0.025f;

    pos_z += glfwGetKey(window, GLFW_KEY_W) * 0.025f;
    pos_z -= glfwGetKey(window, GLFW_KEY_S) * 0.025f;

    reiBindProgram(ctx, REI_PROGRAM_BINDING_VERTEX, vp_program);
    reiSetProgramLocalValue(ctx, REI_PROGRAM_BINDING_VERTEX, 0, pos_x, pos_y, pos_z, 0);
    reiBindProgram(ctx, REI_PROGRAM_BINDING_VERTEX, 0);

    reiSubmitCommandLists(ctx, 1, &list);

    glfwSwapBuffers(window);
  }

  glfwDestroyWindow(window);
  glfwTerminate();
}
