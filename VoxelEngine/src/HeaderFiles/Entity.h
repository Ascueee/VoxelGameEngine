#pragma once
#include <vector>
class Entity{
    public:
        Entity();
        void SetID(int id);
        void SetParentID(int parentID);
        void SetChild(int childID);
        void SetName(std::string name);
        std::string GetName();
        int GetID() const;
        int GetParentID();
        std::vector<int> GetChildren();
    private:
        std::string name;
        int id = 0;
        int parentID = -1;
        std::vector<int> children;
        
};