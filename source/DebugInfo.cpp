#include <glow/DebugInfo.h>
#include <glow/ObjectRegistry.h>
#include <glow/logging.h>
#include <glow/query.h>

#include <sstream>
#include <algorithm>
#include <iomanip>

namespace glow
{

DebugInfo::DebugInfo()
{
}

DebugInfo::~DebugInfo()
{
}

std::vector<DebugInfo::ObjectInfoGroup> DebugInfo::groups()
{
	DebugInfo debugInfo;
	debugInfo.collectInfo();
	return debugInfo.getInfoGroups();
}

void DebugInfo::dump()
{
	for (const ObjectInfoGroup& group: groups())
	{
		glow::debug() << group.name;

		for (const ObjectInfo& info: group.objects)
		{
			glow::debug() << "\t" << info.name;

			for (const Property& property: info.properties)
			{
				glow::debug() << "\t\t" << property.name << ": " << property.value;
			}
		}
	}
}

void DebugInfo::collectInfo()
{
	clear();

	for (Object* object: ObjectRegistry::objects())
	{
		visit(object);
	}
}

std::vector<DebugInfo::ObjectInfoGroup> DebugInfo::getInfoGroups() const
{
	std::vector<ObjectInfoGroup> groups;

	for (const std::pair<std::string, ObjectInfoGroup>& pair: m_infoGroups)
	{
		groups.push_back(pair.second);
	}

	return groups;
}

void DebugInfo::clear()
{
	m_infoGroups.clear();
}

void DebugInfo::visitBuffer(Buffer* buffer)
{
	ObjectInfo info;
	info.name = name("Buffer", buffer);

	info.addProperty("size", humanReadableSize(buffer->getParameter(GL_BUFFER_SIZE)));

	addInfo("Buffers", info);
}

void DebugInfo::visitFrameBufferObject(FrameBufferObject* fbo)
{
	ObjectInfo info;
	info.name = name("FrameBufferObject", fbo);

	addInfo("FrameBufferObjects", info);
}

void DebugInfo::visitProgram(Program* program)
{
	ObjectInfo info;
	info.name = name("Program", program);

	info.addProperty("size", humanReadableSize(program->get(GL_PROGRAM_BINARY_LENGTH)));

	addInfo("Programs", info);
}

void DebugInfo::visitRenderBufferObject(RenderBufferObject* rbo)
{
	ObjectInfo info;
	info.name = name("RenderBufferObject", rbo);

	int w = rbo->getParameter(GL_RENDERBUFFER_WIDTH);
	int h = rbo->getParameter(GL_RENDERBUFFER_HEIGHT);

	int r = rbo->getParameter(GL_RENDERBUFFER_RED_SIZE);
	int g = rbo->getParameter(GL_RENDERBUFFER_GREEN_SIZE);
	int b = rbo->getParameter(GL_RENDERBUFFER_BLUE_SIZE);
	int a = rbo->getParameter(GL_RENDERBUFFER_ALPHA_SIZE);

	int d = rbo->getParameter(GL_RENDERBUFFER_DEPTH_SIZE);
	int s = rbo->getParameter(GL_RENDERBUFFER_STENCIL_SIZE);

	unsigned int size = (unsigned int)std::ceil((w*h*(r+g+b+a+d+s))/8.0);

	info.addProperty("size", humanReadableSize(size));
	info.addProperty("width", w);
	info.addProperty("height", h);

	addInfo("RenderBufferObjects", info);
}

void DebugInfo::visitShader(Shader* shader)
{
	ObjectInfo info;
	info.name = name("Shader", shader);

	info.addProperty("type", shader->typeString());
	if (shader->source() && shader->source()->isFile())
	{
		info.addProperty("source", dynamic_cast<const ShaderFile*>(shader->source())->filePath());
	}

	addInfo("Shaders", info);
}

void DebugInfo::visitTexture(Texture* texture)
{
	ObjectInfo info;
	info.name = name("Texture", texture);

	int maxTextureSize = query::getInteger(GL_MAX_TEXTURE_SIZE);
	int maxLevels = (int)std::ceil(std::log(maxTextureSize)/std::log(2));

	unsigned int size = 0;
	for (int i = 0; i<=maxLevels; ++i)
	{
		int imageSize = 0;

		if (texture->getLevelParameter(i, GL_TEXTURE_COMPRESSED) == GL_TRUE)
		{
			size += texture->getLevelParameter(i, GL_TEXTURE_COMPRESSED_IMAGE_SIZE);
		}
		else
		{
			int w = texture->getLevelParameter(i, GL_TEXTURE_WIDTH);
			int h = texture->getLevelParameter(i, GL_TEXTURE_HEIGHT);
			int d = texture->getLevelParameter(i, GL_TEXTURE_DEPTH);

			int r = texture->getLevelParameter(i, GL_TEXTURE_RED_SIZE);
			int g = texture->getLevelParameter(i, GL_TEXTURE_GREEN_SIZE);
			int b = texture->getLevelParameter(i, GL_TEXTURE_BLUE_SIZE);
			int a = texture->getLevelParameter(i, GL_TEXTURE_ALPHA_SIZE);
			int ds = texture->getLevelParameter(i, GL_TEXTURE_DEPTH_SIZE);

			size += (int)std::ceil((w*h*d*(r+g+b+a+ds))/8.0);
		}
	}

	info.addProperty("size", humanReadableSize(size));

	addInfo("Textures", info);
}

void DebugInfo::visitTransformFeedback(TransformFeedback* transformfeedback)
{
	ObjectInfo info;
	info.name = name("TransformFeedback", transformfeedback);

	addInfo("TransformFeedbacks", info);
}

void DebugInfo::visitVertexArrayObject(VertexArrayObject* vao)
{
	ObjectInfo info;
	info.name = name("VertexArrayObject", vao);

	addInfo("VertexArrayObjects", info);
}



DebugInfo::ObjectInfoGroup& DebugInfo::group(const std::string& name)
{
	if (m_infoGroups.find(name) == m_infoGroups.end())
	{
		ObjectInfoGroup group;
		group.name = name;
		m_infoGroups[name] = group;
	}

	return m_infoGroups[name];
}

void DebugInfo::addInfo(const std::string& groupName, const ObjectInfo& info)
{
	group(groupName).addObjectInfo(info);
}

std::string DebugInfo::humanReadableSize(unsigned bytes) const
{
	static std::vector<char> prefix = { 'k', 'M', 'G', 'T' };
	int power = -1;
	float value = static_cast<float>(bytes);

	std::stringstream ss;

	while (value>=1024.0 && power<static_cast<int>(prefix.size()-1))
	{
		value /= 1024.0;
		power++;
	}

	ss << std::setprecision(3) << value;
	if (power>=0)
	{
		ss << prefix[power];
	}

	ss << "b";

	return ss.str();
}

std::string DebugInfo::name(const std::string& typeName, Object* object) const
{
	std::stringstream ss;

	ss << typeName << " (" << object->id();
	if (!object->name().empty()) ss << ", " << object->name();
	ss << ")";

	return ss.str();
}

void DebugInfo::ObjectInfo::addProperty(const std::string& name, const std::string& value)
{
	properties.push_back({name, value});
}

void DebugInfo::ObjectInfo::addProperty(const std::string& name, GLint value)
{
	addProperty(name, std::to_string(value));
}

void DebugInfo::ObjectInfoGroup::addObjectInfo(const ObjectInfo& info)
{
	objects.push_back(info);
}


} // namespace glow
