#pragma once
#include "WorldNut.h"
namespace SQ {
    class PhysicsNut :
        public WorldNut
    {
    public:
        static UniquePoolPtr<Nut> Deserialize(Nut* deserializeInto, nlohmann::json serializedData);

    };

}