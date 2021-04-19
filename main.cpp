#include <string>
#include <algorithm>
#include <vector>
#include <map>
#include <iostream>
#include "Client.hpp"
#include "Setting.hpp"
#include "WebServer.hpp"
#include "EventLoop.hpp"

int main() {
    Setting sample_server_config;
    sample_server_config.setPort(7777);
    std::vector<Setting> config;
    config.push_back(sample_server_config);
    Setting sample_server_config2;
    sample_server_config2.setPort(7778);
    config.push_back(sample_server_config2);

    EventLoop *loop = new EventLoop(config);

    try {
        loop->initServers();
        loop->runLoop();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        exit(EXIT_FAILURE);
    }

    delete loop;
}
