
#pragma once

#include <set>

#include <glbinding/gl/types.h>

#include <globjects/globjects_api.h>

#include <globjects/base/ref_ptr.h>
#include <globjects/base/ChangeListener.h>
#include <globjects/Object.h>


namespace globjects
{


class Program;

class GLOBJECTS_API ProgramPipeline : public Object, protected ChangeListener
{
public:
    ProgramPipeline();
    virtual ~ProgramPipeline();

    virtual void accept(ObjectVisitor & visitor) override;

    void use() const;
    static void release();

    void useStages(Program * program, gl::UseProgramStageMask stages);
    void releaseStages(gl::UseProgramStageMask stages);
    void releaseProgram(Program * program);

    bool isValid() const;
    void validate() const;

    bool checkUseStatus() const;

    std::string infoLog() const;
    gl::GLint get(gl::GLenum pname) const;

    virtual void notifyChanged(const Changeable * sender) override;

    virtual gl::GLenum objectType() const override;

protected:
    void invalidate();

protected:
    bool m_dirty;
    std::set<ref_ptr<Program>> m_programs;
};


} // namespace globjects
