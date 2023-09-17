#include "BulkWriter.h"

void BulkWriter::output(std::vector<std::string>& blocks, uint numQueue) {
    std::shared_lock<std::shared_mutex> readTLock(_rwBlockMutex);
    auto buffer = blocks;
    if (buffer.empty()) {
        return;
    }
    auto currentTime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::string filename = "b" + std::to_string(currentTime) + "_" + std::to_string(numQueue) + ".log";

    std::ofstream file(filename);
    for (const auto& command : buffer) {
        file << command << std::endl;
    }
    file.close();

    lg->out("bulk:");
    for (const auto& command : buffer) {
        lg->out(" %", command);
    }
    lg->endl();

    buffer.clear();
}
