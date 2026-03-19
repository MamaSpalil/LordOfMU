#ifndef __ATLRX_COMPAT_H__
#define __ATLRX_COMPAT_H__

#pragma once

//
// atlrx.h - Compatibility replacement for the ATL regex header
// that was removed after Visual Studio 2008.
//
// Provides CAtlRECharTraitsA, CAtlREMatchContext, and CAtlRegExp
// using std::regex internally with ATL-to-ECMAScript pattern conversion.
//

#include <regex>
#include <string>

struct CAtlRECharTraitsA
{
	typedef char RECHARTYPE;
};


template <typename CharTraits>
class CAtlREMatchContext
{
public:
	// Note: This minimal compatibility implementation does not store
	// capture group data. The original code only uses boolean match results.
	CAtlREMatchContext() {}
};


template <typename CharTraits>
class CAtlRegExp
{
	typedef typename CharTraits::RECHARTYPE RECHAR;

	std::basic_regex<RECHAR> m_regex;
	bool m_bValid;

public:
	CAtlRegExp() : m_bValid(false) {}

	int Parse(const RECHAR* szPattern, bool bCaseSensitive = true)
	{
		if (!szPattern)
		{
			m_bValid = false;
			return 1;
		}

		try
		{
			std::string converted = ConvertATLtoECMA(szPattern);

			std::regex_constants::syntax_option_type flags = std::regex_constants::ECMAScript;

			if (!bCaseSensitive)
				flags = flags | std::regex_constants::icase;

			m_regex.assign(converted, flags);
			m_bValid = true;
			return 0;
		}
		catch (...)
		{
			m_bValid = false;
			return 1;
		}
	}

	bool Match(const RECHAR* szInput, CAtlREMatchContext<CharTraits>* pContext = 0)
	{
		if (!m_bValid || !szInput)
			return false;

		try
		{
			return std::regex_search(std::string(szInput), m_regex);
		}
		catch (...)
		{
			return false;
		}
	}

private:
	static std::string ConvertATLtoECMA(const RECHAR* szPattern)
	{
		std::string result;
		bool bInCharClass = false;

		for (const RECHAR* p = szPattern; *p; ++p)
		{
			if (bInCharClass)
			{
				if (*p == '\\' && *(p + 1))
				{
					result += *p;
					++p;
					result += *p;
				}
				else
				{
					result += *p;
					if (*p == ']')
						bInCharClass = false;
				}
				continue;
			}

			switch (*p)
			{
			case '{':
				result += "(?:";
				break;
			case '}':
				result += ')';
				break;
			case '(':
				result += "\\(";
				break;
			case ')':
				result += "\\)";
				break;
			case '[':
				bInCharClass = true;
				result += '[';
				break;
			case '!':
				if (*(p + 1) == '{')
				{
					result += "(?!";
					++p;
				}
				else
				{
					result += '!';
				}
				break;
			case '\\':
				if (!*(p + 1))
				{
					result += '\\';
					break;
				}
				++p;
				switch (*p)
				{
				case 'a':
					result += "[a-zA-Z]";
					break;
				case 'h':
					result += "[0-9a-fA-F]";
					break;
				case 'q':
					result += "\"[^\"]*\"";
					break;
				case 'd':
					result += "\\d";
					break;
				case 'w':
					result += "\\w";
					break;
				case 's':
					result += "\\s";
					break;
				default:
					result += '\\';
					result += *p;
					break;
				}
				break;
			default:
				result += *p;
				break;
			}
		}

		return result;
	}
};


#endif // __ATLRX_COMPAT_H__
