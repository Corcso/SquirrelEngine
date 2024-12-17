#pragma once
#include "PCH.h"
#include "SQUtility.h"
#include <iostream>
namespace SQ {
	/// <summary>
	/// Base nut.
	/// </summary>
	class Nut
	{
	public: 
		std::string name;

		/// <summary>
		/// <para>Returns the parent of this nut. </para>
		/// <para>Returns nullptr if Nut has no parent.</para>
		/// <para>Returns nullptr if parent is not of expected type. (Use T = Nut to guarantee success)</para>
		/// </summary>
		/// <typeparam name="T">Type of nut expected</typeparam>
		/// <returns>Pointer to parent nut</returns>
		template <typename T = Nut>
		inline T* GetParent() {
			// If parent exists dynamically cast it to T
			if (parent) return dynamic_cast<T*>(parent);
			// Otherwise return null pointer
			else return nullptr;
		}

		/// <summary>
		/// <para>Gets the Nth child of this Nut. </para>
		/// <para>Returns nullptr if index is too large.</para>
		/// <para>Returns nullptr if child is not of expected type. (Use T = Nut to guarantee success)</para>
		/// </summary>
		/// <typeparam name="T">Type of nut expected</typeparam>
		/// <param name="index">Index N of child</param>
		/// <returns>Pointer to the requested child</returns>
		template <typename T = Nut>
		inline T* GetNthChild(unsigned int index) {
			// Check we have a child at that index
			if (index < children.size()) return dynamic_cast<T*>(children[index].get());
			// If not return null pointer
			else return nullptr;
		}

		/// <summary>
		/// <para>Gets the decendant of this Nut based on path provided. </para>
		/// <para>Returns nullptr if decendant cannot be found</para>
		/// <para>Returns nullptr if decendant is not of expected type. (Use T = Nut to guarantee success)</para>
		/// </summary>
		/// <typeparam name="T">Type of nut expected</typeparam>
		/// <param name="path">Path to nut in the form of, Child/Grandchild/Greatgrandchild</param>
		/// <returns>Pointer to the requested nut</returns>
		template <typename T = Nut>
		inline T* GetNut(std::string path) {
			std::vector<std::string> splitPath = SplitString(path, '/');

			// If path given to us was "" then we are the desired Nut
			if(splitPath.size() == 1 && splitPath[0] == "") return dynamic_cast<T*>(this);

			// Else loop over our children, and return GetNut ran on the child without their name
			for (UniquePoolPtr<Nut>& child : children) {
				if (child->name == splitPath[0]) {
					splitPath.erase(splitPath.begin());
					return child->GetNut<T>(JoinString(splitPath, '/'));
				}
			}

			// If no child found with matching name, return nullptr
			return nullptr;
		}

		/// <summary>
		/// Sets the parent, use this when reparenting a nut.
		/// </summary>
		/// <param name="newParent">New parent</param>
		/// <param name="currentOwnership">Current owner unique pointer, only set this if you own the nut, if it is owned by another nut as a child this will be handled automatically. </param>
		void SetParent(Nut* newParent, UniquePoolPtr<Nut> currentOwnership = UniquePoolPtr<Nut>());

		/// <summary>
		/// <para>Takes ownership of the nut out of the parent</para>
		/// <para>Essentially a deparent</para>
		/// </summary>
		/// <returns>Ownership of the nut</returns>
		UniquePoolPtr<Nut> TakeOwnership();

		/// <summary>
		/// Adds a child to this nut.
		/// <para>Does not handle ownership! Use set parent instead for safe ownership transfer.</para>
		/// </summary>
		/// <param name="newChild"></param>
		void AddChild(UniquePoolPtr<Nut> newChild);

		/// <summary>
		/// Gets the number of children on this nut
		/// </summary>
		/// <returns>Number of children</returns>
		unsigned int GetChildCount();

		/// <summary>
		/// Ready function, runs once a nut's lifetime when it first enters the main tree. 
		/// <para>You must call this manually, if you don't add the nut to the main scene tree</para>
		/// </summary>
		virtual void Ready() { hasReadyBeenCalled = true; }
		/// <summary>
		/// Update function, runs once a frame
		/// </summary>
		virtual void Update() {}
		/// <summary>
		/// Late Update function, runs once a frame, after all updates. 
		/// </summary>
		virtual void LateUpdate() {}

		/// <summary>
		/// Queues this for destruction at the end of the current frame. 
		/// </summary>
		void QueueDestroy();

		/// <summary>
		/// Returns true if this nut will be deleted at the end of this frame.
		/// </summary>
		/// <returns>If the nut is queued for end of frame deletion.</returns>
		bool IsQueuedForDestruction();

		/// <summary>
		/// <para>Deserialize function for a Nut</para>
		/// <para>Used by ShelledNut Resource to instantiate nuts</para>
		/// <para>If deserizliseInto is nullptr, Deserialize will create a new Nut</para>
		/// <para>If deserizliseInto is not nullptr, Deserialize will not create a new Nut</para>
		/// <para>serializedData, is parsed into the new/passed in Nut</para>
		/// </summary>
		/// <param name="deserializeInto">Nut pointer to deserialise into, can be nullptr.</param>
		/// <param name="serilizedData">JSON data to be parsed into nut</param>
		/// <returns>Pointer to nut which was deserialized.</returns>
		static UniquePoolPtr<Nut> Deserialize(Nut* deserializeInto, nlohmann::json serializedData);

		/// <summary>
		/// Check for if ready has been called. 
		/// </summary>
		/// <returns>True if ready has been called. False if not.</returns>
		bool HasReadyBeenCalled() { return hasReadyBeenCalled; }

		virtual ~Nut() {}
	protected:
		/// <summary>
		/// Signal for when new children are added. 
		/// Calls the parent and lets them know. 
		/// Protected so child classes can call it when overriding it. 
		/// </summary>
		/// <param name="myChild">If the new child is this nodes direct child.</param>
		/// <param name="newChild">A pointer to the new child.</param>
		virtual void NewChildAdded(bool myChild, Nut* newChild);

		
	private:
		Nut* parent;
		std::vector<UniquePoolPtr<Nut>> children;

		bool hasReadyBeenCalled = false;

		bool isQueuedToDestroy;
	};
}

