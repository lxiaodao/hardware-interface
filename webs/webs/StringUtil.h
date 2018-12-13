#pragma once
#include <string>
using namespace std;




class StringUtil {

public:
	/**
	 src£»Ô´×Ö·û´®
	 format£º±»Ìæ»»×Ö·û´®
	 dest£ºÌæ»»³ÉµÄ×Ö·û´®
	*/
	static void StringUtil::string_replaceAll(string &src, const string formart, const string dest)
	{
		string::size_type pos = 0;
		string::size_type a = formart.size();
		string::size_type b = dest.size();
		while ((pos = src.find(formart, pos)) != string::npos)
		{
			src.replace(pos, a, dest);
			pos += b;
		}
	}

  };



