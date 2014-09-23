/*
 * range_iterator.hpp file
 * 
 * Copyright (C) 2014 Lukas Hermanns
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */

#ifndef _CPPLIBEXT_RANGE_ITERATOR_H_
#define _CPPLIBEXT_RANGE_ITERATOR_H_


#include <stdexcept>


/**
Range iterator class.
This iterator keeps track of its range ('begin' and 'end') and supports additional exception handling in debug mode.
\tparam Container Specifies the container class type. This could be std::vector, std::array, std::string, multi_array etc.
\code
// For a simple for-each loop, this class has no advantage over the classic iterators:
for (range_iterator<decltype(container)> it { container }; !it.reached_end(); ++it)
{
    // ...
}

// Passing iterators to functions, it is more handy than passing standard iterator, 'begin' and 'end' range iterators:
void ParseArguments(const_range_iterator<std::vector<std::string>> it)
{
    for (; !it.reached_end(); ++it)
    {
        if (*it == "option")
        {
            // Expect "flag" after "option"
            if (it.has_next())
            {
                ++it;
                if (*it == "flag")
                {
                    // ...
                    continue;
                }
            }
            Error("Expected 'flag' after 'option'");
        }
    }
}
\endcode
\todo This is still in progress
*/
template <class Container> class range_iterator
{
    
    public:

        /* --- Extended types --- */
        typedef typename Container::iterator                iterator_type;

        /* --- STL iterator types --- */
        typedef typename iterator_type::iterator_category   iterator_category;
        typedef typename iterator_type::value_type          value_type;
        typedef typename iterator_type::difference_type     difference_type;
        typedef typename iterator_type::pointer             pointer;
        typedef typename iterator_type::reference           reference;

        range_iterator(Container& container) :
            begin_  ( container.begin() ),
            end_    ( container.end()   ),
            it_     ( begin_            )
        {
        }
        range_iterator(iterator_type begin, iterator_type end) :
            begin_  ( begin  ),
            end_    ( end    ),
            it_     ( begin_ )
        {
        }

        range_iterator<Container>& operator ++ ()
        {
            #ifdef _DEBUG
            if (reached_end())
                throw std::out_of_range("range_iterator::operator ++ out of range");
            #endif
            ++it_;
            return *this;
        }

        range_iterator<Container> operator ++ (int)
        {
            auto tmp(*this);
            operator ++ ();
            return tmp;
        }

        range_iterator<Container>& operator -- ()
        {
            #ifdef _DEBUG
            if (reached_begin())
                throw std::out_of_range("range_iterator::operator -- out of range");
            #endif
            --it_;
            return *this;
        }

        range_iterator<Container> operator -- (int)
        {
            auto tmp(*this);
            operator -- ();
            return tmp;
        }

        //! Returns true if the iterator has reached the end (it == container.end()).
        bool reached_end() const
        {
            return it_ == end_;
        }

        //! Returns true if the iterator has reached the begin (it == container.begin()).
        bool reached_begin() const
        {
            return it_ == begin_;
        }

        //! Returns true if this iterator has a next element.
        bool has_next() const
        {
            if (!reached_end())
            {
                auto next = it;
                ++next;
                return next != end_;
            }
            return false;
        }

        //! Returns true if this iterator has a previous element.
        bool has_prev() const
        {
            return !reached_begin();
        }

        //! Returns a reference to the element, this iterator points to.
        reference operator * ()
        {
            #ifdef _DEBUG
            if (it_ == end_)
                throw std::out_of_range("range_iterator::operator * out of range");
            #endif
            return *it_;
        }

        //! Returns a pointer of the element, this iterator points to.
        pointer operator -> ()
        {
            #ifdef _DEBUG
            if (it_ == end_)
                throw std::out_of_range("range_iterator::operator * out of range");
            #endif
            return it_.operator -> ();
        }

        //! Returns the begin iterator of the range.
        iterator_type begin() const
        {
            return begin_;
        }

        //! Returns the end iterator of the range.
        iterator_type end() const
        {
            return end_;
        }

    private:
        
        iterator_type begin_, end_, it_;

};


#endif


