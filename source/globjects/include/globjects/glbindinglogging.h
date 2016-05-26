
#pragma once

#include <globjects/globjects_api.h>

#include <globjects/base/LogMessageBuilder.h>

#include <glbinding/gl/types.h>


namespace glbinding
{


class Version;


} // namespace glbinding


namespace globjects 
{


GLOBJECTS_API LogMessageBuilder operator<<(LogMessageBuilder builder, const glbinding::Version & version);
GLOBJECTS_API LogMessageBuilder operator<<(LogMessageBuilder builder, const gl::GLboolean & value);
GLOBJECTS_API LogMessageBuilder operator<<(LogMessageBuilder builder, const gl::GLenum & value);
GLOBJECTS_API LogMessageBuilder operator<<(LogMessageBuilder builder, const gl::GLextension & value);
GLOBJECTS_API LogMessageBuilder operator<<(LogMessageBuilder builder, const gl::AttribMask & value);
GLOBJECTS_API LogMessageBuilder operator<<(LogMessageBuilder builder, const gl::ClearBufferMask & value);
GLOBJECTS_API LogMessageBuilder operator<<(LogMessageBuilder builder, const gl::ClientAttribMask & value);
GLOBJECTS_API LogMessageBuilder operator<<(LogMessageBuilder builder, const gl::ContextFlagMask & value);
GLOBJECTS_API LogMessageBuilder operator<<(LogMessageBuilder builder, const gl::ContextProfileMask & value);
GLOBJECTS_API LogMessageBuilder operator<<(LogMessageBuilder builder, const gl::FfdMaskSGIX & value);
GLOBJECTS_API LogMessageBuilder operator<<(LogMessageBuilder builder, const gl::FragmentShaderColorModMaskATI & value);
GLOBJECTS_API LogMessageBuilder operator<<(LogMessageBuilder builder, const gl::FragmentShaderDestMaskATI & value);
GLOBJECTS_API LogMessageBuilder operator<<(LogMessageBuilder builder, const gl::FragmentShaderDestModMaskATI & value);
GLOBJECTS_API LogMessageBuilder operator<<(LogMessageBuilder builder, const gl::MapBufferUsageMask & value);
GLOBJECTS_API LogMessageBuilder operator<<(LogMessageBuilder builder, const gl::MemoryBarrierMask & value);
GLOBJECTS_API LogMessageBuilder operator<<(LogMessageBuilder builder, const gl::PathRenderingMaskNV & value);
GLOBJECTS_API LogMessageBuilder operator<<(LogMessageBuilder builder, const gl::PerformanceQueryCapsMaskINTEL & value);
GLOBJECTS_API LogMessageBuilder operator<<(LogMessageBuilder builder, const gl::SyncObjectMask & value);
GLOBJECTS_API LogMessageBuilder operator<<(LogMessageBuilder builder, const gl::TextureStorageMaskAMD & value);
GLOBJECTS_API LogMessageBuilder operator<<(LogMessageBuilder builder, const gl::UseProgramStageMask & value);
GLOBJECTS_API LogMessageBuilder operator<<(LogMessageBuilder builder, const gl::VertexHintsMaskPGI & value);
GLOBJECTS_API LogMessageBuilder operator<<(LogMessageBuilder builder, const gl::PathFontStyle & value);
GLOBJECTS_API LogMessageBuilder operator<<(LogMessageBuilder builder, const gl::UnusedMask & value);
GLOBJECTS_API LogMessageBuilder operator<<(LogMessageBuilder builder, const gl::BufferAccessMask & value);


} // namespace globjects
