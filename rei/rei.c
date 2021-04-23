#include "rei.h"
#include <string.h> // For strlen
#include <stdio.h>  // For printf

#ifdef _WIN32
#pragma comment(lib, "../glfw/lib/glfw3.lib")
#pragma comment(lib, "opengl32.lib")
#endif

// Context creation

typedef struct ReiTypeContext {
  void (*glGenProgramsARB)(int programsCount, ReiHandleProgram * outPrograms);
  void (*glDeleteProgramsARB)(int programsCount, ReiHandleProgram * programs);
  void (*glBindProgramARB)(ReiProgramBinding binding, ReiHandleProgram program);
  void (*glProgramStringARB)(ReiProgramBinding binding, unsigned stringType, unsigned stringLengthWithoutNullTerminator, const char * string);
  void (*glGenTextures)(int texturesCount, ReiHandleTexture * outTextures);
  void (*glDeleteTextures)(int texturesCount, ReiHandleTexture * textures);
  void (*glActiveTexture)(unsigned slot);
  void (*glBindTexture)(ReiTextureBinding binding, ReiHandleTexture texture);
  void (*glTexParameteri)(ReiTextureBinding binding, unsigned parameter, int value);
  void (*glPixelStorei)(unsigned parameter, int value);
  void (*glTexImage2D)(ReiTextureBinding binding, int bindingLevel, ReiTextureTexelFormat bindingTexelFormat, int width, int height, int setTo0, ReiTextureTexelFormat texelsFormat, ReiTextureTexelType texelsType, const void * texels);
  void (*glTexSubImage2D)(ReiTextureBinding binding, int bindingLevel, int bindingX, int bindingY, int width, int height, ReiTextureTexelFormat texelsFormat, ReiTextureTexelType texelsType, const void * texels);
  void (*glCopyTexImage2D)(ReiTextureBinding binding, int bindingLevel, ReiTextureTexelFormat bindingTexelFormat, int backbufferX, int backbufferY, int width, int height, int setTo0);
  void (*glCopyTexSubImage2D)(ReiTextureBinding binding, int bindingLevel, int bindingX, int bindingY, int backbufferX, int backbufferY, int width, int height);
  void (*glReadPixels)(int backbufferX, int backbufferY, int width, int height, ReiTextureTexelFormat texelsFormat, ReiTextureTexelType texelsType, void * outTexels);
  ReiHandleCommandList (*glGenLists)(int setTo1);
  void (*glDeleteLists)(ReiHandleCommandList list, int setTo1);
  void (*glNewList)(ReiHandleCommandList list, unsigned mode);
  void (*glEndList)();
  void (*glClearDepthf)(float value);
  void (*glClearStencil)(int value);
  void (*glClearColor)(float r, float g, float b, float a);
  void (*glClear)(ReiClearFlags mask);
  void (*glEnable)(unsigned cap);
  void (*glDisable)(unsigned cap);
  void (*glViewport)(int x, int y, int width, int height);
  void (*glScissor)(int x, int y, int width, int height);
  void (*glCullFace)(unsigned mode);
  void (*glFrontFace)(unsigned mode);
  void (*glPolygonOffset)(float factor, float units);
  void (*glDepthMask)(unsigned char flag);
  void (*glDepthFunc)(unsigned func);
  void (*glStencilOpSeparate)(unsigned face, unsigned sfail, unsigned dpfail, unsigned dppass);
  void (*glStencilFuncSeparate)(unsigned face, unsigned func, int ref, unsigned mask);
  void (*glStencilMaskSeparate)(unsigned face, unsigned mask);
  void (*glLogicOp)(unsigned opcode);
  void (*glBlendColor)(float red, float green, float blue, float alpha);
  void (*glBlendFuncSeparate)(unsigned sfactorRGB, unsigned dfactorRGB, unsigned sfactorAlpha, unsigned dfactorAlpha);
  void (*glBlendEquationSeparate)(unsigned modeRGB, unsigned modeAlpha);
  void (*glColorMask)(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha);
  void (*glBegin)(unsigned topology);
  void (*glEnd)();
  void (*glMultiTexCoord4f)(unsigned slot, float x, float y, float z, float w);
  void (*glColor4f)(float r, float g, float b, float a);
  void (*glNormal3f)(float x, float y, float z);
  void (*glVertex4f)(float x, float y, float z, float w);
  void (*glProgramEnvParameter4fARB)(ReiProgramBinding binding, unsigned index, float x, float y, float z, float w);
  void (*glProgramLocalParameter4fARB)(ReiProgramBinding binding, unsigned index, float x, float y, float z, float w);
  void (*glCallList)(ReiHandleCommandList list);
  ReiStatus (*glGetError)();
  const char * (*glGetString)(unsigned parameter);
  void (*glGetIntegerv)(unsigned parameter, int * outValue);
  void (*glFlush)();
  void (*glFinish)();
} ReiTypeContext;

