
#include <globjects/ProgramPipeline.h>

#include <glbinding/gl/enum.h>
#include <glbinding/gl/boolean.h>
#include <glbinding/gl/functions.h>

#include <globjects/Program.h>
#include <globjects/ObjectVisitor.h>

#include "Resource.h"


namespace globjects
{


ProgramPipeline::ProgramPipeline()
: Object(new ProgramPipelineResource())
, m_dirty(true)
{
}

ProgramPipeline::~ProgramPipeline()
{
    if (0 == id())
    {
        for (auto & program : m_programs)
            program->deregisterListener(this);
    }
    else
    {
        for (ref_ptr<Program> program : std::set<ref_ptr<Program>>(m_programs))
            releaseProgram(program);
    }
}

void ProgramPipeline::accept(ObjectVisitor & visitor)
{
    visitor.visitProgramPipeline(this);
}

void ProgramPipeline::use() const
{
    if (m_dirty)
    {
        for (const Program * program : m_programs)
        {
            program->link();
        }

        const_cast<ProgramPipeline *>(this)->m_dirty = false;

        checkUseStatus();
    }

    gl::glUseProgram(0);
    gl::glBindProgramPipeline(id());
}

void ProgramPipeline::release()
{
    gl::glBindProgramPipeline(0);
}

void ProgramPipeline::useStages(Program * program, gl::UseProgramStageMask stages)
{
    program->setParameter(gl::GL_PROGRAM_SEPARABLE, gl::GL_TRUE);

    program->registerListener(this);
    m_programs.emplace(program);

    program->link();

    gl::glUseProgramStages(id(), stages, program->id());

    invalidate();
}

void ProgramPipeline::releaseStages(gl::UseProgramStageMask stages)
{
    gl::glUseProgramStages(id(), stages, 0);

    invalidate();
}

void ProgramPipeline::releaseProgram(Program * program)
{
    program->deregisterListener(this);
    m_programs.erase(program);

    invalidate();
}

void ProgramPipeline::notifyChanged(const Changeable * /*sender*/)
{
    invalidate();
}

bool ProgramPipeline::isValid() const
{
    return get(gl::GL_VALIDATE_STATUS) == 1;
}

void ProgramPipeline::validate() const
{
    gl::glValidateProgramPipeline(id());
}

void ProgramPipeline::invalidate()
{
    m_dirty = true;
}

bool ProgramPipeline::checkUseStatus() const
{
    validate();

    if (!isValid())
    {
        critical() << "Use error:" << std::endl << infoLog();

        return false;
    }

    return true;
}

gl::GLint ProgramPipeline::get(const gl::GLenum pname) const
{
    gl::GLint value = 0;
    gl::glGetProgramPipelineiv(id(), pname, &value);

    return value;
}

std::string ProgramPipeline::infoLog() const
{
    gl::GLint length = get(gl::GL_INFO_LOG_LENGTH);

    if (length == 0)
        return std::string();

    std::vector<char> log(length);

    gl::glGetProgramPipelineInfoLog(id(), length, &length, log.data());

    return std::string(log.data(), length);
}

gl::GLenum ProgramPipeline::objectType() const
{
    return gl::GL_PROGRAM_PIPELINE;
}


} // namespace globjects
