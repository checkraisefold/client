#pragma once
#include <any>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <memory>
#include <string>
#include <string_view>
#include <type_traits>
#include <typeindex>
#include <typeinfo>
#include <unordered_map>
#include <vector>

#include "utils.hpp"

class settings {
 private:
	std::unordered_map<std::string_view, std::any> vars{
	    {"console_open", false},
	};

 public:
	const bool init(const char *config_name = "options.txt");

	template <typename T>
	const T get_var(const char *idx) const {
		if (!vars.contains(idx)) throw std::runtime_error(fmt::format("Unknown setting '{}'", idx));
		return std::any_cast<T>(vars.at(idx));
	}

	template <typename T>
	bool set_var(const char *idx, const T val) const {
		auto v = vars.at(idx);

		auto &type = v.type();
		if (type != typeid(val))
			throw std::runtime_error(
			    fmt::format("Settings, Invalid type for {}, expected {} got {}", idx,
			                type.name(), typeid(val).name()));

		vars.at(idx) = val;
	}

	const std::type_info &get_type_info(const char *idx) {
		return vars.at(idx).type();
	}
};

inline std::unique_ptr<settings> g_settings;