void reiCreateContext(ReiTypeProcedureGetProcAddress getProcAddress, ReiContext * outContext) {
  outContext->glGenProgramsARB = getProcAddress("glGenProgramsARB");
  outContext->glDeleteProgramsARB = getProcAddress("glDeleteProgramsARB");
  outContext->glBindProgramARB = getProcAddress("glBindProgramARB");
  outContext->glProgramStringARB = getProcAddress("glProgramStringARB");
  outContext->glGenTextures = getProcAddress("glGenTextures");
  outContext->glDeleteTextures = getProcAddress("glDeleteTextures");
  outContext->glActiveTexture = getProcAddress("glActiveTexture");
  outContext->glBindTexture = getProcAddress("glBindTexture");
  outContext->glTexParameteri = getProcAddress("glTexParameteri");
  outContext->glPixelStorei = getProcAddress("glPixelStorei");
  outContext->glTexImage2D = getProcAddress("glTexImage2D");
  outContext->glTexSubImage2D = getProcAddress("glTexSubImage2D");
  outContext->glCopyTexImage2D = getProcAddress("glCopyTexImage2D");
  outContext->glCopyTexSubImage2D = getProcAddress("glCopyTexSubImage2D");
  outContext->glReadPixels = getProcAddress("glReadPixels");
  outContext->glGenLists = getProcAddress("glGenLists");
  outContext->glDeleteLists = getProcAddress("glDeleteLists");
  outContext->glNewList = getProcAddress("glNewList");
  outContext->glEndList = getProcAddress("glEndList");
  outContext->glClearDepthf = getProcAddress("glClearDepthf");
  outContext->glClearStencil = getProcAddress("glClearStencil");
  outContext->glClearColor = getProcAddress("glClearColor");
  outContext->glClear = getProcAddress("glClear");
  outContext->glEnable = getProcAddress("glEnable");
  outContext->glDisable = getProcAddress("glDisable");
  outContext->glViewport = getProcAddress("glViewport");
  outContext->glScissor = getProcAddress("glScissor");
  outContext->glCullFace = getProcAddress("glCullFace");
  outContext->glFrontFace = getProcAddress("glFrontFace");
  outContext->glPolygonOffset = getProcAddress("glPolygonOffset");
  outContext->glDepthMask = getProcAddress("glDepthMask");
  outContext->glDepthFunc = getProcAddress("glDepthFunc");
  outContext->glStencilOpSeparate = getProcAddress("glStencilOpSeparate");
  outContext->glStencilFuncSeparate = getProcAddress("glStencilFuncSeparate");
  outContext->glStencilMaskSeparate = getProcAddress("glStencilMaskSeparate");
  outContext->glLogicOp = getProcAddress("glLogicOp");
  outContext->glBlendColor = getProcAddress("glBlendColor");
  outContext->glBlendFuncSeparate = getProcAddress("glBlendFuncSeparate");
  outContext->glBlendEquationSeparate = getProcAddress("glBlendEquationSeparate");
  outContext->glColorMask = getProcAddress("glColorMask");
  outContext->glBegin = getProcAddress("glBegin");
  outContext->glEnd = getProcAddress("glEnd");
  outContext->glMultiTexCoord4f = getProcAddress("glMultiTexCoord4f");
  outContext->glColor4f = getProcAddress("glColor4f");
  outContext->glNormal3f = getProcAddress("glNormal3f");
  outContext->glVertex4f = getProcAddress("glVertex4f");
  outContext->glProgramEnvParameter4fARB = getProcAddress("glProgramEnvParameter4fARB");
  outContext->glProgramLocalParameter4fARB = getProcAddress("glProgramLocalParameter4fARB");
  outContext->glCallList = getProcAddress("glCallList");
  outContext->glGetError = getProcAddress("glGetError");
  outContext->glGetString = getProcAddress("glGetString");
  outContext->glGetIntegerv = getProcAddress("glGetIntegerv");
  outContext->glFlush = getProcAddress("glFlush");
  outContext->glFinish = getProcAddress("glFinish");
}

