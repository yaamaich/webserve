/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationBuilder.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emad <emad@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/21 15:59:15 by emad              #+#    #+#             */
/*   Updated: 2026/10/21 16:27:54 by emad             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATIONBUILDER_HPP
#define LOCATIONBUILDER_HPP

#include "ConfigData.hpp"
#include <string>

class LocationBuilder
{
    private:
        LocationConfig _config;
    public:
        LocationBuilder();

        void    setPath(const std::string& path);
        void    setRoot(const std::string& root);
        void    setIndex(const std::string& index);
        void    setAutoIndex(bool state);
        void    setRedirect(const std::string& redirect);
        void    setUploadStore(const std::string& store);
        void    addMethod(const std::string& method);
        void    clearMethoD();
        void    addCgi(const std::string& ext, const std::string& excecPath);
        LocationConfig  build() const;
  
};

#endif