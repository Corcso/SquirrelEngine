#pragma once
#include "PCH.h"
#include "Utility.h"
namespace SQ {
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
			if (children.size() <= index) return dynamic_cast<T*>(children[index].get());
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
			if(splitPath.size() == 0) return dynamic_cast<T*>(this);

			// Else loop over our children, and return GetNut ran on the child without their name
			for (std::unique_ptr<Nut>& child : children) {
				if (child->name == splitPath[0]) {
					splitPath.erase(splitPath.begin());
					return child->GetNut<T>(JoinString(splitPath, '/'));
				}
			}

			// If no child found with matching name, return nullptr
			return nullptr;
		}


	private:
		Nut* parent;
		std::vector<std::unique_ptr<Nut>> children;
	};
}