// Program handle creation and destruction

ReiHandleProgram reiCreateProgram(ReiContext * context) {
  const ReiTypeContext * ctx = (ReiTypeContext *)(void *)context;
  ReiHandleProgram out = (ReiHandleProgram)0;
  ctx->glGenProgramsARB(1, &out);
  return out;
}

void reiDestroyProgram(ReiContext * context, ReiHandleProgram program) {
  const ReiTypeContext * ctx = (ReiTypeContext *)(void *)context;
  ctx->glDeleteProgramsARB(1, &program);
}

// Program binding

void reiBindProgram(ReiContext * context, ReiProgramBinding binding, ReiHandleProgram program) {
  const ReiTypeContext * ctx = (ReiTypeContext *)(void *)context;
  ctx->glBindProgramARB(binding, program);
}

// Program initialization

void reiProgramInitialize(ReiContext * context, ReiProgramBinding binding, unsigned codeStringLengthWithoutNullTerminator, const char * codeString) {
  const ReiTypeContext * ctx = (ReiTypeContext *)(void *)context;
  ctx->glProgramStringARB(binding, 0x8875, codeStringLengthWithoutNullTerminator, codeString); // GL_PROGRAM_FORMAT_ASCII
}

// Texture handle creation and deletion

ReiHandleTexture reiCreateTexture(ReiContext * context) {
  const ReiTypeContext * ctx = (ReiTypeContext *)(void *)context;
  ReiHandleTexture out = (ReiHandleTexture)0;
  ctx->glGenTextures(1, &out);
  return out;
}

void reiDestroyTexture(ReiContext * context, ReiHandleTexture texture) {
  const ReiTypeContext * ctx = (ReiTypeContext *)(void *)context;
  ctx->glDeleteTextures(1, &texture);
}

// Texture binding

void reiSetActiveTextureSlot(ReiContext * context, unsigned slot) {
  const ReiTypeContext * ctx = (ReiTypeContext *)(void *)context;
  ctx->glActiveTexture(0x84C0 + slot); // GL_TEXTURE0
}

void reiBindTextureToActiveTextureSlot(ReiContext * context, ReiTextureBinding binding, ReiHandleTexture texture) {
  const ReiTypeContext * ctx = (ReiTypeContext *)(void *)context;
  ctx->glBindTexture(binding, texture);
}

// Texture state

void reiTextureSetStateMipmap(ReiContext * context, ReiTextureBinding binding, ReiBool32 generateMipLevels) {
  const ReiTypeContext * ctx = (ReiTypeContext *)(void *)context;
  ctx->glTexParameteri(binding, 0x8191, generateMipLevels); // GL_GENERATE_MIPMAP
}

