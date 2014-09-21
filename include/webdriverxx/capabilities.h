#ifndef WEBDRIVERXX_CAPABILITIES_H
#define WEBDRIVERXX_CAPABILITIES_H

#include <picojson.h>
#include <string>

namespace webdriverxx {
namespace detail {
struct CapabilitiesAccess;
} // namespace detail

class Capabilities { // copyable
public:
	bool Contains(const std::string& name) const {
		return object_.find(name) != object_.end();
	}

	bool GetBool(const std::string& name) const {
		picojson::object::const_iterator it = object_.find(name);
		return it == object_.end() ? false : it->second.evaluate_as_boolean();
	}

	std::string GetString(const std::string& name) const {
		picojson::object::const_iterator it = object_.find(name);
		return it == object_.end() ? std::string() : it->second.to_str();
	}

	Capabilities() {}

	Capabilities& With(const std::string& name, const std::string& value)
	{
		object_[name] = picojson::value(value);
		return *this;
	}

	Capabilities& With(const std::string& name, bool value)
	{
		object_[name] = picojson::value(value);
		return *this;
	}

private:
	friend struct detail::CapabilitiesAccess;

	explicit Capabilities(const picojson::object& object)
		: object_(object) {}

	const picojson::object& GetJsonObject() const {
		return object_;
	}

private:
	picojson::object object_;
};

} // namespace webdriverxx

#endif
