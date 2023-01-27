#pragma once

#include <fstream>
#include <string>
#include <iostream>

#include "../PrettyPrint.hpp"

namespace python
{

	class BufferedInputStream
	{
		std::ifstream ifs;
		
		std::string sz_buffer;
		
		int index;

		friend class BinaryFileParser;

		const char* data() const;

		void skip(int n);

	public:

		BufferedInputStream(const char* filename);

		~BufferedInputStream();

		char read();

		int read_int();

		double read_double();

		void unread();

		//void close();

		friend std::ostream& operator<<(std::ostream& os, const BufferedInputStream& stream)
		{
			print_hex_code<>(stream.sz_buffer.data(), stream.sz_buffer.size());
			return os;
		}

	};

}