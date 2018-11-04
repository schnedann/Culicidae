#ifndef META_H
#define META_H

template<typename T> struct convctype{
  using type = T;
};

template<typename T> const T& constant(T& _) { return const_cast<const T&>(_); }
template<typename T> T& variable(const T& _) { return const_cast<T&>(_); }

template< class T > struct remove_const          { typedef T type; };
template< class T > struct remove_const<const T> { typedef T type; };

#endif // META_H