void reiTextureSetStateSampler(ReiContext * context, ReiTextureBinding binding, ReiSamplerFiltering magFiltering, ReiSamplerFiltering minFiltering, ReiSamplerBehaviorOutsideTextureCoordinate behaviorOutsideTextureCoordinateU, ReiSamplerBehaviorOutsideTextureCoordinate behaviorOutsideTextureCoordinateV, int maxAnisotropy) {
  const ReiTypeContext * ctx = (ReiTypeContext *)(void *)context;
  ctx->glTexParameteri(binding, 0x2800, magFiltering); // GL_TEXTURE_MAG_FILTER
  ctx->glTexParameteri(binding, 0x2801, minFiltering); // GL_TEXTURE_MIN_FILTER
  ctx->glTexParameteri(binding, 0x2802, behaviorOutsideTextureCoordinateU); // GL_TEXTURE_WRAP_S
  ctx->glTexParameteri(binding, 0x2803, behaviorOutsideTextureCoordinateV); // GL_TEXTURE_WRAP_T
  ctx->glTexParameteri(binding, 0x84FE, maxAnisotropy); // GL_TEXTURE_MAX_ANISOTROPY
}

// Texture texels upload and backbuffer readback

void reiTextureDefineAndCopyFromCpu(ReiContext * context, ReiTextureBinding binding, int bindingLevel, ReiTextureTexelFormat bindingTexelFormat, int width, int height, ReiTextureTexelFormat texelsFormat, ReiTextureTexelType texelsType, int texelsBytesAlignment, const void * texels) {
  const ReiTypeContext * ctx = (ReiTypeContext *)(void *)context;
  ctx->glPixelStorei(0x0CF5, texelsBytesAlignment); // GL_UNPACK_ALIGNMENT
  ctx->glTexImage2D(binding, bindingLevel, bindingTexelFormat, width, height, 0, texelsFormat, texelsType, texels);
}

void reiTextureDefineAndCopyFromBackbuffer(ReiContext * context, ReiTextureBinding binding, int bindingLevel, ReiTextureTexelFormat bindingTexelFormat, int backbufferX, int backbufferY, int width, int height) {
  const ReiTypeContext * ctx = (ReiTypeContext *)(void *)context;
  ctx->glCopyTexImage2D(binding, bindingLevel, bindingTexelFormat, backbufferX, backbufferY, width, height, 0);
}

void reiTextureCopyFromCpu(ReiContext * context, ReiTextureBinding binding, int bindingLevel, int bindingX, int bindingY, int width, int height, ReiTextureTexelFormat texelsFormat, ReiTextureTexelType texelsType, int texelsBytesAlignment, const void * texels) {
  const ReiTypeContext * ctx = (ReiTypeContext *)(void *)context;
  ctx->glPixelStorei(0x0CF5, texelsBytesAlignment); // GL_UNPACK_ALIGNMENT
  ctx->glTexSubImage2D(binding, bindingLevel, bindingX, bindingY, width, height, texelsFormat, texelsType, texels);
}

void reiTextureCopyFromBackbuffer(ReiContext * context, ReiTextureBinding binding, int bindingLevel, int bindingX, int bindingY, int backbufferX, int backbufferY, int width, int height) {
  const ReiTypeContext * ctx = (ReiTypeContext *)(void *)context;
  ctx->glCopyTexSubImage2D(binding, bindingLevel, bindingX, bindingY, backbufferX, backbufferY, width, height);
}

void reiReadbackToCpuFromBackbuffer(ReiContext * context, int backbufferX, int backbufferY, int width, int height, ReiTextureTexelFormat texelsFormat, ReiTextureTexelType texelsType, int texelsBytesAlignment, void * outTexels) {
  const ReiTypeContext * ctx = (ReiTypeContext *)(void *)context;
  ctx->glPixelStorei(0x0D05, texelsBytesAlignment); // GL_PACK_ALIGNMENT
  ctx->glReadPixels(backbufferX, backbufferY, width, height, texelsFormat, texelsType, outTexels);
}

// Command list creation and deletion

ReiHandleCommandList reiCreateCommandList(ReiContext * context) {
  const ReiTypeContext * ctx = (ReiTypeContext *)(void *)context;
  return ctx->glGenLists(1);
}

