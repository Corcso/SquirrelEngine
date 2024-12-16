#pragma once
#include "Nut.h"



namespace SQ {

    class Tree;

    /// <summary>
    /// Special nut only used by Tree, for calling Ready functions. 
    /// </summary>
    class RootNut :
        public Nut
    {
    public:
        // Friends with tree so private constructor can be accessed. 
        friend class Tree;

        // Make sure to call base class ready update and late update
        virtual void Ready() override { Nut::Ready(); }
        virtual void Update() override { Nut::Update(); }
        virtual void LateUpdate() override { Nut::LateUpdate(); }

    private:
        // Private constructor
        RootNut() {};



        // Override new child added to call ready on them
        virtual void NewChildAdded(bool myChild, Nut* newChild) override {
        
            RunReadyOnChildren(newChild);
            Nut::NewChildAdded(myChild, newChild);
        }

        /// <summary>
        /// Recursive function for running ready on children just added to the tree. 
        /// </summary>
        /// <param name="from">Where to start</param>
        void RunReadyOnChildren(Nut* from) {
            if (!from->HasReadyBeenCalled()) from->Ready();

            unsigned int childCount = from->GetChildCount();
            for (unsigned int c = 0; c < childCount; ++c) {
                RunReadyOnChildren(from->GetNthChild(c));
            }
        }

    };
}