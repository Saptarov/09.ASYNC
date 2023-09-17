#ifndef __BULK_WRITER_H
#define __BULK_WRITER_H
#include <common.h>
#include "Logger.h"

class BulkWriter
{
public:
    void output(std::vector<std::string>& blocks, uint numQueue);
private:
    std::shared_mutex _rwBlockMutex;
    Logger* lg = Logger::getInstance();
};

#endif // __BULK_WRITER_H
