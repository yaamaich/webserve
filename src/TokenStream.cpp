/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TokenStream.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emad <emad@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/20 14:30:25 by imatouil          #+#    #+#             */
/*   Updated: 2026/10/21 18:56:01 by emad             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/TokenStream.hpp"
#include <fstream>
#include <sstream>
#include <stdexcept>

TokenStream::TokenStream() : _cursor(0) {};

std::string	TokenStream::sanitize(const std::string& line) const
{
	std::string stripped = line;
	std::string out = "";
	size_t comment_pos = line.find('#');
	if (comment_pos != std::string::npos)
		stripped = line.substr(0, comment_pos);
	for (size_t i = 0; i < stripped.length(); i++)
	{
		char c = stripped[i];
		if (c == '{' || c == '}' || c == ';')
			out = out + ' ' + c + ' ';
		else
			out += c;
	}
	return (out);
}

void	TokenStream::loadFromFile(const std::string& filename)
{
	_tokens.clear();
	_cursor = 0;
	std::ifstream file(filename.c_str());
	if (!file.is_open())
		throw std::runtime_error("Config error: cannot open file: " + filename);
	std::string	line;
	while (std::getline(file, line))
	{
		std::stringstream	ss(sanitize(line));
		std::string tok;
		while (ss >> tok)
			_tokens.push_back(tok);
	}
}

bool	TokenStream::hasNext() const
{
	return (_cursor < _tokens.size());
}

const std::string&	TokenStream::peek() const
{
	if (!hasNext())
		throw std::runtime_error("Config Error: Unexpected end of file");
	return (_tokens[_cursor]);
}

std::string	TokenStream::next()
{
	const std::string& tok = peek();
	_cursor++;
	return (tok);
}

void	TokenStream::expect(const std::string& expected)
{
	std::string actual = next();
	if (actual != expected)
		throw std::runtime_error("Config Syntax Error: Expected '" + expected + "', got '" + actual + "'");
}
