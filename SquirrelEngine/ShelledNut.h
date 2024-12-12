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

        std::unique_ptr<Nut> Instantiate();
        
        struct InstantiatePromise {
            bool complete;
            std::unique_ptr<Nut> result;
        };

        std::shared_ptr<InstantiatePromise> InstantiateMultithread();
        static void InstantiateMultithreadWorkFunction(nlohmann::json data, std::shared_ptr<ShelledNut::InstantiatePromise> promiseToActOn);
    private:
        static std::unique_ptr<Nut> Instantiate(nlohmann::json data);
        nlohmann::json jsonData;
    };
}