void reiDestroyCommandList(ReiContext * context, ReiHandleCommandList list) {
  const ReiTypeContext * ctx = (ReiTypeContext *)(void *)context;
  ctx->glDeleteLists(list, 1);
}

// Command list recording

void reiCommandListSet(ReiContext * context, ReiHandleCommandList list) {
  const ReiTypeContext * ctx = (ReiTypeContext *)(void *)context;
  ctx->glNewList(list, 0x1300); // GL_COMPILE
  ctx->glEnable(REI_PROGRAM_BINDING_VERTEX);
  ctx->glEnable(REI_PROGRAM_BINDING_FRAGMENT);
  ctx->glEnable(0x0C11); // GL_SCISSOR_TEST
}

void reiCommandListEnd(ReiContext * context) {
  const ReiTypeContext * ctx = (ReiTypeContext *)(void *)context;
  ctx->glEndList();
}

// Command recording viewport and scissor size

void reiCommandSetViewport(ReiContext * context, int x, int y, int width, int height) {
  const ReiTypeContext * ctx = (ReiTypeContext *)(void *)context;
  ctx->glViewport(x, y, width, height);
}

void reiCommandSetScissor(ReiContext * context, int x, int y, int width, int height) {
  const ReiTypeContext * ctx = (ReiTypeContext *)(void *)context;
  ctx->glScissor(x, y, width, height);
}

// Command recording of backbuffer(s) clearing

void reiCommandClear(ReiContext * context, ReiClearFlags clear, float depthValue, unsigned stencilValue, float colorR, float colorG, float colorB, float colorA) {
  const ReiTypeContext * ctx = (ReiTypeContext *)(void *)context;
  ctx->glDepthMask(1);
  ctx->glClearDepthf(depthValue);
  ctx->glStencilMaskSeparate(0x0408, 0xFF); // GL_FRONT_AND_BACK
  ctx->glClearStencil(stencilValue);
  ctx->glColorMask(1, 1, 1, 1);
  ctx->glClearColor(colorR, colorG, colorB, colorA);
  ctx->glClear(clear);
}

// Command recording of mesh state and mesh work domain execution

