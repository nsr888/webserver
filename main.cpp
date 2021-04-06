#include <string>
#include <algorithm>
#include <vector>
#include <map>
#include <iostream>
#include "Client.hpp"
#include "Settings.hpp"
#include "WebServer.hpp"

int main() {
    Settings sample_server_config;
    sample_server_config.port = 7777;
    std::vector<Settings> config;
    config.push_back(sample_server_config);

    for ( std::vector<Settings>::iterator it = config.begin();
            it != config.end();)
    {
        WebServer   server(*it);
        server.runServer();
    }
}
