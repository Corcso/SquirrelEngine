#pragma once
#include "SquirrelEngine.h"



using namespace SQ;

/// <summary>
/// Example nut to show how airlock scene loading can be done using MultithreadInstantiate();
/// </summary>
class AirlockSceneLoading :
    public Nut
{
public:
    static SQ::UniquePoolPtr<SQ::Nut> Deserialize(Nut* deserializeInto, nlohmann::json serializedData);

    virtual void Ready() override;
    virtual void Update() override;
    virtual void LateUpdate() override;
private:
    /// <summary>
    /// The ShelledNut to instantiate containing the large new scene or level
    /// </summary>
    std::shared_ptr<ShelledNut> packedScene;
    /// <summary>
    /// The multithreaded load promise, all it contains is a complete boolean and ownership of the root nut. 
    /// </summary>
    std::shared_ptr<ShelledNut::InstantiatePromise> promise;
    /// <summary>
    /// Loaded boolean, make sure we only place the new scene in the main tree once. 
    /// </summary>
    bool loaded;
};

