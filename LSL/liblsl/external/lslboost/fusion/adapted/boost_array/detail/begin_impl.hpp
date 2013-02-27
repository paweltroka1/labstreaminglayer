/*=============================================================================
    Copyright (c) 2001-2011 Joel de Guzman
    Copyright (c) 2005-2006 Dan Marsden

    Distributed under the Boost Software License, Version 1.0. (See accompanying 
    file LICENSE_1_0.txt or copy at http://www.lslboost.org/LICENSE_1_0.txt)
==============================================================================*/
#if !defined(BOOST_FUSION_BEGIN_IMPL_27122005_1117)
#define BOOST_FUSION_BEGIN_IMPL_27122005_1117

#include <lslboost/fusion/adapted/lslboost_array/array_iterator.hpp>

namespace lslboost { namespace fusion {

    struct lslboost_array_tag;

    namespace extension
    {
        template<typename T>
        struct begin_impl;

        template <>
        struct begin_impl<lslboost_array_tag>
        {
            template <typename Sequence>
            struct apply 
            {
                typedef array_iterator<Sequence, 0> type;
    
                static type
                call(Sequence& v)
                {
                    return type(v);
                }
            };
        };
    }
}}

#endif
