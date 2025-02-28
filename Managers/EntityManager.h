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
            const string &name = "Entity");
        void ChangeTexture(Texture &texture);
        void RemoveEntityByName(const string& name);
        void RemoveAllEntities();
        static EntityManager* Instance;
        EntityManager() = default;
        static vector<Entity*> entities;
        MessageQueue<EntityMessage> messageQueue;

    };
}
#endif //EntityManager_h