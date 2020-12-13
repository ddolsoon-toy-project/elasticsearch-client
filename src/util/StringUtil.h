#ifndef __STRING_UTIL_H__
#define __STRING_UTIL_H__

#include <string>
#include <functional>
#include <iterator>
#include <algorithm>

class StringUtil
{
	public:
		StringUtil() { }
		~StringUtil() { }

	public:
		std::string replaceAll(const std::string &message, const std::string &pattern, const std::string &replace);
		std::string eraseDuplicate(std::string message, char duplicate);
};

#endif
