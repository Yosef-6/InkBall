#pragma once
#include <map>
#include <string>
#include <memory>
#include <cassert>
#include <iostream>
template <typename Resource, typename Identifier>
class ResourceHolder
{
public:
    void load(Identifier id, const std::string& filename);
    Resource& get(Identifier id);
    const Resource& get(Identifier id) const;
    void insertResource(Identifier id, std::unique_ptr<Resource> resource);
    void deleteResource();
private:
    std::map<Identifier, std::unique_ptr<Resource>> mResourceMap;
};
#include "ResourceHolder.inl"