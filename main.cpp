#include <string>
#include <algorithm>
#include <vector>
#include <map>
#include <iostream>
#include "Client.hpp"
#include "Settings.hpp"
#include "WebServer.hpp"
#include "EventLoop.hpp"

int main() {
    Settings sample_server_config;
    sample_server_config.port = 7777;
    std::vector<Settings> config;
    config.push_back(sample_server_config);
    Settings sample_server_config2;
    sample_server_config2.port = 7778;
    config.push_back(sample_server_config2);

    EventLoop *loop = new EventLoop(config);

    // инициализация сеттинга
    Setting *setting = new Setting();
    setting->setHost("127.0.0.1");
    setting->setServerName("localhost");
    setting->setPort(8080);
    setting->testfillError();
    setting->testfillFavicon();
    setting->testfillIndex();
    setting->testfillRoot();

    try {
        loop->initServers();
        loop->runLoop();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        exit(EXIT_FAILURE);
    }

    delete loop;
}
