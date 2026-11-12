/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TokenStream.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imatouil <imatouil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/20 14:29:52 by imatouil          #+#    #+#             */
/*   Updated: 2026/10/20 17:47:38 by imatouil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKENSTREAM_HPP
#define TOKENSTREAM_HPP

#include <string>
#include <vector>

class TokenStream
{
	private:
		std::vector<std::string>	_tokens;
		size_t						_cursor;

		std::string			sanitize(const std::string& line) const;
	public:
		TokenStream();
		void				loadFromFile(const std::string& filename);
		bool				hasNext() const;
		const std::string&	peek() const;
		std::string			next();
		void				expect(const std::string& expected);
		
};

#endif
