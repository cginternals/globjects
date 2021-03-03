
#pragma once


#include <set>

#include <glbinding/gl/types.h>

#include <globjects/globjects_api.h>

#include <globjects/Object.h>
#include <globjects/base/Instantiator.h>


namespace globjects
{


class Program;


class GLOBJECTS_API ProgramPipeline : public Object, public Instantiator<ProgramPipeline>
{
public:
    enum class Implementation
    {
        Legacy
    };


public:
    ProgramPipeline();
    virtual ~ProgramPipeline();

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

    virtual void notifyChanged(const Program * sender);

    virtual gl::GLenum objectType() const override;

    void addSubject(Program * subject);
    virtual void removeSubject(Program * subject);


protected:
    void invalidate();


protected:
    std::set<Program *> m_programSubjects;
    bool m_dirty;
    std::set<Program *> m_programs;
};


} // namespace globjects
