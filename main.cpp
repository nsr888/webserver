#include <string>
#include <algorithm>
#include <vector>
#include <map>
#include <iostream>
#include "Client.hpp"
#include "Setting.hpp"
#include "WebServer.hpp"
#include "EventLoop.hpp"

<<<<<<< Updated upstream
int main() {
    Setting sample_server_config;
    sample_server_config.setPort(7777);
    std::vector<Setting> config;
    config.push_back(sample_server_config);
    Setting sample_server_config2;
    sample_server_config2.setPort(7778);
    config.push_back(sample_server_config2);
=======
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

    while (i < 4) {                                                 //проверка парсинга конфига, выводит все 4 пути из дефолтного конфига
		std::cout << config[0].getLocationPath(i) << std::endl;
		i++;
	}

    // i = 0;
    // std::cout << std::endl;
    // while (i < 4) { //проверка парсинга второго конфига, выводит все 4 пути из второго конфига мультисервера
	// 	std::cout << config[1].getLocationPath(i) << std::endl;
	// 	i++;
	// }

    EventLoop *loop = new EventLoop(config);

    // инициализация сеттинга вручную
    // Setting *setting = new Setting();
    // setting->setHost("127.0.0.1");
    // setting->setServerName("localhost");
    // setting->setPort(8080);
    // setting->testfillError();
    // setting->testfillFavicon();
    // setting->testfillIndex();
    // setting->testfillRoot();
    // setting->setMaxBodySize(100);
>>>>>>> Stashed changes

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
    setting->setMaxBodySize(100);

    try {
        loop->initServers();
        loop->runLoop();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        exit(EXIT_FAILURE);
    }

    delete loop;
}
