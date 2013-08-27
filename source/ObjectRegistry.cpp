#include <glow/ObjectRegistry.h>
#include <glow/logging.h>

#include <sstream>

namespace glow {

ObjectRegistry::ObjectRegistry()
{
}

ObjectRegistry& ObjectRegistry::instance()
{
	static ObjectRegistry s_instance;
	return s_instance;
}

void ObjectRegistry::registerObject(Object* object)
{
	m_objects.insert(object);
}

void ObjectRegistry::deregisterObject(Object* object)
{
	m_objects.erase(object);
}

std::string ObjectRegistry::info() const
{
	IF_NDEBUG(return "This feature is only available in debug mode.");

	std::stringstream ss;

	for (Object* object: m_objects)
	{
		ss << object->typeName() << " (" << object->id();
		if (!object->name().empty()) ss << ", " << object->name();
		ss << ")" << std::endl;
		for (const std::pair<std::string, unsigned>& pair: object->properties().properties())
		{
			ss << "\t" << pair.first << ": " << pair.second << std::endl;
		}
	}
	return ss.str();
}

void ObjectRegistry::dumpInfo() const
{
	debug() << info();
}

} // namespace glow
