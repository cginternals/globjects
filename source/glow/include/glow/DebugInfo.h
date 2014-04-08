#pragma once

#include <string>
#include <vector>
#include <map>

#include <GL/glew.h>

#include <glow/glow.h>
#include <glow/ObjectVisitor.h>


namespace glow
{

/** \brief Collects various debug information of all current registered glow Objects.
    
    To collect information about all glow Objects, this class is a subclass of
    ObjectVisitor. The debug info can be queried using generalInfo() and 
    objectInfo() or be printed to the command line using printObjectInfo(), 
    printGeneralInfo() or printAll().
    
    \code{.cpp}

        DebugInfo::printAll();

    \endcode
 */
class GLOW_API DebugInfo : public ObjectVisitor
{
public:
	struct Property
	{
		std::string name;
		std::string value;
	};
	struct InfoUnit
	{
		std::string name;
		std::vector<Property> properties;

		void addProperty(const std::string & name, const std::string & value);
		void addProperty(const std::string & name, GLint value);
	};
	struct InfoGroup
	{
		std::string name;
		std::vector<InfoUnit> units;

		void addInfoUnit(const InfoUnit & info);
	};

public:
	static std::vector<InfoGroup> generalInfo();
	static std::vector<InfoGroup> objectInfo();

	static void printObjectInfo();
	static void printGeneralInfo();
	static void printAll();

	static void print(const std::vector<InfoGroup> & info);

public:
    virtual void visitBuffer(Buffer * buffer) override;
    virtual void visitFrameBufferObject(FrameBufferObject * fbo) override;
    virtual void visitProgram(Program * program) override;
    virtual void visitRenderBufferObject(RenderBufferObject * rbo) override;
    virtual void visitSampler(Sampler * sampler) override;
    virtual void visitShader(Shader * shader) override;
    virtual void visitTexture(Texture * texture) override;
    virtual void visitTransformFeedback(TransformFeedback * transformfeedback) override;
    virtual void visitVertexArrayObject(VertexArrayObject * vao) override;

protected:
	DebugInfo();
	virtual ~DebugInfo();

    std::vector<InfoGroup> collectObjectInfo();

    InfoGroup & group(const std::string & name);
    void addInfo(const std::string & groupName, const InfoUnit & unit);

    static std::string name(const std::string & typeName, const Object * object);
    static std::string humanReadableSize(long long bytes);

protected:
	std::map<std::string, InfoGroup> m_infoGroups;
	std::map<std::string, unsigned> m_memoryUsage;
};

} // namespace glow
