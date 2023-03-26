// #pragma once

// #include "../objects/Singleton.hpp"
// #include <stdint.h>


// namespace python
// {
//     /*
//       base                      top                            end
//         |                        |                              |
//         |  <-------used------>   |  <--------capacity---------> |
//         | <---------------------size--------------------------> |
                        
//     */
//     class Space
//     {
//         friend class Heap;

//         char* m_base;
//         char* m_top;
//         char* m_end;
//         size_t m_size;
//         size_t m_capacity;

//     public:

//         Space(size_t size);

//         ~Space();

//         void* allocate(size_t size);

//         void clear();

//         bool can_alloc(size_t size);

//         bool has_obj(char* obj);
//     };

//     class Heap : public Singleton<Heap>
//     {
//         constexpr static size_t MAX_CAPACITY = 2 * 1024 * 1024;

//         Space* m_mem1;

//         Space* m_mem2;

//         Space* m_eden;

//         Space* m_survivor;

//         Space* m_metaspace;

//     public:

//         Heap(size_t size = MAX_CAPACITY);

//         ~Heap();

//         void* allocate(size_t size);

//         void* allocate_meta(size_t size);

//         void copy_live_object();

//         void gc();
//     };


// } // namespace python
