/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emad <emad@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/10 16:22:24 by imatouil          #+#    #+#             */
/*   Updated: 2026/10/21 21:17:29 by emad             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include "../includes/Server.hpp"
// #include <iostream>

// int main()
// {
//     Server server(8080);

//     if (!server.init())
//     {
//         std::cerr << "Error: cannot init server" << std::endl;
//         return 1;
//     }
//     server.run();
//     return 0;
// }



#include "../includes/Server.hpp"
#include "../includes/ConfigParser.hpp"
#include <iostream>

int main(int ac, char **av)
{
    if (ac > 2)
    {
        std::cerr << "Usage: " << av[0] << " [configuration file]" << std::endl;
        return (1);
    }
    std::string configPath = "configs/default.conf";
    if (ac == 2)
        configPath = av[1];
    try
    {
        ConfigParser parser;
        std::vector<ServerConfig> _serversConfigs = parser.parse(configPath);
        Server server(_serversConfigs);
        if (!server.init())
        {
            std::cerr << "Error: cannot init server" << std::endl;
            return 1;
        }
        server.run();
    }
    catch(const std::exception& e)
    {
        std::cerr << "Fatal Error: " << e.what() << std::endl;
        return 1;
    }
    // return (0);
}