void reiCommandMeshSetState(ReiContext * context, ReiMeshState * state) {
  const ReiTypeContext * ctx = (ReiTypeContext *)(void *)context;
  state->rasterizationDepthClampEnable == 1 ? ctx->glEnable(0x864F) : ctx->glDisable(0x864F);  // GL_DEPTH_CLAMP
  if (state->rasterizationCullMode == REI_CULL_MODE_NONE) {
    ctx->glDisable(0x0B44); // GL_CULL_FACE
  } else {
    ctx->glEnable(0x0B44);  // GL_CULL_FACE
    ctx->glCullFace(state->rasterizationCullMode);
  }
  ctx->glFrontFace(state->rasterizationFrontFace);
  state->rasterizationDepthBiasEnable == 1 ? ctx->glEnable(0x8037) : ctx->glDisable(0x8037); // GL_POLYGON_OFFSET_FILL
  ctx->glPolygonOffset(state->rasterizationDepthBiasSlopeFactor, state->rasterizationDepthBiasConstantFactor);
  state->multisampleEnable == 1 ? ctx->glEnable(0x809D)  : ctx->glDisable(0x809D); // GL_MULTISAMPLE
  state->multisampleAlphaToCoverageEnable == 1 ? ctx->glEnable(0x809E) : ctx->glDisable(0x809E); // GL_SAMPLE_ALPHA_TO_COVERAGE
  state->multisampleAlphaToOneEnable == 1 ? ctx->glEnable(0x809F) : ctx->glDisable(0x809F); // GL_SAMPLE_ALPHA_TO_ONE
  state->depthTestEnable == 1 ? ctx->glEnable(0x0B71) : ctx->glDisable(0x0B71); // GL_DEPTH_TEST
  ctx->glDepthMask(state->depthTestDepthWriteEnable == 1 ? 1 : 0);
  ctx->glDepthFunc(state->depthTestDepthCompareOp);
  state->stencilTestEnable == 1 ? ctx->glEnable(0x0B90) : ctx->glDisable(0x0B90); // GL_STENCIL_TEST
  ctx->glStencilOpSeparate(0x0404, state->stencilTestFrontStencilTestFailOp, state->stencilTestFrontStencilTestPassDepthTestFailOp, state->stencilTestFrontStencilTestPassDepthTestPassOp); // GL_FRONT
  ctx->glStencilFuncSeparate(0x0404, state->stencilTestFrontCompareOp, state->stencilTestFrontAndBackReference, state->stencilTestFrontAndBackCompareMask); // GL_FRONT
  ctx->glStencilMaskSeparate(0x0404, state->stencilTestFrontAndBackWriteMask); // GL_FRONT
  ctx->glStencilOpSeparate(0x0405, state->stencilTestBackStencilTestFailOp, state->stencilTestBackStencilTestPassDepthTestFailOp, state->stencilTestBackStencilTestPassDepthTestPassOp); // GL_BACK
  ctx->glStencilFuncSeparate(0x0405, state->stencilTestBackCompareOp, state->stencilTestFrontAndBackReference, state->stencilTestFrontAndBackCompareMask); // GL_BACK
  ctx->glStencilMaskSeparate(0x0405, state->stencilTestFrontAndBackWriteMask); // GL_BACK
  state->blendLogicOpEnable == 1 ? ctx->glEnable(0x0BF2) : ctx->glDisable(0x0BF2); // GL_COLOR_LOGIC_OP
  ctx->glLogicOp(state->blendLogicOp);
  ctx->glBlendColor(state->blendConstants[0], state->blendConstants[1], state->blendConstants[2], state->blendConstants[3]);
  ctx->glColorMask(state->outputColorWriteEnableR == 1 ? 1 : 0, state->outputColorWriteEnableG == 1 ? 1 : 0, state->outputColorWriteEnableB == 1 ? 1 : 0, state->outputColorWriteEnableA == 1 ? 1 : 0);
  state->outputColorBlendEnable == 1 ? ctx->glEnable(0x0BE2) : ctx->glDisable(0x0BE2); // GL_BLEND
  ctx->glBlendFuncSeparate(state->outputColorBlendColorFactorSource, state->outputColorBlendColorFactorTarget, state->outputColorBlendAlphaFactorSource, state->outputColorBlendAlphaFactorTarget);
  ctx->glBlendEquationSeparate(state->outputColorBlendColorOp, state->outputColorBlendAlphaOp);
  ctx->glBindProgramARB(REI_PROGRAM_BINDING_VERTEX,   state->programVertex);
  ctx->glBindProgramARB(REI_PROGRAM_BINDING_FRAGMENT, state->programFragment);
}

void reiCommandMeshSet(ReiContext * context) {
  const ReiTypeContext * ctx = (ReiTypeContext *)(void *)context;
  ctx->glBegin(0x0004); // GL_TRIANGLES
}

void reiCommandMeshEnd(ReiContext * context) {
  const ReiTypeContext * ctx = (ReiTypeContext *)(void *)context;
  ctx->glEnd();
}

void reiCommandMeshTexcoord(ReiContext * context, unsigned index, float x, float y, float z, float w) {
  const ReiTypeContext * ctx = (ReiTypeContext *)(void *)context;
  ctx->glMultiTexCoord4f(0x84C0 + index, x, y, z, w); // GL_TEXTURE0
}

void reiCommandMeshColor(ReiContext * context, float r, float g, float b, float a) {
  const ReiTypeContext * ctx = (ReiTypeContext *)(void *)context;
  ctx->glColor4f(r, g, b, a);
}

void reiCommandMeshNormal(ReiContext * context, float x, float y, float z) {
  const ReiTypeContext * ctx = (ReiTypeContext *)(void *)context;
  ctx->glNormal3f(x, y, z);
}

void reiCommandMeshPosition(ReiContext * context, float x, float y, float z, float w) {
  const ReiTypeContext * ctx = (ReiTypeContext *)(void *)context;
  ctx->glVertex4f(x, y, z, w);
}

