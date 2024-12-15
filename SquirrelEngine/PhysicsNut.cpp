#include "PCH.h"
#include "PhysicsNut.h"
#include "Services.h"
namespace SQ {
    
    PhysicsNut::PhysicsNut(bool isStatic)
    {
        this->isStatic = isStatic;
    }

    UniquePoolPtr<Nut> SQ::PhysicsNut::Deserialize(Nut* deserializeInto, nlohmann::json serializedData)
    {
        // Cast deserializeInto to our type, call it toWorkOn
        PhysicsNut* toWorkOn = dynamic_cast<PhysicsNut*>(deserializeInto);
        // If toWorkOn is nullptr, make a new nut of our type. 
        UniquePoolPtr<Nut> owner;
        if (deserializeInto == nullptr) {
            UniquePoolPtr<PhysicsNut> instance = Services::GetPoolAllocationService()->MakeUniquePoolPtr<PhysicsNut>();
            toWorkOn = instance.get();
            owner = instance.StaticUniquePoolPtrCast<Nut>();
            deserializeInto = owner.get();
        }
        // Call parent deserialise, passing in our toWorkOn.
        WorldNut::Deserialize(toWorkOn, serializedData);

        // Perform deserialization on our data. 
        if (!serializedData["static"].is_null()) toWorkOn->isStatic = serializedData["static"];
        else toWorkOn->isStatic = false;
        if (!serializedData["elasticity"].is_null()) toWorkOn->elasticity = serializedData["elasticity"];
        if (!serializedData["density"].is_null()) toWorkOn->density = serializedData["density"];
        if (!serializedData["collisionShape"].is_null()) toWorkOn->shape = Services::GetResourceManager()->Retrieve<CollisionShape>(serializedData["collisionShape"]);
       
        // Return toWorkOn
        return owner;
    }

    void PhysicsNut::Ready()
    {
        // Call base ready and register body with physics engine
        WorldNut::Ready();
        Services::GetPhysics()->RegisterBody(this);
    }

    void PhysicsNut::SetShape(std::shared_ptr<CollisionShape> shape)
    {
        this->shape = shape;
        Services::GetPhysics()->BodyShapeUpdated(this);
    }
    std::shared_ptr<CollisionShape> PhysicsNut::GetShape()
    {
        return shape;
    }
    float PhysicsNut::GetElasticity()
    {
        return elasticity;
    }
    float PhysicsNut::GetDensity()
    {
        return density;
    }
    bool PhysicsNut::IsStatic()
    {
        return isStatic;
    }
    PhysicsNut::~PhysicsNut()
    {
        Services::GetPhysics()->RemoveBody(this);
    }
}