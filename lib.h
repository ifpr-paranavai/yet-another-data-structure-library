#ifndef __LIB__
#define __LIB__

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

