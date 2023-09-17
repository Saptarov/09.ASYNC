#ifndef BUILKPROCESSOR_H
#define BUILKPROCESSOR_H
#include "common.h"
#include "BulkWriter.h"
#include "Worker.h"

class BulkProcessor {
public:
    BulkProcessor(int blockSize);
    ~BulkProcessor();
    void processCommands();
    void processCommands(std::vector<std::string> commands);

private:
    void processCommand(const std::string& command);
    void startDynamicBlock();
    void endDynamicBlock();
    void runWriter();

private:
    struct DynamicBlock {
        DynamicBlock(bool lastClosedBracket);
        bool canWriteCommand();
    private:
        bool _lastClosedBracket;
    };

    const size_t blockSize_;
    std::list<DynamicBlock> _dynamicBlocks;
    std::vector<std::string> _blocks;
    uint _blockLen;
    std::mutex _writeQuueue;
    std::shared_ptr<Worker> simplePool;
};
#endif // BUILKPROCESSOR_H
