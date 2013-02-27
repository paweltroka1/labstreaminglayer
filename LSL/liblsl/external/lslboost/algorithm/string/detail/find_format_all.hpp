//  Boost string_algo library find_format_all.hpp header file  ---------------------------//

//  Copyright Pavol Droba 2002-2003.
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.lslboost.org/LICENSE_1_0.txt)

//  See http://www.lslboost.org/ for updates, documentation, and revision history.

#ifndef BOOST_STRING_FIND_FORMAT_ALL_DETAIL_HPP
#define BOOST_STRING_FIND_FORMAT_ALL_DETAIL_HPP

#include <lslboost/algorithm/string/config.hpp>
#include <lslboost/range/iterator_range.hpp>
#include <lslboost/range/const_iterator.hpp>
#include <lslboost/range/value_type.hpp>
#include <lslboost/algorithm/string/detail/find_format_store.hpp>
#include <lslboost/algorithm/string/detail/replace_storage.hpp>

namespace lslboost {
    namespace algorithm {
        namespace detail {

// find_format_all_copy (iterator variant) implementation ---------------------------//

           template< 
                typename OutputIteratorT,
                typename InputT,
                typename FinderT,
                typename FormatterT,
                typename FindResultT,
                typename FormatResultT >
            inline OutputIteratorT find_format_all_copy_impl2(
                OutputIteratorT Output,
                const InputT& Input,
                FinderT Finder,
                FormatterT Formatter,
                const FindResultT& FindResult,
                const FormatResultT& FormatResult )
            {       
                typedef BOOST_STRING_TYPENAME 
                    range_const_iterator<InputT>::type input_iterator_type; 

                typedef find_format_store<
                        input_iterator_type, 
                        FormatterT,
                        FormatResultT > store_type;

                // Create store for the find result
                store_type M( FindResult, FormatResult, Formatter );

                // Initialize last match
                input_iterator_type LastMatch=::lslboost::begin(Input);

                // Iterate through all matches
                while( M )
                {
                    // Copy the beginning of the sequence
                    Output = std::copy( LastMatch, M.begin(), Output );
                    // Copy formated result
                    Output = std::copy( ::lslboost::begin(M.format_result()), ::lslboost::end(M.format_result()), Output );

                    // Proceed to the next match
                    LastMatch=M.end();
                    M=Finder( LastMatch, ::lslboost::end(Input) );
                }

                // Copy the rest of the sequence
                Output = std::copy( LastMatch, ::lslboost::end(Input), Output );

                return Output;
            }

            template< 
                typename OutputIteratorT,
                typename InputT,
                typename FinderT,
                typename FormatterT,
                typename FindResultT >
            inline OutputIteratorT find_format_all_copy_impl(
                OutputIteratorT Output,
                const InputT& Input,
                FinderT Finder,
                FormatterT Formatter,
                const FindResultT& FindResult )
            {   
                if( ::lslboost::algorithm::detail::check_find_result(Input, FindResult) ) {
                    return ::lslboost::algorithm::detail::find_format_all_copy_impl2( 
                        Output,
                        Input,
                        Finder,
                        Formatter,
                        FindResult,
                        Formatter(FindResult) );
                } else {
                    return std::copy( ::lslboost::begin(Input), ::lslboost::end(Input), Output );
                }
            }

 // find_format_all_copy implementation ----------------------------------------------//

           template< 
                typename InputT, 
                typename FinderT,
                typename FormatterT,
                typename FindResultT,
                typename FormatResultT >
            inline InputT find_format_all_copy_impl2(
                const InputT& Input,
                FinderT Finder,
                FormatterT Formatter,
                const FindResultT& FindResult,
                const FormatResultT& FormatResult)
            {
                typedef BOOST_STRING_TYPENAME 
                    range_const_iterator<InputT>::type input_iterator_type; 

                typedef find_format_store<
                        input_iterator_type, 
                        FormatterT,
                        FormatResultT > store_type;

                // Create store for the find result
                store_type M( FindResult, FormatResult, Formatter );

                // Initialize last match
                input_iterator_type LastMatch=::lslboost::begin(Input);

                // Output temporary
                InputT Output;

                // Iterate through all matches
                while( M )
                {
                    // Copy the beginning of the sequence
                    insert( Output, ::lslboost::end(Output), LastMatch, M.begin() );
                    // Copy formated result
                    insert( Output, ::lslboost::end(Output), M.format_result() );

                    // Proceed to the next match
                    LastMatch=M.end();
                    M=Finder( LastMatch, ::lslboost::end(Input) );
                }

                // Copy the rest of the sequence
                ::lslboost::algorithm::detail::insert( Output, ::lslboost::end(Output), LastMatch, ::lslboost::end(Input) );

                return Output;
            }

