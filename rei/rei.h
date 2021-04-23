#pragma once

#ifdef __cplusplus
extern "C" {
#endif

typedef unsigned ReiBool32;

typedef unsigned ReiHandleProgram;
typedef unsigned ReiHandleTexture;
typedef unsigned ReiHandleCommandList;

// Context creation

typedef struct ReiContext {
  void * glGenProgramsARB;
  void * glDeleteProgramsARB;
  void * glBindProgramARB;
  void * glProgramStringARB;
  void * glGenTextures;
  void * glDeleteTextures;
  void * glActiveTexture;
  void * glBindTexture;
  void * glTexParameteri;
  void * glPixelStorei;
  void * glTexImage2D;
  void * glTexSubImage2D;
  void * glCopyTexImage2D;
  void * glCopyTexSubImage2D;
  void * glReadPixels;
  void * glGenLists;
  void * glDeleteLists;
  void * glNewList;
  void * glEndList;
  void * glClearDepthf;
  void * glClearStencil;
  void * glClearColor;
  void * glClear;
  void * glEnable;
  void * glDisable;
  void * glViewport;
  void * glScissor;
  void * glCullFace;
  void * glFrontFace;
  void * glPolygonOffset;
  void * glDepthMask;
  void * glDepthFunc;
  void * glStencilOpSeparate;
  void * glStencilFuncSeparate;
  void * glStencilMaskSeparate;
  void * glLogicOp;
  void * glBlendColor;
  void * glBlendFuncSeparate;
  void * glBlendEquationSeparate;
  void * glColorMask;
  void * glBegin;
  void * glEnd;
  void * glMultiTexCoord4f;
  void * glColor4f;
  void * glNormal3f;
  void * glVertex4f;
  void * glProgramEnvParameter4fARB;
  void * glProgramLocalParameter4fARB;
  void * glCallList;
  void * glGetError;
  void * glGetString;
  void * glGetIntegerv;
  void * glFlush;
  void * glFinish;
} ReiContext;

typedef void * (*ReiTypeProcedureGetProcAddress)(char * procname);

void reiCreateContext(ReiTypeProcedureGetProcAddress getProcAddress, ReiContext * outContext);

// Program handle creation and destruction

ReiHandleProgram reiCreateProgram(ReiContext * context);
void reiDestroyProgram(ReiContext * context, ReiHandleProgram program);

// Program binding

typedef enum ReiProgramBinding {
  REI_PROGRAM_BINDING_VERTEX   = 0x8620,
  REI_PROGRAM_BINDING_FRAGMENT = 0x8804,
} ReiProgramBinding;

void reiBindProgram(ReiContext * context, ReiProgramBinding binding, ReiHandleProgram program);

// Program initialization

void reiProgramInitialize(ReiContext * context, ReiProgramBinding binding, unsigned codeStringLengthWithoutNullTerminator, const char * codeString);

// Texture handle creation and destruction

ReiHandleTexture reiCreateTexture(ReiContext * context);
void reiDestroyTexture(ReiContext * context, ReiHandleTexture texture);

// Texture binding

typedef enum ReiTextureBinding {
  REI_TEXTURE_BINDING_2D              = 0x0DE1,
  REI_TEXTURE_BINDING_CUBE            = 0x8513,
  REI_TEXTURE_BINDING_CUBE_POSITIVE_X = 0x8515,
  REI_TEXTURE_BINDING_CUBE_NEGATIVE_X = 0x8516,
  REI_TEXTURE_BINDING_CUBE_POSITIVE_Y = 0x8517,
  REI_TEXTURE_BINDING_CUBE_NEGATIVE_Y = 0x8518,
  REI_TEXTURE_BINDING_CUBE_POSITIVE_Z = 0x8519,
  REI_TEXTURE_BINDING_CUBE_NEGATIVE_Z = 0x851A,
} ReiTextureBinding;

void reiSetActiveTextureSlot(ReiContext * context, unsigned slot);
void reiBindTextureToActiveTextureSlot(ReiContext * context, ReiTextureBinding binding, ReiHandleTexture texture);

// Texture state

typedef enum ReiSamplerFiltering {
  REI_SAMPLER_FILTERING_NEAREST             = 0x2600,
  REI_SAMPLER_FILTERING_NEAREST_MIP_NEAREST = 0x2700,
  REI_SAMPLER_FILTERING_NEAREST_MIP_LINEAR  = 0x2702,
  REI_SAMPLER_FILTERING_LINEAR              = 0x2601,
  REI_SAMPLER_FILTERING_LINEAR_MIP_NEAREST  = 0x2701,
  REI_SAMPLER_FILTERING_LINEAR_MIP_LINEAR   = 0x2703,
} ReiSamplerFiltering;

typedef enum ReiSamplerBehaviorOutsideTextureCoordinate {
  REI_SAMPLER_BEHAVIOR_OUTSIDE_TEXTURE_COORDINATE_REPEAT              = 0x2901,
  REI_SAMPLER_BEHAVIOR_OUTSIDE_TEXTURE_COORDINATE_CLAMP_TO_EDGE_VALUE = 0x812F,
} ReiSamplerBehaviorOutsideTextureCoordinate;

void reiTextureSetStateMipmap(ReiContext * context, ReiTextureBinding binding, ReiBool32 generateMipLevels);
void reiTextureSetStateSampler(ReiContext * context, ReiTextureBinding binding, ReiSamplerFiltering magFiltering, ReiSamplerFiltering minFiltering, ReiSamplerBehaviorOutsideTextureCoordinate behaviorOutsideTextureCoordinateU, ReiSamplerBehaviorOutsideTextureCoordinate behaviorOutsideTextureCoordinateV, int maxAnisotropy);

// Texture texels upload and backbuffer readback

typedef enum ReiTextureTexelFormat {
  REI_TEXTURE_TEXEL_FORMAT_A    = 0x1906,
  REI_TEXTURE_TEXEL_FORMAT_RGB  = 0x1907,
  REI_TEXTURE_TEXEL_FORMAT_RGBA = 0x1908,
} ReiTextureTexelFormat;

typedef enum ReiTextureTexelType {
  REI_TEXTURE_TEXEL_TYPE_SBYTE  = 0x1400,
  REI_TEXTURE_TEXEL_TYPE_UBYTE  = 0x1401,
  REI_TEXTURE_TEXEL_TYPE_SSHORT = 0x1402,
  REI_TEXTURE_TEXEL_TYPE_USHORT = 0x1403,
  REI_TEXTURE_TEXEL_TYPE_SINT   = 0x1404,
  REI_TEXTURE_TEXEL_TYPE_UINT   = 0x1405,
  REI_TEXTURE_TEXEL_TYPE_FLOAT  = 0x1406,
} ReiTextureTexelType;

void reiTextureDefineAndCopyFromCpu(ReiContext * context, ReiTextureBinding binding, int bindingLevel, ReiTextureTexelFormat bindingTexelFormat, int width, int height, ReiTextureTexelFormat texelsFormat, ReiTextureTexelType texelsType, int texelsBytesAlignment, const void * texels);
void reiTextureDefineAndCopyFromBackbuffer(ReiContext * context, ReiTextureBinding binding, int bindingLevel, ReiTextureTexelFormat bindingTexelFormat, int backbufferX, int backbufferY, int width, int height);

void reiTextureCopyFromCpu(ReiContext * context, ReiTextureBinding binding, int bindingLevel, int bindingX, int bindingY, int width, int height, ReiTextureTexelFormat texelsFormat, ReiTextureTexelType texelsType, int texelsBytesAlignment, const void * texels);
void reiTextureCopyFromBackbuffer(ReiContext * context, ReiTextureBinding binding, int bindingLevel, int bindingX, int bindingY, int backbufferX, int backbufferY, int width, int height);

void reiReadbackToCpuFromBackbuffer(ReiContext * context, int backbufferX, int backbufferY, int width, int height, ReiTextureTexelFormat texelsFormat, ReiTextureTexelType texelsType, int texelsBytesAlignment, void * outTexels);

// Command list creation and destruction

ReiHandleCommandList reiCreateCommandList(ReiContext * context);
void reiDestroyCommandList(ReiContext * context, ReiHandleCommandList list);

// Command list recording

void reiCommandListSet(ReiContext * context, ReiHandleCommandList list);
void reiCommandListEnd(ReiContext * context);

// Command recording viewport and scissor size

void reiCommandSetViewport(ReiContext * context, int x, int y, int width, int height);
void reiCommandSetScissor(ReiContext * context, int x, int y, int width, int height);

// Command recording of backbuffer(s) clearing

typedef unsigned ReiClearFlags;
typedef enum ReiClearFlagBits {
  REI_CLEAR_DEPTH_BIT   = 0x00000100,
  REI_CLEAR_STENCIL_BIT = 0x00000400,
  REI_CLEAR_COLOR_BIT   = 0x00004000,
} ReiClearFlagBits;

void reiCommandClear(ReiContext * context, ReiClearFlags clear, float depthValue, unsigned stencilValue, float colorR, float colorG, float colorB, float colorA);

// Command recording of mesh state and mesh work domain execution

typedef enum ReiCullMode {
  REI_CULL_MODE_NONE  = 0,
  REI_CULL_MODE_FRONT = 0x0404,
  REI_CULL_MODE_BACK  = 0x0405,
} ReiCullMode;

typedef enum ReiFrontFace {
  REI_FRONT_FACE_COUNTER_CLOCKWISE = 0x0901,
  REI_FRONT_FACE_CLOCKWISE         = 0x0900,
} ReiFrontFace;

typedef enum ReiCompareOp {
  REI_COMPARE_OP_NEVER            = 0x0200,
  REI_COMPARE_OP_LESS             = 0x0201,
  REI_COMPARE_OP_EQUAL            = 0x0202,
  REI_COMPARE_OP_LESS_OR_EQUAL    = 0x0203,
  REI_COMPARE_OP_GREATER          = 0x0204,
  REI_COMPARE_OP_NOT_EQUAL        = 0x0205,
  REI_COMPARE_OP_GREATER_OR_EQUAL = 0x0206,
  REI_COMPARE_OP_ALWAYS           = 0x0207,
} ReiCompareOp;

typedef enum ReiStencilOp {
  REI_STENCIL_OP_KEEP                = 0x1E00,
  REI_STENCIL_OP_ZERO                = 0,
  REI_STENCIL_OP_REPLACE             = 0x1E01,
  REI_STENCIL_OP_INCREMENT_AND_CLAMP = 0x1E02,
  REI_STENCIL_OP_DECREMENT_AND_CLAMP = 0x1E03,
  REI_STENCIL_OP_INVERT              = 0x150A,
} ReiStencilOp;

typedef enum ReiLogicOp {
  REI_LOGIC_OP_CLEAR         = 0x1500,
  REI_LOGIC_OP_AND           = 0x1501,
  REI_LOGIC_OP_AND_REVERSE   = 0x1502,
  REI_LOGIC_OP_COPY          = 0x1503,
  REI_LOGIC_OP_AND_INVERTED  = 0x1504,
  REI_LOGIC_OP_NO_OP         = 0x1505,
  REI_LOGIC_OP_XOR           = 0x1506,
  REI_LOGIC_OP_OR            = 0x1507,
  REI_LOGIC_OP_NOR           = 0x1508,
  REI_LOGIC_OP_EQUIVALENT    = 0x1509,
  REI_LOGIC_OP_INVERT        = 0x150A,
  REI_LOGIC_OP_OR_REVERSE    = 0x150B,
  REI_LOGIC_OP_COPY_INVERTED = 0x150C,
  REI_LOGIC_OP_OR_INVERTED   = 0x150D,
  REI_LOGIC_OP_NAND          = 0x150E,
  REI_LOGIC_OP_SET           = 0x150F,
} ReiLogicOp;

typedef enum ReiBlendFactor {
  REI_BLEND_FACTOR_ZERO                     = 0,
  REI_BLEND_FACTOR_ONE                      = 1,
  REI_BLEND_FACTOR_SOURCE_COLOR             = 0x0300,
  REI_BLEND_FACTOR_ONE_MINUS_SOURCE_COLOR   = 0x0301,
  REI_BLEND_FACTOR_TARGET_COLOR             = 0x0306,
  REI_BLEND_FACTOR_ONE_MINUS_TARGET_COLOR   = 0x0307,
  REI_BLEND_FACTOR_SOURCE_ALPHA             = 0x0302,
  REI_BLEND_FACTOR_ONE_MINUS_SOURCE_ALPHA   = 0x0303,
  REI_BLEND_FACTOR_TARGET_ALPHA             = 0x0304,
  REI_BLEND_FACTOR_ONE_MINUS_TARGET_ALPHA   = 0x0305,
  REI_BLEND_FACTOR_CONSTANT_COLOR           = 0x8001,
  REI_BLEND_FACTOR_ONE_MINUS_CONSTANT_COLOR = 0x8002,
  REI_BLEND_FACTOR_SOURCE_ALPHA_SATURATE    = 0x0308,
} ReiBlendFactor;

typedef enum ReiBlendOp {
  REI_BLEND_OP_ADD              = 0x8006,
  REI_BLEND_OP_SUBTRACT         = 0x800A,
  REI_BLEND_OP_REVERSE_SUBTRACT = 0x800B,
  REI_BLEND_OP_MIN              = 0x8007,
  REI_BLEND_OP_MAX              = 0x8008,
} ReiBlendOp;

typedef struct ReiMeshState {
  ReiHandleProgram programVertex;
  ReiHandleProgram programFragment;
  ReiBool32        rasterizationDepthClampEnable;
  ReiCullMode      rasterizationCullMode;
  ReiFrontFace     rasterizationFrontFace;
  ReiBool32        rasterizationDepthBiasEnable;
  float            rasterizationDepthBiasConstantFactor;
  float            rasterizationDepthBiasSlopeFactor;
  ReiBool32        multisampleEnable;
  ReiBool32        multisampleAlphaToCoverageEnable;
  ReiBool32        multisampleAlphaToOneEnable;
  ReiBool32        depthTestEnable;
  ReiBool32        depthTestDepthWriteEnable;
  ReiCompareOp     depthTestDepthCompareOp;
  ReiBool32        stencilTestEnable;
  ReiStencilOp     stencilTestFrontStencilTestFailOp;
  ReiStencilOp     stencilTestFrontStencilTestPassDepthTestPassOp;
  ReiStencilOp     stencilTestFrontStencilTestPassDepthTestFailOp;
  ReiCompareOp     stencilTestFrontCompareOp;
  ReiStencilOp     stencilTestBackStencilTestFailOp;
  ReiStencilOp     stencilTestBackStencilTestPassDepthTestPassOp;
  ReiStencilOp     stencilTestBackStencilTestPassDepthTestFailOp;
  ReiCompareOp     stencilTestBackCompareOp;
  unsigned         stencilTestFrontAndBackCompareMask;
  unsigned         stencilTestFrontAndBackWriteMask;
  unsigned         stencilTestFrontAndBackReference;
  ReiBool32        blendLogicOpEnable;
  ReiLogicOp       blendLogicOp;
  float            blendConstants[4];
  ReiBool32        outputColorWriteEnableR;
  ReiBool32        outputColorWriteEnableG;
  ReiBool32        outputColorWriteEnableB;
  ReiBool32        outputColorWriteEnableA;
  ReiBool32        outputColorBlendEnable;
  ReiBlendFactor   outputColorBlendColorFactorSource;
  ReiBlendFactor   outputColorBlendColorFactorTarget;
  ReiBlendOp       outputColorBlendColorOp;
  ReiBlendFactor   outputColorBlendAlphaFactorSource;
  ReiBlendFactor   outputColorBlendAlphaFactorTarget;
  ReiBlendOp       outputColorBlendAlphaOp;
} ReiMeshState;

void reiCommandMeshSetState(ReiContext * context, ReiMeshState * state);
void reiCommandMeshSet(ReiContext * context);
void reiCommandMeshEnd(ReiContext * context);
void reiCommandMeshTexcoord(ReiContext * context, unsigned index, float x, float y, float z, float w);
void reiCommandMeshColor(ReiContext * context, float r, float g, float b, float a);
void reiCommandMeshNormal(ReiContext * context, float x, float y, float z);
void reiCommandMeshPosition(ReiContext * context, float x, float y, float z, float w);

// Command list submission and dynamic state setting

void reiSetProgramEnvironmentValue(ReiContext * context, ReiProgramBinding binding, unsigned index, float x, float y, float z, float w);
void reiSetProgramLocalValue(ReiContext * context, ReiProgramBinding binding, unsigned index, float x, float y, float z, float w);
void reiSubmitCommandLists(ReiContext * context, unsigned commandListsCount, ReiHandleCommandList * commandLists);

// Error handling

typedef enum ReiStatus {
  REI_STATUS_SUCCESS                 = 0,
  REI_STATUS_ERROR_INVALID_ENUM      = 0x0500,
  REI_STATUS_ERROR_INVALID_VALUE     = 0x0501,
  REI_STATUS_ERROR_INVALID_OPERATION = 0x0502,
  REI_STATUS_ERROR_STACK_OVERFLOW    = 0x0503,
  REI_STATUS_ERROR_STACK_UNDERFLOW   = 0x0504,
  REI_STATUS_ERROR_OUT_OF_MEMORY     = 0x0505,
} ReiStatus;

ReiStatus    reiGetStatus(ReiContext * context);
const char * reiGetProgramStatusString(ReiContext * context);
int          reiGetProgramStatusCodePosition(ReiContext * context);

// Misc

void reiFlush(ReiContext * context);
void reiFinish(ReiContext * context);
void reiCheckCode(ReiContext * context, const char * codeVertex, const char * codeFragment);

#ifdef __cplusplus
}
#endif
