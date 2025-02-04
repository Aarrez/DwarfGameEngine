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

        void ProcessMessages(EntityMessage message);

        static void CreateEntity(const SerializedFile& file, const Texture &texture, const string &name = "Entity");
        static std::vector<Entity*>& GetEntityList();
        static void RemoveEntityByName(const string& name);
        static void RemoveAllEntities();

    private:
        static EntityManager* Instance;
        EntityManager() = default;
        static vector<Entity*> entities;
        MessageQueue<EntityMessage> messageQueue;

    };
}
#endif //EntityManager_h