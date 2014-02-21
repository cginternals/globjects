#include <glow/constants.h>

#include <algorithm>

#include <glow/gl_constants.h>

namespace {

std::vector<std::string> extensions = {
    "ARB",
    "EXT",
    "NV",
    "AMD",
    "ATI",
    "KHR",
    "SGIS",
    "SGIX",
    "IBM",
    "INGR",
    "APPLE",
    "PGI",
    "QCOM",
    "OES",
    "INTEL",
    "IMG",
    "WEBGL"
};

int extensionPriority(const std::string & name)
{
    std::size_t pos = name.find_last_of('_');
    if (pos == std::string::npos)
        return -1;

    std::string extension = name.substr(pos+1);
    auto it = std::find(extensions.begin(), extensions.end(), extension);
    if (it == extensions.end())
        return -1;

    return static_cast<int>(it - extensions.begin());
}

void sortByExtension(std::vector<std::string> & names)
{
    std::sort(names.begin(), names.end(), [](const std::string& name1, const std::string& name2) {

		int priority1 = extensionPriority(name1);
		int priority2 = extensionPriority(name2);

        if (priority1 == priority2)
            return name1.size() < name2.size();

        return priority1 < priority2;
    });
}

}

namespace glow {

std::vector<std::string> enumNames(GLenum param)
{
    auto range = GLconstants.equal_range(static_cast<GLuint64>(param));

    std::vector<std::string> names;
    for (auto it = range.first; it != range.second; ++it)
    {
        names.push_back(it->second);
    }

    sortByExtension(names);

    return names;
}

std::string enumName(GLenum param)
{
    std::vector<std::string> names = enumNames(param);
    if (names.empty())
    {
        return "UNKNOWN_GL_ENUM";
    }

    return names[0];
}

} // namespace glow