// Command list submission and dynamic state setting

void reiSetProgramEnvironmentValue(ReiContext * context, ReiProgramBinding binding, unsigned index, float x, float y, float z, float w) {
  const ReiTypeContext * ctx = (ReiTypeContext *)(void *)context;
  ctx->glProgramEnvParameter4fARB(binding, index, x, y, z, w);
}

void reiSetProgramLocalValue(ReiContext * context, ReiProgramBinding binding, unsigned index, float x, float y, float z, float w) {
  const ReiTypeContext * ctx = (ReiTypeContext *)(void *)context;
  ctx->glProgramLocalParameter4fARB(binding, index, x, y, z, w);
}

void reiSubmitCommandLists(ReiContext * context, unsigned commandListsCount, const ReiHandleCommandList * commandLists) {
  const ReiTypeContext * ctx = (ReiTypeContext *)(void *)context;
  unsigned i = 0;
  for (i = 0; i < commandListsCount; i += 1) {
    ctx->glCallList(commandLists[i]);
  }
}

// Error handling

ReiStatus reiGetStatus(ReiContext * context) {
  const ReiTypeContext * ctx = (ReiTypeContext *)(void *)context;
  return ctx->glGetError();
}

const char * reiGetProgramStatusString(ReiContext * context) {
  const ReiTypeContext * ctx = (ReiTypeContext *)(void *)context;
  return ctx->glGetString(0x8874); // GL_PROGRAM_ERROR_STRING
}

int reiGetProgramStatusCodePosition(ReiContext * context) {
  const ReiTypeContext * ctx = (ReiTypeContext *)(void *)context;
  int out = 0;
  ctx->glGetIntegerv(0x864B, &out); // GL_PROGRAM_ERROR_POSITION
  return out;
}

// Misc

void reiFlush(ReiContext * context) {
  const ReiTypeContext * ctx = (ReiTypeContext *)(void *)context;
  ctx->glFlush();
}

void reiFinish(ReiContext * context) {
  const ReiTypeContext * ctx = (ReiTypeContext *)(void *)context;
  ctx->glFinish();
}

void reiCheckCode(ReiContext * context, const char * codeVertex, const char * codeFragment) {
  const char * error = NULL;
  if (codeVertex != 0) {
    ReiHandleProgram programVertex = reiCreateProgram(context);
    reiBindProgram(context, REI_PROGRAM_BINDING_VERTEX, programVertex);
    reiProgramInitialize(context, REI_PROGRAM_BINDING_VERTEX, strlen(codeVertex), codeVertex);
    reiBindProgram(context, REI_PROGRAM_BINDING_VERTEX, 0);
    error = reiGetProgramStatusString(context);
    if (error != NULL) {
      if (error[0] != '\0') {
        printf("%s", "[reiCheckCode] Vertex code error:\n");
        printf("%s", error);
        fflush(stdout);
      }
    }
    reiDestroyProgram(context, programVertex);
  } else {
    printf("%s", "[reiCheckCode] Warning: codeVertex == NULL\n");
    fflush(stdout);
  }
  if (codeFragment != 0) {
    ReiHandleProgram programFragment = reiCreateProgram(context);
    reiBindProgram(context, REI_PROGRAM_BINDING_FRAGMENT, programFragment);
    reiProgramInitialize(context, REI_PROGRAM_BINDING_FRAGMENT, strlen(codeFragment), codeFragment);
    reiBindProgram(context, REI_PROGRAM_BINDING_FRAGMENT, 0);
    error = reiGetProgramStatusString(context);
    if (error != NULL) {
      if (error[0] != '\0') {
        printf("%s", "[reiCheckCode] Fragment code error:\n");
        printf("%s", error);
        fflush(stdout);
      }
    }
    reiDestroyProgram(context, programFragment);
  } else {
    printf("%s", "[reiCheckCode] Warning: codeFragment == NULL\n");
    fflush(stdout);
  }
}
