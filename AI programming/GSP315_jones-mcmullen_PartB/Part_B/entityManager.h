#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

enum {
    PLAYER,
    CAR
};

class entityManager {
    public:
		~entityManager();
        void create_entity(int entityType); 
        void delete_entity(int entityID);

    private:
        std::vector<Entity*> entityVector;
        std::vector<Entity*> entityVectorIter;
};

#endif