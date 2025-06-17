#include "ComponentFactory.hpp"

namespace ECS {

    size_t ComponentFactory::GetSize(ComponentID id)
    {
        return idToSize.at(id);
    }

    const std::string& ComponentFactory::GetTypeName(ComponentID id)
    {
        return idToName.at(id);
    }

    ComponentID ComponentFactory::GetId(const std::string& typeName)
    {
        return nameToID.at(typeName);
    }
}
