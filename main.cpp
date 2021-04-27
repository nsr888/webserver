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
    // инициализация конфига парсером
    const char *config_file = "config/default.conf"; // конфиг по умолчанию
    if (argc != 1) {
        config_file = argv[1]; // если при запуске дают другой конфиг берём его
    }
    std::vector<Setting> config;
    Parser parser;
    config = parser.startParsing(config_file);
    size_t i = 0;

    while (i < 4) { //проверка парсинга конфига, выводит все 4 пути из дефолтного конфига
		std::cout << config[0].getLocationPath(i) << std::endl;
		i++;
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
