#pragma once
#include "Types.hpp"
#include <unordered_map>
#include <typeindex>

namespace ECS {

    class ComponentFactory {
    public:
        template<typename T>
        static ComponentID RegisterComponent();

        template<typename T>
        static ComponentID GetComponentID();

        static size_t GetSize(ComponentID id);

        static const std::string& GetTypeName(ComponentID id);

        static ComponentID GetId(const std::string& typeName);

        template<typename T>
        static ComponentID GetId();

    private:
        static inline ComponentID nextID = 0;
        static inline std::unordered_map<std::type_index, ComponentID> typeToID;
        static inline std::unordered_map<ComponentID, size_t> idToSize;
        static inline std::unordered_map<ComponentID, std::string> idToName;
        static inline std::unordered_map<std::string, ComponentID> nameToID;
    };

    template<typename T>
    inline ComponentID ComponentFactory::RegisterComponent()
    {
        std::type_index typeIdx = std::type_index(typeid(T));
        if (typeToID.find(typeIdx) != typeToID.end()) 
        {
            return typeToID[typeIdx];
        }

        if (nextID >= MAX_COMPONENTS) 
        {
            throw std::runtime_error("Exceeded maximum number of components");
        }

        ComponentID id = nextID++;
        typeToID[typeIdx] = id;
        idToSize[id] = sizeof(T);
        idToName[id] = typeid(T).name();
        nameToID[idToName[id]] = id;

        return id;
    }

    template<typename T>
    inline ComponentID ComponentFactory::GetComponentID()
    {
        std::type_index typeIdx = std::type_index(typeid(T));
        auto it = typeToID.find(typeIdx);
        if (it == typeToID.end()) {
            throw std::runtime_error("Component not registered");
        }
        return it->second;
    }

    template<typename T>
    inline ComponentID ComponentFactory::GetId()
    {
        std::type_index typeIdx = std::type_index(typeid(T));
        auto it = typeToID.find(typeIdx);
        if (it == typeToID.end()) {
            throw std::runtime_error("Component not registered");
        }
        return it->second;
    }
}