#include <iostream>
#include <vector>
#include <glibmm/ustring.h>
static int currentId = 0;
class Type{
    public:
        Type(const std::string name){
            id = ++currentId;
            this->name = name;
            std::cout << "Type "<<name<<" constructed. Id: "<<id<<"\n";
        }
        ~Type(){
            // std::cout << "Type "<<name<<" destructed "<<id<<"\n";
            resistantAgainstTypes.~vector();
            weakAgainstTypes.~vector();
        }
        bool nameMatches(std::string comparison){
            return name.compare(comparison) == 0;
        }
        std::string name;
        int id;

        std::vector<int> resistantAgainstTypes = std::vector<int>();
        std::vector<int> weakAgainstTypes = std::vector<int>();
};