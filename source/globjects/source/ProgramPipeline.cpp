
#include <globjects/ProgramPipeline.h>

#include <glbinding/gl/enum.h>
#include <glbinding/gl/boolean.h>
#include <glbinding/gl/functions.h>

#include <globjects/Program.h>
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

void ProgramPipeline::use() const
{
    if (m_dirty)
    {
        for (const Program * program : m_programs)
        {
            program->link();
        }

        const_cast<ProgramPipeline *>(this)->m_dirty = false;
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

void ProgramPipeline::notifyChanged(const Changeable * sender)
{
    invalidate();
}

void ProgramPipeline::invalidate()
{
    m_dirty = true;
}

gl::GLenum ProgramPipeline::objectType() const
{
    return gl::GL_PROGRAM_PIPELINE;
}


} // namespace globjects
