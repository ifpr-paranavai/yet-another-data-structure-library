#ifndef __LIB__
#define __LIB__

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

#endif

