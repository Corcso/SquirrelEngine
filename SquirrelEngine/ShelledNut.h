#pragma once
#include "PCH.h"
#include "json.hpp"
#include "Resource.h"
#include "Nut.h"

namespace SQ {
    class ShelledNut :
        public Resource
    {
    public:
        static ShelledNut* Load(std::string path);

        UniquePoolPtr<Nut> Instantiate();
        
        struct InstantiatePromise {
            bool complete;
            UniquePoolPtr<Nut> result;
        };

        std::shared_ptr<InstantiatePromise> InstantiateMultithread();
        static void InstantiateMultithreadWorkFunction(nlohmann::json data, std::shared_ptr<ShelledNut::InstantiatePromise> promiseToActOn);
    private:
        static UniquePoolPtr<Nut> Instantiate(nlohmann::json data);
        nlohmann::json jsonData;
    };
}