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

void DebugInfo::printObjectInfo()
{
	print(objectInfo());
}

void DebugInfo::printGeneralInfo()
{
	print(generalInfo());
}

void DebugInfo::printAll()
{
	printGeneralInfo();
	printObjectInfo();
}

void DebugInfo::print(const std::vector<InfoGroup>& info)
{
	for (const InfoGroup& group: info)
	{
		glow::debug() << group.name;

		for (const InfoUnit& unit: group.units)
		{
			glow::debug() << "\t" << unit.name;

			for (const Property& property: unit.properties)
			{
				glow::debug() << "\t\t" << property.name << ": " << property.value;
			}
		}
	}
}

std::vector<DebugInfo::InfoGroup> DebugInfo::generalInfo()
{
	InfoGroup generalGroup;

	InfoUnit generalInfo;
	InfoUnit memoryInfo;
	InfoUnit textureInfo;

	generalGroup.name = "General";

	generalInfo.name = "OpenGL";
	memoryInfo.name = "Memory";
	textureInfo.name = "General Texture Info";

	generalInfo.addProperty("version", query::version().toString());
	generalInfo.addProperty("vendor", query::vendor());
	generalInfo.addProperty("renderer", query::renderer());
	generalInfo.addProperty("core profile", query::isCoreProfile()?"true":"false");
	generalInfo.addProperty("GLSL version", query::getString(GL_SHADING_LANGUAGE_VERSION));

	memoryInfo.addProperty("total", humanReadableSize(1024ll*memory::total()));
	memoryInfo.addProperty("dedicated", humanReadableSize(1024ll*memory::dedicated()));
	memoryInfo.addProperty("available", humanReadableSize(1024ll*memory::available()));
	memoryInfo.addProperty("evicted", humanReadableSize(1024ll*memory::evicted()));
	memoryInfo.addProperty("evictionCount", memory::evictionCount());

	int maxTextureSize = query::getInteger(GL_MAX_TEXTURE_SIZE);
	textureInfo.addProperty("Max Texture Size", std::to_string(maxTextureSize)+" x "+std::to_string(maxTextureSize));
	textureInfo.addProperty("Max Vertex Texture Image Units", query::getInteger(GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS));
	textureInfo.addProperty("Max Texture Image Units", query::getInteger(GL_MAX_IMAGE_UNITS));
	textureInfo.addProperty("Max Geometry Texture Units", query::getInteger(GL_MAX_GEOMETRY_TEXTURE_IMAGE_UNITS));
	std::vector<int> maxViewportSize = query::getIntegers(GL_MAX_VIEWPORT_DIMS, 2);
	textureInfo.addProperty("Max viewport size", std::to_string(maxViewportSize[0])+" x "+std::to_string(maxViewportSize[1]));
	textureInfo.addProperty("Max clip distances", query::getInteger(GL_MAX_CLIP_DISTANCES));
	textureInfo.addProperty("Max Samples", query::getInteger(GL_MAX_SAMPLES));

	generalGroup.addInfoUnit(generalInfo);
	generalGroup.addInfoUnit(memoryInfo);
	generalGroup.addInfoUnit(textureInfo);

	return std::vector<InfoGroup>({ generalGroup });
}

std::vector<DebugInfo::InfoGroup> DebugInfo::objectInfo()
{
	DebugInfo debugInfo;
	return debugInfo.collectObjectInfo();
}

//==========================0

std::vector<DebugInfo::InfoGroup> DebugInfo::collectObjectInfo()
{
	m_infoGroups.clear();

	for (Object* object: ObjectRegistry::objects())
	{
		visit(object);
	}

	std::vector<InfoGroup> groups;

	for (const std::pair<std::string, InfoGroup>& pair: m_infoGroups)
	{
		groups.push_back(pair.second);
	}

	return groups;
}

