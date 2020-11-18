#pragma once

#include <list>
#include <functional>

namespace erd //Easy R&D Library
{
     /**
     * <summary>
     * Contains the actual implementation of the keys_tuple template.
     * </summary>
     *
     * <remarks>
     * Stored as a recursive inheritance hierarchy from the first element (most
     * derived class) to the last (least derived class). The keys_tuple Key
     * is a const char* object which must exist in compilation time
     * and will be stored as point in the hierarchy; we use it to implement
     * a constant-time get() operation.
     * Using:
     * <code>
     * static const char KEY_HELLO[] = "keyHello";
     * static const char KEY_INT  [] = "keyInt";
     *
     *          erd::keys_tuple<erd::keys_t<KEY_HELLO, KEY_INT>, erd::types_t<std::string, int>> foo("string", 42);
     *
     *          foo.set<KEY_HELLO>("Hello world!!!");
     *          foo.set<KEY_INT>(43);
     *
     *  *******************
     *
     * std::cout<<foo.get<KEY_HELLO>()<<std::endl;
     * std::cout<<"The Answer to the Ultimate Question of Life, the Universe, and Everything is "<<foo.get<KEY_INT>()<<std::endl;
     * </code>
     * </remarks>
     */
    template <typename Keys, typename Types> struct keys_tuple;


    ///Object to store Keys. No implementation is needed
    template<const char* ...>struct keys_t;


    ///Object to store Types. No implementation is needed
    template<typename ...>struct types_t;

    namespace erd_impl
    {
        /**
        * <summary>
        * Defenition of keys_tuple_impl.
        * </summary>
        */
        template <typename Keys, typename Types> struct keys_tuple_impl;


       /**
       * <summary>
       * Defenition of access_helper stateless object for keys_tuple.
       * </summary>
       *
       * <remarks>
       * Creating a recursive access_helper chain to keys_tuple by using their const char* key.
       * To access to each element will be created recursion of access_helpers until that Key.
       * However everything will be done in compilation time and for run time
       * it will be only direct access.
       * For using access_helper need to use set/get and so on functions
       * </remarks>
       */
        template <const char* Key, typename Keys, typename Types>struct access_helper;


       /**
       * <summary>
       * Recursive access_helper implementation.
       * </summary>
       *
       * <remarks>
       * Here will be created access_helper chain until the element with needed key.
       * Otherwise it will be static assert.
       * </remarks>
       */
        template <const char* Key, const char* First_k, const char* ... Others_k, typename First_t, typename ... Others_t>
        struct access_helper<Key, keys_t<First_k, Others_k ...>, types_t<First_t, Others_t ...>>
        {
            /// <summary>
            /// Fetching keys_tuple_impl type for middle recursion <Key, keys_t<Others_k ...>, types_t<Others_t ...>>
            /// </summary>
            using value_type = typename access_helper<Key, keys_t<Others_k ...>, types_t<Others_t ...>>::value_type;

            using keys_tuple_type = keys_tuple_impl<keys_t<Others_k ...>, types_t<Others_t ...>>;

            using nextaccess_helperType = access_helper<Key, keys_t<Others_k ...>, types_t<Others_t ...>>;

            /// <summary>
            /// Fetching keys_tuple_impl value function of recursion specialization of access_helper
            /// </summary>
            static inline const value_type& get(const keys_tuple_type& element) noexcept
            {
                return nextaccess_helperType::get(element);
            }

            /// <summary>
            /// Set keys_tuple_impl value function of recursion specialization of access_helper
            /// </summary>
            static inline void set(const value_type& value, keys_tuple_type& element)
            {
                nextaccess_helperType::set(value, element);
            }
        };

        /// <summary>
        /// Closing access_helper recursion specialization for success case
        /// </summary>
        template <const char* Key, const char* ... Others_k, typename First_t, typename ... Others_t>
        struct access_helper<Key, keys_t<Key, Others_k ...>, types_t<First_t, Others_t ...>>
        {
            /// <summary>
            /// Fetching keys_tuple_impl type for success case closing recursion
            /// </summary>
            using value_type =  typename keys_tuple_impl<keys_t<Key, Others_k ...>, types_t<First_t, Others_t ...>>::value_type;

            using keys_tuple_type = keys_tuple_impl<keys_t<Key, Others_k ...>, types_t<First_t, Others_t ...>>;

            /// <summary>
            /// Fetching keys_tuple_impl value function of success case closing recursion
            /// </summary>
            static inline const value_type& get(const keys_tuple_type& element) noexcept
            {
                return element.m_value;
            }

            /// <summary>
            /// Set keys_tuple_impl type for success case closing recursion
            /// </summary>
            static inline void set(const value_type& value, keys_tuple_type& element)
            {
                element.m_value = value;
            }
        };


