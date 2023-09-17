#include "BulkProcessor.h"

BulkProcessor::BulkProcessor(int blockSize)
    : blockSize_(blockSize)
{
    simplePool = std::make_shared<Worker>(2);
}

BulkProcessor::~BulkProcessor() {
    while (!simplePool->isDone()) {
        std::this_thread::yield();
    }
}

void BulkProcessor::processCommands() {
    std::string command;
    while (std::getline(std::cin, command)) {
        if (command == "{") {
            startDynamicBlock();
        } else if (command == "}") {
            endDynamicBlock();
        } else {
            processCommand(command);
        }
    }
}

void BulkProcessor::processCommands(std::vector<std::string> commands) {
    for (auto& command : commands) {
        std::cout << command << std::endl;
        if (command == "{") {
            startDynamicBlock();
        } else if (command == "}") {
            endDynamicBlock();
        } else {
            processCommand(command);
        }
    }
}

void BulkProcessor::processCommand(const std::string& command) {
    _blocks.push_back(command);
    if (_dynamicBlocks.empty() && _blocks.size() == blockSize_) {
        runWriter();
    }
}

void BulkProcessor::startDynamicBlock() {
    if (_dynamicBlocks.empty()) {
        runWriter();
        _dynamicBlocks.push_back(DynamicBlock(true));
    } else {
        _dynamicBlocks.push_back(DynamicBlock(false));
    }
}

void BulkProcessor::endDynamicBlock() {
    auto block = _dynamicBlocks.back();
    if (block.canWriteCommand()) {
        runWriter();
    }
    _dynamicBlocks.pop_back();
}

void BulkProcessor::runWriter() {
    simplePool->submit([&](){//тут само собой бьет по производительности, коль будет замечанием, подправлю
        std::lock_guard<std::mutex> l(_writeQuueue);
        _blockLen++;
        BulkWriter writer;
        writer.output(_blocks, _blockLen);
    });
}

BulkProcessor::DynamicBlock::DynamicBlock(bool lastClosedBracket)
    : _lastClosedBracket(lastClosedBracket)
{}

bool BulkProcessor::DynamicBlock::canWriteCommand() {
    return _lastClosedBracket;
}

