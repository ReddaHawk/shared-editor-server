#include "DocumentEntity.h"

DocumentEntity::DocumentEntity(std::string path)
{
    this->path=path;
    this->date=getCurrentDate();
}


//Get the current date in UTC format
char* DocumentEntity::getCurrentDate(){
    // Declaring argument for time()
    std::time_t tt;

    // Declaring variable to store return value of
    // localtime()
    struct std::tm * ti;

    // Applying time()
    time (&tt);

    // Using localtime()
    ti = localtime(&tt);

    //UTC+2
    ti->tm_hour=ti->tm_hour+2;

    return asctime(ti);
}

/*Get the document creation date*/
char* DocumentEntity::getDate(){
    return date;
}
