#include "PCH.h"
#include "Nut.h"

void SQ::Nut::SetParent(Nut* newParent)
{
	if (parent != nullptr) {
		for (std::vector<std::unique_ptr<Nut>>::iterator it = parent->children.begin(); it != parent->children.end(); it++) {
			if (it->get() == this) {
				newParent->AddChild(std::move(*it));
				children.erase(it);
			}
		}
	}
	else {
		newParent->AddChild(std::move(std::unique_ptr<Nut>(this)));
	}
	parent = newParent;
}

void SQ::Nut::AddChild(std::unique_ptr<Nut> newChild)
{
	children.push_back(std::move(newChild));
}

unsigned int SQ::Nut::GetChildCount()
{
	return children.size();
}

void SQ::Nut::QueueDestroy()
{
	isQueuedToDestroy = true;
}

bool SQ::Nut::IsQueuedForDestruction()
{
	return isQueuedToDestroy;
}
