#pragma once

#include <string>
#include <set>

#include <glow/glow.h>

namespace glow {
    
    class IncludeProcessor {
        
    public:
        
        virtual ~IncludeProcessor();

        static std::string resolveIncludes(const std::string& source);
        
    protected:
        
        IncludeProcessor();
        
        std::set<std::string> m_includes;
        std::string process(const std::string& source);
    };
    
} // namespace glow
