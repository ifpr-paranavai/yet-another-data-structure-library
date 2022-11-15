#ifndef __LIB__
#define __LIB__

#include <cinttypes>
#include <cassert>

#define DEBUG_MODE 0

#if DEBUG_MODE
	#include <vector>
	#include <cmath>

	template <typename T>
	class stats_t
	{
		private:
			std::vector<T> dataset;
		
		public:
			stats_t (uint64_t capacity)
			{
				this->dataset.reserve(capacity);
			}
			void add (T val)
			{
				this->dataset.push_back(val);
			}
			double mean ()
			{
				uint64_t size = this->dataset.size();
				double sum = 0;

				for (auto it : this->dataset) {
					sum += static_cast<double>(it);
				}

				return sum / static_cast<double>(size);
			}
			double stddev ()
			{
				uint32_t size = this->dataset.size();
				double mean = this->mean();
				double dev = 0;

				for (auto it : this->dataset) {
					double diff = static_cast<double>(it) - mean;
					dev += diff * diff;
				}

				dev /= static_cast<double>(size);
				return sqrt(dev);
			}
	};
#endif

#define ENVBITSIZE 64 // bit size of operational system
// check bit size of operational system
#if _WIN32 || _WIN64 || __GNUC__
	#if _WIN64 || __x86_64__ || __ppc64__
		#define ENV64BIT
	#else
		#define ENV32BIT
		#define ENVBITSIZE 32
	#endif
#endif

#define OO_ENCAPSULATE(TYPE, VAR) \
	private: \
		TYPE VAR; \
	public: \
		inline TYPE get_##VAR () \
		{ \
			return this->VAR; \
		} \
		inline void set_##VAR (TYPE VAR) \
		{ \
			this->VAR = VAR; \
		}

#define OO_ENCAPSULATE_DV(TYPE, VAR, DEFAULT_VALUE) \
	private: \
		TYPE VAR = DEFAULT_VALUE; \
	public: \
		inline TYPE get_##VAR () \
		{ \
			return this->VAR; \
		} \
		inline void set_##VAR (TYPE VAR) \
		{ \
			this->VAR = VAR; \
		}

#define OO_ENCAPSULATE_RO(TYPE, VAR) \
	private: \
		TYPE VAR; \
	public: \
		inline TYPE get_##VAR () \
		{ \
			return this->VAR; \
		}

#define OO_ENCAPSULATE_RO_DV(TYPE, VAR, DEFAULT_VALUE) \
	private: \
		TYPE VAR = DEFAULT_VALUE; \
	public: \
		inline TYPE get_##VAR () \
		{ \
			return this->VAR; \
		}

bool is_prime (uint64_t n);
uint64_t next_prime (uint64_t n);

#endif

