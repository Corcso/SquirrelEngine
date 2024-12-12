#pragma once
#include "SquirrelEngine.h"
class Spinner :
    public SQ::MeshNut
{
public:
    Spinner();

    static Nut* Deserialize(Nut* deserializeInto, nlohmann::json serializedData);

    void Update() override;
    private:
        std::shared_ptr<SQ::ShelledNut> toLoad;
        std::shared_ptr<SQ::ShelledNut::InstantiatePromise> promise;
        bool loaded = false;
        std::shared_ptr<SQ::Mesh> a;
};

