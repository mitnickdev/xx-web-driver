#ifndef WEBDRIVERXX_WEBDRIVER_H
#define WEBDRIVERXX_WEBDRIVER_H

#include "element.h"
#include "window.h"
#include "by.h"
#include "capabilities.h"
#include "types.h"
#include "keys.h"
#include "js_args.h"
#include "detail/finder.h"
#include "detail/resource.h"
#include "detail/http_connection.h"
#include "detail/keyboard.h"
#include <string>

namespace webdriverxx {

const char *const kDefaultUrl = "http://localhost:4444/wd/hub/";

// Gives access to some API functions but doesn't create session
class BasicWebDriver { // noncopyable
public:
	explicit BasicWebDriver(const std::string& url = kDefaultUrl);
	virtual ~BasicWebDriver() {}

	picojson::object GetStatus() const;
	SessionsInformation GetSessions() const;

protected:
	struct Session {
		detail::Resource resource;
		Capabilities capabilities;

		explicit Session(
			const detail::Resource& resource,
			const Capabilities& capabilities = Capabilities()
			)
			: resource(resource)
			, capabilities(capabilities) {}
	};

	Session CreateSession(
		const Capabilities& required,
		const Capabilities& desired
		) const;

private:
	BasicWebDriver(BasicWebDriver&);
	BasicWebDriver& operator=(BasicWebDriver&);

private:
	const detail::HttpConnection http_connection_;
	const detail::ServerRoot server_root_;
};

class WebDriver // noncopyable
	: public BasicWebDriver
	, private detail::Finder {
public:
	explicit WebDriver(
		const std::string& url = kDefaultUrl,
		const Capabilities& required = Capabilities(),
		const Capabilities& desired = Capabilities()
		);

	Capabilities GetCapabilities() const;
	std::string GetSource() const;
	std::string GetTitle() const;
	std::string GetUrl() const;
	std::vector<Window> GetWindows() const;
	Window GetCurrentWindow() const;

	const WebDriver& Navigate(const std::string& url) const;
	const WebDriver& Forward() const;
	const WebDriver& Back() const;
	const WebDriver& Refresh() const;

	const WebDriver& Execute(const std::string& script, const JsArgs& args = JsArgs()) const;
	template<typename T>
	T Eval(const std::string& script, const JsArgs& args = JsArgs()) const;

	const WebDriver& CloseCurrentWindow() const;
	const WebDriver& SetFocusToWindow(const std::string& name_or_handle) const;
	
	Element FindElement(const By& by) const;
	std::vector<Element> FindElements(const By& by) const;

	const WebDriver& SendKeys(const char* keys) const;
	const WebDriver& SendKeys(const std::string& keys) const;
	const WebDriver& SendKeys(const Shortcut& shortcut) const;

private:
	Element FindElement(
		const By& by,
		const detail::Resource& context
		) const;
	std::vector<Element> FindElements(
		const By& by,
		const detail::Resource& context
		) const;
	Window MakeWindow(const std::string& handle) const;
	Element MakeElement(const std::string& id) const;
	detail::Keyboard GetKeyboard() const;

private:
	const Session session_;
	const detail::Resource& resource_;
	const detail::AutoResourceDeleter session_deleter_;
};

} // namespace webdriverxx

#include "webdriver.inl"

#endif
