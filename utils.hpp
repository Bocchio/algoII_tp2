#ifndef UTILS_HPP_
#define UTILS_HPP_

// Some functional programming utilities
// Container, function pointer and iterator
template <typename T, typename F>
void map(T& object, F f) {
    for (typename T::iterator it = object.begin(); it != object.end(); ++it)
        f(*it);
}


#endif  // UTILS_HPP_
