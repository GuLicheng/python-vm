// #include "Heap.hpp"

// #include <algorithm>
// #include <iostream>

// namespace python
// {
//     Space::Space(size_t size)
//     {
//         this->m_size = size;
//         this->m_base = (char*)::operator new(size);
//         this->m_end = this->m_base + this->m_size;
//         this->m_top = (char*)(((long long)(this->m_base + 15)) & -16);
//         this->m_capacity = this->m_end - this->m_top;
//     }

//     Space::~Space()
//     {
//         ::operator delete(this->m_base);
//     }

//     void* Space::allocate(size_t size)
//     {
//         size = (size + 7) & -8;
//         char* start = this->m_top;
//         this->m_top += size;
//         this->m_capacity -= size;
//         return start;
//     }

//     void Space::clear()
//     {
//         std::fill_n(this->m_base, this->m_end, 0);
//         this->m_top = (char*)(((long long)(this->m_base + 15)) & -16);
//         this->m_capacity = this->m_end - this->m_top;
//     }

//     bool Space::can_alloc(size_t size)
//     {
//         return this->m_capacity > size;
//     }

//     bool Space::has_obj(char* obj)
//     {
//         return this->m_base <= obj && obj < this->m_end;
//     }

//     Heap::Heap(size_t size)
//     {
//         this->m_mem1 = new Space(size);
//         this->m_mem2 = new Space(size);
//         this->m_metaspace = new Space(size / 16);

//         this->m_mem1->clear();
//         this->m_mem2->clear();
//         this->m_metaspace->clear();

//         this->m_eden = this->m_mem1;
//         this->m_survivor = this->m_mem2;
//     }

//     Heap::~Heap()
//     {
//         ::operator delete(this->m_mem1);
//         ::operator delete(this->m_mem2);
//         ::operator delete(this->m_metaspace);
//     }

//     void* Heap::allocate(size_t size)
//     {
//         if (this->m_eden->can_alloc(size))
//             this->gc();
//         return m_eden->allocate(size);
//     }

//     void* Heap::allocate_meta(size_t size)
//     {
//         if (!this->m_metaspace->can_alloc(size))
//             throw std::bad_alloc{};
//         return m_metaspace->allocate(size);
//     }

//     void Heap::copy_live_object()
//     {
//         ScavengeOopClosure closure(this->m_eden, this->m_survivor, this->m_metaspace);
//         closure.scavenge();
//     }

//     void Heap::gc()
//     {
//         std::cout 
//             << "gc starting... \n\tbefore gc: \n\t eden's capacity is " 
//             << this->m_eden->m_capacity
//             << "\n";

//         this->copy_live_object();

//         std::swap(this->m_eden, this->m_survivor);

//         std::cout 
//             << "\tafter gc: \n\t eden's capacity is " 
//             << this->m_eden->m_capacity
//             << "\ngc end\n";

//         this->m_survivor->clear();

//     }

// } // namespace python