        template <const char* First_k, const char* ... Others_k, typename First_t, typename ... Others_t>
        struct keys_tuple_impl<keys_t<First_k, Others_k ...>, types_t<First_t, Others_t ...>>
        : keys_tuple_impl<keys_t<Others_k ...>, types_t<Others_t ...>>
        {
            using value_type = First_t;

            keys_tuple_impl(const First_t& firstVal, const Others_t& ... othersVal)
                :keys_tuple_impl<keys_t<Others_k ...>, types_t<Others_t ...>>(othersVal ... ),
                m_value(firstVal),
                m_defVal(firstVal)
                {}


            value_type m_value;
            value_type m_defVal;
            const char* m_key = First_k;
        };

       /**
       * <summary>
       * closing recursion and  itself
       * </summary>
       */
        template <>
        struct keys_tuple_impl<keys_t<>, types_t<>>
        {
            keys_tuple_impl(){}
        };
    }


    /**
    * <summary>
    * Contains the actual implementation of the keys_tuple template.
    * </summary>
    *
    * <remarks>
    * Stored as a recursive inheritance hierarchy from the first element (most
    * derived class) to the last (least derived class). The keys_tuple Key
    * is a const char* object which must exist in compilation time
    * and will be stored as point in the hierarchy; we use it to implement
    * a constant-time get() operation.
    * </remarks>
    */
    template <const char* ... Others_k, typename ... Others_t>
    struct keys_tuple<keys_t<Others_k ...>, types_t<Others_t ...>>
    : public erd_impl::keys_tuple_impl<keys_t<Others_k ...>, types_t<Others_t ...>>
    {
        static_assert(sizeof...(Others_k) == sizeof...(Others_t), "Ammount of keys and types is not equal!!!");
		
        using this_type = erd_impl::keys_tuple_impl<keys_t<Others_k ...>, types_t<Others_t ...>> ;

        template<const char* Key>
        using access_helper_t = erd_impl::access_helper<Key, keys_t<Others_k ...>, types_t<Others_t ...>>;

        keys_tuple(const Others_t& ... othersVal)
            :erd_impl::keys_tuple_impl<keys_t<Others_k ...>, types_t<Others_t ...>>(othersVal ... )
        {

        }

        keys_tuple()
            :erd_impl::keys_tuple_impl<keys_t<Others_k ...>, types_t<Others_t ...>>(Others_t() ...)
        {
        }

			
			
       /**
	   * <summary>
	   * Element value getter.
	   * </summary>
	   *
	   * <remarks>
	   * Implementation of element getter.
	   * Using:
	   * <code>
       * static const char KEY_HELLO[] = "keyHello";
       * static const char KEY_INT  [] = "keyInt";
       *
       *          erd::keys_tuple<erd::keys_t<KEY_HELLO, KEY_INT>, erd::types_t<std::string, int>> foo("string", 42);
       *
       *          foo.set<KEY_HELLO>("Hello world!!!");
       *          foo.set<KEY_INT>(43);
       *
       *  *******************
       *
       * std::cout<<foo.get<KEY_HELLO>()<<std::endl;
       * std::cout<<"The Answer to the Ultimate Question of Life, the Universe, and Everything is "<<foo.get<KEY_INT>()<<std::endl;
       * std::cout<<"The Answer to the Ultimate Question of Life, the Universe, and Everything is "<<foo.get<KEY_INT>()<<std::endl;
       * </code>
	   * </remarks>
	   */
        template <const char* Key>
        const typename access_helper_t<Key>::value_type& get() const noexcept
		{
            return access_helper_t<Key>::get(*this);
		}



        /**
        * <summary>
        * Element value setter.
        * </summary>
        *
        * <remarks>
        * Implementation of element setter.
        * Using:
        * <code>
        * static const char KEY_HELLO[] = "keyHello";
        * static const char KEY_INT  [] = "keyInt";
        *
        *          erd::keys_tuple<erd::keys_t<KEY_HELLO, KEY_INT>, erd::types_t<std::string, int>> foo("string", 42);
        *
        *          foo.set<KEY_HELLO>("Hello world!!!");
        *          foo.set<KEY_INT>(43);
        *
        *  *******************
        *
        * std::cout<<foo.get<KEY_HELLO>()<<std::endl;
        * std::cout<<"The Answer to the Ultimate Question of Life, the Universe, and Everything is "<<foo.get<KEY_INT>()<<std::endl;
        * </code>
        * </remarks>
        */
        template <const char* Key>
        void set(const typename access_helper_t<Key>::value_type& value)
		{
            access_helper_t<Key>::set(value, *this);
		}
    };

};//namespace erd
