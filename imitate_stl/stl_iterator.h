#ifndef STL_ITERATOR_H
#define STL_ITERATOR_H

struct input_iterator_tag{};
struct output_iterator_tag{};
struct forward_iterator_tag:public input_iterator_tag{};
struct bidirectional_iterator_tag:public forward_iterator_tag{};
struct random_iterator_tag:public bidirectional_iterator_tag{};

//traits 手法以后再写
//@2016/12/1
};

#endif