void DebugInfo::visitBuffer(Buffer* buffer)
{
	InfoUnit info;
	info.name = name("Buffer", buffer);

	info.addProperty("size", humanReadableSize(buffer->getParameter(GL_BUFFER_SIZE)));

	addInfo("Buffers", info);
}

void DebugInfo::visitFrameBufferObject(FrameBufferObject* fbo)
{
	InfoUnit info;
	info.name = name("FrameBufferObject", fbo);

	addInfo("FrameBufferObjects", info);
}

void DebugInfo::visitProgram(Program* program)
{
	InfoUnit info;
	info.name = name("Program", program);

	info.addProperty("size", humanReadableSize(program->get(GL_PROGRAM_BINARY_LENGTH)));

	addInfo("Programs", info);
}

void DebugInfo::visitRenderBufferObject(RenderBufferObject* rbo)
{
	InfoUnit info;
	info.name = name("RenderBufferObject", rbo);

	int w = rbo->getParameter(GL_RENDERBUFFER_WIDTH);
	int h = rbo->getParameter(GL_RENDERBUFFER_HEIGHT);

	int r = rbo->getParameter(GL_RENDERBUFFER_RED_SIZE);
	int g = rbo->getParameter(GL_RENDERBUFFER_GREEN_SIZE);
	int b = rbo->getParameter(GL_RENDERBUFFER_BLUE_SIZE);
	int a = rbo->getParameter(GL_RENDERBUFFER_ALPHA_SIZE);

	int d = rbo->getParameter(GL_RENDERBUFFER_DEPTH_SIZE);
	int s = rbo->getParameter(GL_RENDERBUFFER_STENCIL_SIZE);

	int size = (int)std::ceil((w*h*(r+g+b+a+d+s))/8.0);

	info.addProperty("size", humanReadableSize(size));
	info.addProperty("width", w);
	info.addProperty("height", h);

	addInfo("RenderBufferObjects", info);
}

void DebugInfo::visitShader(Shader* shader)
{
	InfoUnit info;
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
	InfoUnit info;
	info.name = name("Texture", texture);

	int maxTextureSize = query::getInteger(GL_MAX_TEXTURE_SIZE);
	int maxLevels = (int)std::ceil(std::log(maxTextureSize)/std::log(2))+1;

	int size = 0;
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
	InfoUnit info;
	info.name = name("TransformFeedback", transformfeedback);

	addInfo("TransformFeedbacks", info);
}

void DebugInfo::visitVertexArrayObject(VertexArrayObject* vao)
{
	InfoUnit info;
	info.name = name("VertexArrayObject", vao);

	addInfo("VertexArrayObjects", info);
}



DebugInfo::InfoGroup& DebugInfo::group(const std::string& name)
{
	if (m_infoGroups.find(name) == m_infoGroups.end())
	{
		InfoGroup group;
		group.name = name;
		m_infoGroups[name] = group;
	}

	return m_infoGroups[name];
}

void DebugInfo::addInfo(const std::string& groupName, const InfoUnit& info)
{
	group(groupName).addInfoUnit(info);
}

std::string DebugInfo::humanReadableSize(long long bytes)
{
	if (bytes<0)
	{
		return "n/a";
	}

	static std::vector<char> prefix = { 'k', 'M', 'G', 'T' };
	int power = -1;
	double value = static_cast<double>(bytes);

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

std::string DebugInfo::name(const std::string& typeName, Object* object)
{
	std::stringstream ss;

	ss << typeName << " (" << object->id();
	if (!object->name().empty()) ss << ", " << object->name();
	ss << ")";

	return ss.str();
}

void DebugInfo::InfoUnit::addProperty(const std::string& name, const std::string& value)
{
	properties.push_back({name, value});
}

void DebugInfo::InfoUnit::addProperty(const std::string& name, GLint value)
{
	addProperty(name, std::to_string(value));
}

void DebugInfo::InfoGroup::addInfoUnit(const InfoUnit& unit)
{
	units.push_back(unit);
}


} // namespace glow