            template< 
                typename InputT, 
                typename FinderT,
                typename FormatterT,
                typename FindResultT >
            inline InputT find_format_all_copy_impl(
                const InputT& Input,
                FinderT Finder,
                FormatterT Formatter,
                const FindResultT& FindResult)
            {
                if( ::lslboost::algorithm::detail::check_find_result(Input, FindResult) ) {
                    return ::lslboost::algorithm::detail::find_format_all_copy_impl2(
                        Input,
                        Finder,
                        Formatter,
                        FindResult,
                        Formatter(FindResult) );
                } else {
                    return Input;
                }
            }

 // find_format_all implementation ------------------------------------------------//
        
            template<
                typename InputT,
                typename FinderT,
                typename FormatterT,
                typename FindResultT,
                typename FormatResultT >
            inline void find_format_all_impl2( 
                InputT& Input,
                FinderT Finder,
                FormatterT Formatter,
                FindResultT FindResult,
                FormatResultT FormatResult)
            {
                typedef BOOST_STRING_TYPENAME 
                    range_iterator<InputT>::type input_iterator_type; 
                typedef find_format_store<
                        input_iterator_type, 
                        FormatterT,
                        FormatResultT > store_type;

                // Create store for the find result
                store_type M( FindResult, FormatResult, Formatter );
          
                // Instantiate replacement storage
                std::deque<
                    BOOST_STRING_TYPENAME range_value<InputT>::type> Storage;

                // Initialize replacement iterators
                input_iterator_type InsertIt=::lslboost::begin(Input);
                input_iterator_type SearchIt=::lslboost::begin(Input);
                
                while( M )
                {
                    // process the segment
                    InsertIt=process_segment( 
                        Storage,
                        Input,
                        InsertIt,
                        SearchIt,
                        M.begin() );
                    
                    // Adjust search iterator
                    SearchIt=M.end();

                    // Copy formated replace to the storage
                    ::lslboost::algorithm::detail::copy_to_storage( Storage, M.format_result() );

                    // Find range for a next match
                    M=Finder( SearchIt, ::lslboost::end(Input) );
                }

                // process the last segment
                InsertIt=::lslboost::algorithm::detail::process_segment( 
                    Storage,
                    Input,
                    InsertIt,
                    SearchIt,
                    ::lslboost::end(Input) );
                
                if ( Storage.empty() )
                {
                    // Truncate input
                    ::lslboost::algorithm::detail::erase( Input, InsertIt, ::lslboost::end(Input) );
                }
                else
                {
                    // Copy remaining data to the end of input
                    ::lslboost::algorithm::detail::insert( Input, ::lslboost::end(Input), Storage.begin(), Storage.end() );
                }
            }

            template<
                typename InputT,
                typename FinderT,
                typename FormatterT,
                typename FindResultT >
            inline void find_format_all_impl( 
                InputT& Input,
                FinderT Finder,
                FormatterT Formatter,
                FindResultT FindResult)
            {
                if( ::lslboost::algorithm::detail::check_find_result(Input, FindResult) ) {
                    ::lslboost::algorithm::detail::find_format_all_impl2(
                        Input,
                        Finder,
                        Formatter,
                        FindResult,
                        Formatter(FindResult) );
                }
            }

        } // namespace detail
    } // namespace algorithm
} // namespace lslboost

#endif  // BOOST_STRING_FIND_FORMAT_ALL_DETAIL_HPP
