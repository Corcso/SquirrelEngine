#include "PCH.h"
#include "Nut.h"
#include "Services.h"
namespace SQ {
	void Nut::SetParent(Nut* newParent, UniquePoolPtr<Nut> currentOwnership)
	{
		// If we have a parent, take ownership from it, return that ownership and remove ourself from the parents children
		if (parent != nullptr) {
			for (std::vector<UniquePoolPtr<Nut>>::iterator it = parent->children.begin(); it != parent->children.end(); it++) {
				if (it->get() == this) {
					newParent->AddChild(std::move(*it));
					parent->children.erase(it);
					break;
				}
			}
		}
		// If we dont have a parent, currentOwnership should be set to take the ownership. 
		else {
			newParent->AddChild(std::move(currentOwnership));
		}

		// Set our parent to our new parent. 
		parent = newParent;
	}

	UniquePoolPtr<Nut> Nut::TakeOwnership()
	{
		// Get temp owner
		UniquePoolPtr<Nut> tempOwner;
		// If we have a parent then take ownership from it, and remove ourself as a child
		if (parent != nullptr) {
			for (std::vector<UniquePoolPtr<Nut>>::iterator it = parent->children.begin(); it != parent->children.end(); it++) {
				if (it->get() == this) {
					tempOwner = std::move(*it);
					parent->children.erase(it);
					break;
				}
			}
		}
		// Return the taken ownership
		// If no parent it is assumed the application programmer has ownership. 
		return std::move(tempOwner);
	}

	void Nut::AddChild(UniquePoolPtr<Nut> newChild)
	{
		// Set new childs parent to us
		newChild->parent = this;
		// Get a pointer to the new child, before passing over ownership.
		Nut* childPointerForSignal = newChild.get();
		// Add the child to our children vector, taking ownership.
		children.push_back(std::move(newChild));

		// Signal all nodes above that a new child was added. 
		NewChildAdded(true, childPointerForSignal);
	}

	unsigned int Nut::GetChildCount()
	{
		return children.size();
	}

	void Nut::QueueDestroy()
	{
		isQueuedToDestroy = true;
	}

	bool Nut::IsQueuedForDestruction()
	{
		return isQueuedToDestroy;
	}

	UniquePoolPtr<Nut> Nut::Deserialize(Nut* deserializeInto, nlohmann::json serializedData)
	{
		// Create blank ownership
		UniquePoolPtr<Nut> owner;
		// If no deserialiseInto, we must create the nut
		if (deserializeInto == nullptr) { 
			owner = Services::GetPoolAllocationService()->MakeUniquePoolPtr<Nut>(); 
			deserializeInto	= owner.get();
		}
		// Must have name, otherwise throw error. 
		if (!serializedData["name"].is_null()) deserializeInto->name = serializedData["name"];
		else throw 17;

		return owner;
	}

	void Nut::ImGuiRenderMyInspector()
	{
		ImGui::Text(name.c_str());
	}

	void Nut::NewChildAdded(bool myChild, Nut* newChild)
	{
		// Send up the tree, myChild is always false for any recusrive call of this. 
		if(parent != nullptr) parent->NewChildAdded(false, newChild);
	}
}