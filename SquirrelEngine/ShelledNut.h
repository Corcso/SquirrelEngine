#pragma once
#include "PCH.h"
#include "json.hpp"
#include "Resource.h"
#include "Nut.h"

namespace SQ {
    /// <summary>
    /// Shelled nut for loading scenes from files. 
    /// </summary>
    class ShelledNut :
        public Resource
    {
    public:
        /// <summary>
        /// Load a scene file into memory, does not load any resources or create any nuts. 
        /// </summary>
        /// <param name="path">Path to .nut file</param>
        /// <returns>Pointer to new ShelledNut</returns>
        static ShelledNut* Load(std::string path);

        /// <summary>
        /// Onthread instantiation.
        /// <para>Creates the nuts and loads any required resources.</para>
        /// <para>Will block frame renders, only recomended for small quick scenes</para>
        /// </summary>
        /// <returns>Ownership of the root of the Shelled nut</returns>
        UniquePoolPtr<Nut> Instantiate();
        
        /// <summary>
        /// Instantiation promise used by the multithreaded instantiation. 
        /// </summary>
        struct InstantiatePromise {
            bool complete; // True if the instantiation is complete
            UniquePoolPtr<Nut> result; // If complete, ownership of the root nut
        };

        /// <summary>
        /// Instantiate on another thread. Useful for loading big scenes seamlessly. 
        /// </summary>
        /// <returns>A promise, see InstantiatePromise</returns>
        std::shared_ptr<InstantiatePromise> InstantiateMultithread();

        /// <summary>
        /// Worker function for the thread which runs instantiation
        /// </summary>
        /// <param name="data">Data to deserialize</param>
        /// <param name="promiseToActOn">Promise to act upon</param>
        static void InstantiateMultithreadWorkFunction(nlohmann::json data, std::shared_ptr<ShelledNut::InstantiatePromise> promiseToActOn);
    private:
        /// <summary>
        /// Private instantiate function which is recursive per child. 
        /// </summary>
        /// <param name="data">Data to deserialize</param>
        /// <returns>Ownership of the root nut</returns>
        static UniquePoolPtr<Nut> Instantiate(nlohmann::json data);
        nlohmann::json jsonData;
    };
}