#include <glow/ObjectRegistry.h>
#include <glow/logging.h>

#include <sstream>
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

std::string ObjectRegistry::name(Object* object) const
{
	std::stringstream ss;

	ss << object->typeName() << " (" << object->id();
	if (!object->name().empty()) ss << ", " << object->name();
	ss << ")";

	return ss.str();
}

std::vector<ObjectRegistry::ObjectInfoGroup> ObjectRegistry::infoGroups() const
{
	IF_NDEBUG(return std::vector<ObjectInfoGroup>();)

	std::unordered_map<std::string, std::vector<Object*>> groupedObjects;

	for (Object* object: m_objects)
	{
		groupedObjects[object->typeName()].push_back(object);
	}

	std::vector<ObjectInfoGroup> groups;

	for (std::pair<std::string, std::vector<Object*>> pair: groupedObjects)
	{
		std::vector<Object*> objects = pair.second;
		std::sort(objects.begin(), objects.end(), [](Object* o1, Object* o2) { return o1->id() < o2->id(); });

		ObjectInfoGroup group;
		group.name = pair.first + "s";

		for (Object* object: objects)
		{
			ObjectInfo info;
			info.name = name(object);

			for (const std::pair<std::string, unsigned>& pair: object->properties().properties())
			{
				std::string value = std::to_string(pair.second);
				if (pair.first == "_memory")
				{
					value = humanReadableSize(pair.second);
				}
				info.properties.push_back(KeyValuePair{pair.first, value});
			}

			for (const std::pair<std::string, std::string>& pair: object->properties().stringProperties())
			{
				info.properties.push_back(KeyValuePair{pair.first, pair.second});
			}

			group.objects.push_back(info);
		}

		groups.push_back(group);
	}

	return groups;
}

std::string ObjectRegistry::info() const
{
	IF_NDEBUG(return "This feature is only available in debug mode.";)

	std::stringstream ss;

	for (const ObjectInfoGroup& group: infoGroups())
	{
		ss << group.name << ":" << std::endl;
		for (const ObjectInfo& info: group.objects)
		{
			ss << "\t" << info.name << std::endl;
			for (const KeyValuePair& pair: info.properties)
			{
				ss << "\t\t" <<  pair.key << ": " << pair.value << std::endl;
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
