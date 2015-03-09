#ifndef _RESOURCE_HOLDER_HPP_
#define _RESOURCE_HOLDER_HPP_

#include <map>
#include <string>
#include <memory>
#include <stdexcept>
#include <cassert>

template <typename Resource, typename Identifier>
class ResourceManager
{
    public:
        void                        load(Identifier id, const std::string& filename);

        template <typename Parameter>
        void                        load(Identifier id, const std::string& filename, const Parameter& secondParam);

        Resource&                   get(Identifier id);
        const Resource&             get(Identifier id) const;

    private:
        void                        insertResource(Identifier id, std::unique_ptr<Resource> resource);

    private:
        std::map<Identifier, std::unique_ptr<Resource>> mResourceMap;
};

#include "ResourceManager.inl"
#endif // _RESOURCE_HOLDER_HPP_
