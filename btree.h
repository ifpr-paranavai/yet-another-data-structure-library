#ifndef __YADSL_BTREE__
#define __YADSL_BTREE__

namespace yadsl
{
	template <typename T>
	class btree_t
	{
		using value_type = T;

		public:
			struct node_t
			{
				value_type value;
				node_t *left = nullptr;
				node_t *right = nullptr;

				node_t (const value_type& val)
				{
					this->value = val;
				}
			}

		private:
			node_t *root = nullptr;
	};
}

#endif
