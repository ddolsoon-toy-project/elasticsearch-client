#include "StringUtil.h"

std::string StringUtil::replaceAll(const std::string &message, const std::string &pattern, const std::string &replace) 
{
	std::string result = message;
	std::string::size_type pos = 0;
	std::string::size_type offset = 0;
	while ( (pos = result.find(pattern, offset)) != std::string::npos )
	{
		result.replace(result.begin() + pos, result.begin() + pos + pattern.size(), replace);
		offset = pos + replace.size();
	}
	return result;
}

std::string StringUtil::eraseDuplicate(std::string message, char duplicate)
{
	message.erase( std::unique( message.begin(), message.end(), 
				[=]( const auto& c1, const auto& c2 ) { return c1 == duplicate && c1 == c2; } ),
			message.end());

	return message;
}
