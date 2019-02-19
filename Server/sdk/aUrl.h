/**
* @file aUrl.h
* @brief url 解析类
* @author 
* @version 
* @date 2013-07-09
*/
#include <string>
#include <map>
class aUrl
{
	public:
		aUrl(const char * http):_http(http)
		{
			parse();
		}
		aUrl(std::string & http):_http(http)
		{
			parse();
		}

		std::string getParam()
		{
			return _param;
		}	

		std::string getHost()
		{
			return _host;
		}

		std::string getPath()
		{
			return _path;
		}

		std::string geturl()
		{
			return "http://" + _host + _path +"?"+_param;
		}

		std::string urlEncode(const std::string& url)
		{
			std::string src = url;
			char hex[] = "0123456789ABCDEF";
			std::string dst;
			for (size_t i = 0; i < src.size(); ++i)
			{
				unsigned char cc = src[i];
				if (isascii(cc))
				{
					if (cc == ' ')
					{
						dst += "%20";
					}
					else
						dst += cc;
				}
				else
				{
					unsigned char c = static_cast<unsigned char>(src[i]);
					dst += '%';
					dst += hex[c / 16];
					dst += hex[c % 16];
				}
			}
			return dst;
		}

		std::string urlDecode(const std::string& url)
		{
			std::string result;  
			int hex = 0;  
			for (size_t i = 0; i < url.length(); ++i)  
			{  
				switch (url[i])  
				{  
					case '+':  
						result += ' ';  
						break;  
					case '%':  
						if (isxdigit(url[i + 1]) && isxdigit(url[i + 2]))  
						{
							std::string hexStr = url.substr(i + 1, 2);  
							hex = strtol(hexStr.c_str(), 0, 16);
							//字母和数字[0-9a-zA-Z]、一些特殊符号[$-_.+!*'(),] 、以及某些保留字[$&+,/:;=?@]  
							//可以不经过编码直接用于URL  
							if (!((hex >= 48 && hex <= 57) || //0-9  
										(hex >=97 && hex <= 122) ||   //a-z  
										(hex >=65 && hex <= 90) ||    //A-Z  
										//一些特殊符号及保留字[$-_.+!*'(),]  [$&+,/:;=?@]  
										hex == 0x21 || hex == 0x24 || hex == 0x26 || hex == 0x27 || hex == 0x28 || hex == 0x29 
										|| hex == 0x2a || hex == 0x2b|| hex == 0x2c || hex == 0x2d || hex == 0x2e || hex == 0x2f 
										|| hex == 0x3A || hex == 0x3B|| hex == 0x3D || hex == 0x3f || hex == 0x40 || hex == 0x5f 
							     ))  
							{  
								result += char(hex);
								i += 2;  
							}  
							else result += '%';  
						}else {  
							result += '%';  
						}  
						break;  
					default:
						result += url[i];  
						break;  
				}  
			}  
			return result;  

		}

		const std::string &operator[](const char * name)const
		{
			std::map<std::string , std::string>::const_iterator it = _paramMap.find(name);
			if (it != _paramMap.end())
			{
				return it->second;
			}

			return NoneStr;
		}

	private:	

		size_t  string_trim_before(size_t begin)
		{
			if(begin == std::string::npos)
				return begin;
			while(begin < _http.size() && _http.at(begin) == ' ')++begin;
			return begin;
		}
		void parse()
		{
			if(_http.find("GET") != std::string::npos)
			{
				size_t  path_begin = _http.find("GET") + 4;
				size_t  path_end  = _http.find("?");

				if( path_end == std::string::npos)
				{
					path_end = _http.find(" ",path_begin);
					if( path_end != std::string::npos)
						_path = _http.substr(path_begin,path_end-path_begin);
				}
				else
				{
					_path = _http.substr(path_begin,path_end-path_begin);
				}
						

				
				size_t param_begin = _http.find("?");


				if(param_begin == std::string::npos)
				{
					return;
				}
				param_begin = string_trim_before(param_begin + 1);
				size_t param_end = _http.find(" ",param_begin);
				_param = _http.substr(param_begin,param_end-param_begin);		


				std::vector<std::string> strparam;

				Age::stringbreak(strparam,_param,"&");	

				for(std::vector<std::string>::iterator it = strparam.begin() ; it != strparam.end(); ++ it)
				{
					size_t split = (*it).find("=");	
					_paramMap.insert(std::make_pair( (*it).substr(0,split),(*it).substr(split + 1)));	
				}

				size_t host_begin = _http.find("Host:");
				if(host_begin != std::string::npos)
				{
					host_begin = string_trim_before(host_begin);
					size_t host_end = _http.find("\r\n",host_begin);
					if(host_end != std::string::npos)
						_host = _http.substr(host_begin,host_end - host_begin);		
				}
			}
			
			else if(_http.find("POST") != std::string::npos)
			{
				do{


					size_t  path_begin = _http.find("GET") + 4;
					size_t  path_end  = _http.find("?");

					if( path_end == std::string::npos)
					{
						path_end = _http.find(" ",path_begin);
						if( path_end != std::string::npos)
							_path=_http.substr(path_begin,path_end-path_begin);
					}
					else
					{
						_path = _http.substr(path_begin,path_end-path_begin);
					}
					
					size_t param_begin = _http.find("?");

					if(param_begin == std::string::npos)
					{
						break;
					}
					param_begin = string_trim_before(param_begin + 1);

					size_t param_end = _http.find(" ",param_begin);
					_param = _http.substr(param_begin,param_end-param_begin);


					std::vector<std::string> strparam;

					Age::stringbreak(strparam,_param,"&");

					for(std::vector<std::string>::iterator it = strparam.begin() ; it != strparam.end(); ++ it)
					{
						size_t split = (*it).find("=");
						_paramMap.insert(std::make_pair( (*it).substr(0,split),(*it).substr(split + 1)));
					}
				}while(false);

				size_t host_begin = _http.find("Host:");
				if(host_begin != std::string::npos)
				{
					host_begin = string_trim_before(host_begin);
					size_t host_end = _http.find("\r\n",host_begin);
					if(host_end != std::string::npos)
						_host = _http.substr(host_begin,host_end - host_begin);		
				}


				size_t contenLenBegin = _http.find("Content-Length:");		
				size_t contenLenEnd =  _http.find("\r\n",contenLenBegin);

				std::string lenstr = _http.substr(contenLenBegin + 15,contenLenEnd - contenLenBegin - 15);

				size_t param_len = atoi(lenstr.c_str());
				if(param_len == 0)
					return;

				size_t param_begin = _http.find("\r\n\r\n");
				if(param_begin == std::string::npos)
					return;
				param_begin = string_trim_before(param_begin + 4);


				_param = _http.substr(param_begin,param_len);		


				std::vector<std::string> strparam;

				Age::stringbreak(strparam,_param,"&");	

				for(std::vector<std::string>::iterator it = strparam.begin() ; it != strparam.end(); ++ it)
				{
					size_t split = (*it).find("=");	
					_paramMap.insert(std::make_pair( (*it).substr(0,split),(*it).substr(split + 1)));	
				}


			}

		}	

		std::string _http;
		std::string _param;
		std::string _host;
		std::string _path;
		std::map<std::string , std::string> _paramMap;
		std::string NoneStr;
};

