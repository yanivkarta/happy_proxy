#ifndef __PROXY_CONCEPTS_H_
#define __PROXY_CONCEPTS_H_
 
#include <string>
#include <vector>
#include <map>
#include <set>
#include <memory>

//check cppversion
#if __cplusplus > 201703L 
#define CPP20
#else if __cplusplus == 201703L
#define CPP17
#else if __cplusplus == 201402L
#define CPP14
#else if __cplusplus == 201103L
#define CPP11
#endif

namespace happy_proxy
{
    //concept for equality comparable
    template<typename T>
    concept bool Equality_comparable() {
        return requires (T a, T b) {
            { a == b } -> bool;
            { a != b } -> bool;
        };
    }
    //concept for range
    template<typename R>
    concept bool Range() {
        return requires (R range) {
            begin(range);
            end(range);
        };
    }
    //concept for range
    template<typename R>
    concept bool Range() {
        return requires (R range) {
            typename Value_type<R>;    // Must have a 
                                    // value type.
            typename Iterator_type<R>; // Must have an
                                    // iterator type.
            { begin(range) } -> Iterator_type<R>;
            { end(range) } -> Iterator_type<R>;
            // The iterator type must really be an 
            // input iterator.
            requires Input_iterator<Iterator_type<R>>();
            // The value of R is the same as its
            // iterator's value type.
            requires Same<Value_type<R>,
                    Value_type<Iterator_type<R>>>().
        };//concept for range
    }//concept for range
}//namespace happy_proxy
#endif