#ifndef WEBDRIVERXX_DETAIL_HTTP_CONNECTION_H
#define WEBDRIVERXX_DETAIL_HTTP_CONNECTION_H

#include "http_client.h"
#include "http_request.h"
#include "../errors.h"
#include <curl/curl.h>

namespace webdriverxx {
namespace detail {

class HttpConnection : public IHttpClient
{
public:
	HttpConnection()
		: connection_(InitCurl())
	{}

	~HttpConnection()
	{
		curl_easy_cleanup(connection_);
	}

private:
	HttpResponse Get(const std::string& url)
	{
		return HttpGetRequest(connection_, url).DoRequest();
	}
	
	HttpResponse Put(const std::string& /*url*/)
	{
		return HttpResponse();
	}
	
	HttpResponse Delete(const std::string& /*url*/)
	{
		return HttpResponse();
	}
	
	HttpResponse Post(const std::string& /*url*/)
	{
		return HttpResponse();
	}

	static CURL* InitCurl()
	{
		CURL *const result = curl_easy_init();
		if (!result)
			throw CurlException("Cannot initialize CURL");
		return result;
	}

private:
	HttpConnection(HttpConnection&);
	HttpConnection& operator=(HttpConnection&);

private:
	CURL *const connection_;
};

} // namespace detail
} // namespace webdriverxx

#endif
