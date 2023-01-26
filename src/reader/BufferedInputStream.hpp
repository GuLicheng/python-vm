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


			// for (int i = 0; i < stream.sz_buffer.size(); ++i)
			// {
			// 	os.setf(std::ios::hex, std::ios::basefield);
			// 	os.width(2);
			// 	os.fill('0');
			// 	os << (stream.sz_buffer[i] & 0xFF) << ' ';
			// 	if (i % 16 == 15) os << '\n';
			// }
			// os.setf(std::ios::dec, std::ios::basefield);
			// return os;
		}

	};

}