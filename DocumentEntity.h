#ifndef DOCUMENTENTITY_H
#define DOCUMENTENTITY_H

#include <ctime>
#include <string>

class DocumentEntity
{
private:
    std::string path;
    char* date;

public:
    DocumentEntity();
   DocumentEntity(std::string path);

    char* getCurrentDate();
    char* getDate();
};

#endif // DOCUMENTENTITY_H
