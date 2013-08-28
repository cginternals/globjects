#include <glow/ObjectRegistry.h>
#include <glow/logging.h>

#include <sstream>
#include <vector>
#include <algorithm>
#include <iomanip>

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
	if (object->id() == 0) return;
	m_objects.insert(object);
}

void ObjectRegistry::deregisterObject(Object* object)
{
	m_objects.erase(object);
}

std::string ObjectRegistry::humanReadableSize(unsigned bytes) const
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

std::string ObjectRegistry::info() const
{
	IF_NDEBUG(return "This feature is only available in debug mode.");

	std::stringstream ss;

	std::unordered_map<std::string, std::vector<Object*>> sortedObjects;

	for (Object* object: m_objects)
	{
		sortedObjects[object->typeName()].push_back(object);
	}

	//std::sort(pair.second.begin(), pair.second.end(), [](Object* o1, Object* o2) { return o1->id() < o2->id(); });

	for (const std::pair<std::string, std::vector<Object*>>& pair: sortedObjects)
	{
		ss << pair.first << "s:" << std::endl;

		for (Object* object: pair.second)
		{
			ss << " - " << object->typeName() << " (" << object->id();
			if (!object->name().empty()) ss << ", " << object->name();
			ss << ")" << std::endl;
			for (const std::pair<std::string, unsigned>& pair: object->properties().properties())
			{
				std::string value = std::to_string(pair.second);
				if (pair.first == "_memory")
				{
					value = humanReadableSize(pair.second);
				}
				ss << "\t" << pair.first << ": " << value << std::endl;
			}
			for (const std::pair<std::string, std::string>& pair: object->properties().stringProperties())
			{
				ss << "\t" << pair.first << ": " << pair.second << std::endl;
			}
		}
	}

	return ss.str();
}

void ObjectRegistry::dumpInfo() const
{
	debug() << info();
}

} // namespace glow
