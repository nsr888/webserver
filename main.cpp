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
    std::vector<Setting> config;

    Setting setting;
    setting.setHost("127.0.0.1");
    setting.setServerName("localhost");
    setting.setPort(7777);
    setting.testfillError();
    setting.testfillFavicon();
    setting.testfillIndex();
    setting.testfillRoot();

    config.push_back(setting);

    Setting setting2;
    setting2.setHost("127.0.0.1");
    setting2.setServerName("localhost");
    setting2.setPort(7777);
    setting2.testfillError();
    setting2.testfillFavicon();
    setting2.testfillIndex();
    setting2.testfillRoot();

    config.push_back(setting2);

    EventLoop *loop = new EventLoop(config);

    try {
        loop->initServers();
        loop->runLoop();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        exit(EXIT_FAILURE);
    }

    delete loop;
}
