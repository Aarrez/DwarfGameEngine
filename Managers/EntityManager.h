#ifndef EntityManager_h
#define EntityManager_h
#include <string>
#include <glm/glm.hpp>

#include "../GL/VirtualObject.h"
#include "../Message/EntityMessage.h"
#include "../Message/MessageQueue.h"


namespace Engine {

    class EntityManager {
        public:
        static void Allocate();
        static EntityManager& Get();
        void ProcessMessages(const EntityMessage& message);
        std::vector<Entity*>& GetEntityList();

    private:
        Entity* CreateEntity(const SerializedFile& file,
            const std::string &name = "Entity");
        void RemoveEntityByName(const std::string& name);
        void RemoveAllEntities();
        static EntityManager* Instance;
        EntityManager() = default;
        static std::vector<Entity*> entities;
        MessageQueue<EntityMessage> messageQueue;

    };
}
#endif //EntityManager_h