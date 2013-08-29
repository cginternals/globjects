#pragma once

#include <glow/ObjectVisitor.h>

#include <set>
#include <string>
#include <vector>
#include <map>

namespace glow
{

class DebugInfo : public ObjectVisitor
{
public:
	struct Property
	{
		std::string name;
		std::string value;
	};
	struct ObjectInfo
	{
		std::string name;
		std::vector<Property> properties;

		void addProperty(const std::string& name, const std::string& value);
		void addProperty(const std::string& name, GLint value);
	};
	struct ObjectInfoGroup
	{
		std::string name;
		std::vector<ObjectInfo> objects;

		void addObjectInfo(const ObjectInfo& info);
	};

	DebugInfo();
	virtual ~DebugInfo();

	static std::vector<ObjectInfoGroup> groups();
	static void dump();

	void collectInfo();
	std::vector<ObjectInfoGroup> getInfoGroups() const;
	void clear();

	virtual void visitBuffer(Buffer* buffer);
	virtual void visitFrameBufferObject(FrameBufferObject* fbo);
	virtual void visitProgram(Program* program);
	virtual void visitRenderBufferObject(RenderBufferObject* rbo);
	virtual void visitShader(Shader* shader);
	virtual void visitTexture(Texture* texture);
	virtual void visitTransformFeedback(TransformFeedback* transformfeedback);
	virtual void visitVertexArrayObject(VertexArrayObject* vao);
protected:
	std::map<std::string, ObjectInfoGroup> m_infoGroups;

	ObjectInfoGroup& group(const std::string& name);
	void addInfo(const std::string& groupName, const ObjectInfo& info);

	std::string name(const std::string& typeName, Object* object) const;
	std::string humanReadableSize(unsigned bytes) const;
};

} // namespace glow
