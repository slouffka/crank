#ifndef CRANK_RESOURCE_MANAGER_HPP
#define CRANK_RESOURCE_MANAGER_HPP

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
#endif // CRANK_RESOURCE_MANAGER_HPP
