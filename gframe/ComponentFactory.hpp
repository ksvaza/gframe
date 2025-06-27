#pragma once
#include "Types.hpp"
#include <unordered_map>
#include <typeindex>

namespace ECS {

    class ComponentFactory {
    public:

        /// <summary>
        /// Registers component
        /// </summary>
        /// <typeparam name="T"></typeparam>
        /// <returns></returns>
        template<typename T>
        static ComponentID RegisterComponent();

        /// <summary>
        /// Returns ComponentID of component T
        /// </summary>
        /// <typeparam name="T"></typeparam>
        /// <returns></returns>
        template<typename T>
        static ComponentID GetComponentID();

        /// <summary>
        /// Returns size in bytes of component with given ID
        /// </summary>
        /// <param name="id"></param>
        /// <returns></returns>
        static size_t GetSize(ComponentID id);

        /// <summary>
        /// Returns Type Name of Component with ComponentID
        /// </summary>
        /// <param name="id"></param>
        /// <returns></returns>
        static const std::string& GetTypeName(ComponentID id);

        /// <summary>
        /// Returns ID of component ith typename
        /// </summary>
        /// <param name="typeName"></param>
        /// <returns></returns>
        static ComponentID GetId(const std::string& typeName);

        /// <summary>
        /// Returns ComponentID of type T
        /// </summary>
        /// <typeparam name="T"></typeparam>
        /// <returns></returns>
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