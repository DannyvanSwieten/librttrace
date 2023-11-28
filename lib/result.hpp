#pragma once
#include <string>
#include <stdexcept>

class Device;
struct Void
{};

template <typename T>
class Result
{
public:
	static Result<T> from_value(T value)
	{
		return Result(value, "");
	}

	static Result<T> from_error(const std::string& error)
	{
		if constexpr (std::is_same_v<T, Void>)
		{
			return Result(Void{}, error);
		}
		else
		{
			return Result(nullptr, error);
		}
	}

	T expect(const std::string_view message) const
	{
		if (is_error())
		{
			throw std::runtime_error(std::string(message) + ": " + m_error);
		}
		return m_value;
	}

	T value() const
	{
		return m_value;
	}
	const std::string& error() const
	{
		return m_error;
	}

	bool is_error() const
	{
		return m_value == nullptr;
	}
	bool is_value() const
	{
		return m_value != nullptr;
	}

private:
	Result(T value, const std::string& error) : m_value(value), m_error(error)
	{}

private:
	T m_value;
	std::string m_error;
};