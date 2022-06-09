#ifndef __YADSL_PAIR__
#define __YADSL_PAIR__

#include <iostream>
#include <cinttypes>
#include <exception>

template <typename Key, typename T>
class pair_t
{
	using key_type = Key;
	using value_type = T;

	private:
		key_type _key;
		value_type _value;

	public:
		pair_t () {}
		pair_t (const key_type& key, const value_type& value)
		{
			this->_key = key;
			this->_value = value;
		}

	// element access
	public:
		inline key_type& key () noexcept
		{
			return this->_key;
		}
		inline value_type& value () noexcept
		{
			return this->_value;
		}
		inline key_type get_key () const noexcept
		{
			return this->_key;
		}
		inline value_type get_value () const noexcept
		{
			return this->_value;
		}

	// element modifier
	public:
		void set_key (const key_type& key) noexcept
		{
			this->_key = key;
		}
		void set_value (const value_type& value) noexcept
		{
			this->_value = value;
		}
};

#endif
