/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaamaich <yaamaich@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/12 20:01:04 by yaamaich          #+#    #+#             */
/*   Updated: 2026/11/12 20:01:04 by yaamaich         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGI_HPP
#define CGI_HPP

#include <string>
#include <vector>

class CGI
{
    public:
        CGI();
        ~CGI();

        std::string execute(const std::string& scriptPath,
                            const std::string& interpreter,
                            const std::string& method,
                            const std::string& queryString,
                            const std::string& body,
                            const std::string& contentType) const;

    private:
        static char** buildEnvironment(const std::string& scriptPath,
                                      const std::string& method,
                                      const std::string& queryString,
                                      const std::string& body,
                                      const std::string& contentType);
};

#endif