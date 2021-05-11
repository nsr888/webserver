#include <string>
#include <algorithm>
#include <vector>
#include <map>
#include <iostream>
#include "Client.hpp"
#include "Setting.hpp"
#include "Parser.hpp"
#include "WebServer.hpp"
#include "EventLoop.hpp"

int main(int argc, char** argv) {
    const char *config_file = "config/default.conf"; // Конфиг по умолчанию
    if (argc != 1) {
        config_file = argv[1];                       // Если при запуске дают другой конфиг берём его
    }
    std::vector<Setting> config;
    Parser parser;
    config = parser.startParsing(config_file);       // Парсим конфиг
    parser.showConfig(config);                       // Выводит все параметры всех конфигов
    if (!parser.checkConfig(config)) {               // Проверка конфига на валидность
        exit(EXIT_FAILURE);
    }

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
