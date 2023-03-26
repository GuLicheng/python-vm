// #pragma once

// #include <vector>
// #include <algorithm>

// #include "Python.hpp"

// namespace python
// {
//     template <typename T>
//     class ArrayList
//     {
//         std::vector<T> arr;

//         //void expand();

//     public:

//         ArrayList(int capacity = 0);

//         void add(T t);
        
//         void insert(int index, T t);

//         T get(int index);

//         void set(int index, T t);

//         int size();

//         int length();

//         int index(T t);

//         T* value();

//         T pop();

//         //void delete_index(int index);

//         //void* operator new(size_t size);

//         //void oops_do(OopClosure* closure);
//     };

//     //using ObjList = ArrayList<class Object>;

//     template<typename T>
//     ArrayList<T>::ArrayList(int capacity)
//     {
//         this->arr.reserve(capacity);
//     }

//     template<typename T>
//     void ArrayList<T>::add(T t)
//     {
//         this->arr.emplace_back(t);
//     }

//     template<typename T>
//     void ArrayList<T>::insert(int index, T t)
//     {
//         this->arr.insert(arr.begin() + index, t);
//     }

//     template<typename T>
//     T ArrayList<T>::get(int index)
//     {
//         if (index < 0)
//             index += this->arr.size();
//         PYTHON_ASSERT(index < (int)(this->arr.size()));
//         return this->arr[index];
//     }

//     template<typename T>
//     void ArrayList<T>::set(int index, T t)
//     {
//         this->arr.at(index) = t;
//     }

//     template<typename T>
//     int ArrayList<T>::size()
//     {
//         return this->arr.size();
//     }

//     template<typename T>
//     int ArrayList<T>::length()
//     {
//         return this->arr.capacity();
//     }

//     template<typename T>
//     int ArrayList<T>::index(T t)
//     {
//         auto dist = std::ranges::distance(
//             std::ranges::find(this->arr, t),
//             this->arr.end()
//         );
//         // auto dist = std::distance(
//         //     std::find(this->arr.begin(), this->arr.end(), t),
//         //     this->arr.end()
//         // );
//         return dist == (int)this->arr.size() ? -1 : dist;
//     }

//     template<typename T>
//     T* ArrayList<T>::value()
//     {
//         return this->arr.data();
//     }

//     template<typename T>
//     T ArrayList<T>::pop()
//     {
//         PYTHON_ASSERT(this->arr.size());
//         auto res = this->arr.back();
//         this->arr.pop_back();
//         return res;
//     }

//     using ObjectList = ArrayList<class Object*>;

// }
