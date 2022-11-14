#include <string>
#include <cstring>
#include <cmath>
#include <cstdint>
#include <initializer_list>

namespace jm
{
    template<typename T>
    struct list_base
    {
        // typedefs
        typedef size_t size_type;

        static const size_type npos = (size_type)-1;
        static const size_type max_size = (size_type)-2;

    public:
        list_base();
        list_base(size_type size);
        ~list_base();

    protected:
        T* do_allocate(size_type size);
        void do_free(T* p, size_type size);
        size_type get_new_capacity(size_type current_capacity);

    protected:
        T* data_begin;
        T* data_end;
        T* data_true_end;
    };

    template<typename T>
    class list : public list_base<T>
    {
        typedef list_base<T>        base_type;
        typedef list<T>    this_type;

        using base_type::data_begin;
        using base_type::data_end;
        using base_type::data_true_end;

        using base_type::npos;
        using base_type::max_size;

        using base_type::do_allocate;
        using base_type::do_free;
        using base_type::get_new_capacity;

    public:
        // typedefs
        typedef T                   value_type;
        typedef T*                  pointer;
        typedef const T*            const_pointer;
        typedef T&                  reference;
        typedef const T&            const_reference;
        typedef T*                  iterator;
        typedef const T*            const_iterator;
        typedef size_t              size_type;
        
        // constructors
        list();
        list(size_type size);
        list(size_type size, const_reference value);
        list(const this_type& other);
        //list(this_type&& other);
        list(std::initializer_list<value_type> ilist);

        template <typename other_type>
        list(const other_type* begin, const other_type* end);

        ~list();

        this_type& operator=(const this_type& other);
        this_type& operator=(std::initializer_list<value_type> ilist);
        // this_type& operator=(this_type&& other);

        void swap(this_type& other);

        void assign(size_type size, const_reference value);

        template<typename other_type>
        void assign(const other_type* begin, const other_type* end);

        void assign(std::initializer_list<value_type> ilist);

        iterator begin();
        const_iterator begin() const;
        const_iterator cbegin() const;

        iterator end();
        const_iterator end() const;
        const_iterator cend() const;


        bool empty() const;
        size_type size() const;
        size_type capacity() const;

        void resize(size_type size, const_reference value);
        void resize(size_type size);
        void reserve(size_type size);

        pointer data();
        const_pointer data() const;

        reference operator[](size_type n);
        const_reference operator[](size_type n) const;

        reference at(size_type n);
        const_reference at(size_type n) const;

        reference back();
        const_reference back() const;

        void push_back(const_reference value);
        reference push_back();
        void pop_back();

        // For contribution
        // iterator insert(const_iterator position, const_reference value);
        // iterator insert(const_iterator position, size_type size, const_reference value);
        // iterator insert(const_iterator position, std::initializer_list<value_type> ilist);
        // template<typename other_type>
        // iterator insert(const_iterator position, const other_type* begin, const other_type* end);        
        // iterator erase(const_iterator position);
        // iterator erase(const_iterator begin, const_iterator end);

        void clear();
        
    protected:
            

    };

    // list_base

    template<typename T>
    inline list_base<T>::list_base()
    : data_begin(nullptr),
      data_end(nullptr),
      data_true_end(nullptr)
    {        
    }

    template<typename T>
    inline list_base<T>::list_base(size_type size)
    {
        data_begin = do_allocate(size);
        data_end = data_begin;
        data_true_end = data_begin + size;
    }

    template<typename T>
    inline list_base<T>::~list_base()
    {
        if(data_begin) do_free(data_begin, (size_type)(data_true_end - data_begin));
        data_begin = data_end = data_true_end = nullptr;
    }

    template<typename T>
    inline T* list_base<T>::do_allocate(size_type size)
    {
        if(size == 0) return nullptr;
        auto* p = new T[size];
        // if(p == nullptr) throw std::exception("Failed to allocate memory");
        return p;
    }

    template <typename T>
    inline void list_base<T>::do_free(T* p, size_type n)
    {
        if(p) free(p);
    }

    template <typename T>
    inline typename list_base<T>::size_type list_base<T>::get_new_capacity(size_type old_capacity)
    {
        return (old_capacity > 0) ? (2 * old_capacity) : 1;
    }
    
    // vector

    template<typename T>
    inline list<T>::list()
    :base_type()
    {
    }
    
    template<typename T>
    inline list<T>::list(size_type sz)
    :base_type(sz)
    {        
        // std::memset(data_begin, 0, size * sizeof(T));
        for(size_type i = 0 ; i < sz ; i++) data_begin[i] = T();
        data_end = data_begin + sz;
    }

    template <typename T>
    inline list<T>::list(size_type sz, const_reference value)
    :base_type(sz)
    {
        for(size_type i = 0 ; i < sz ; i++) data_begin[i] = value;
        data_end = data_begin + sz;
    }

    template <typename T>
    inline list<T>::list(const this_type& other)
    :base_type(other.size())
    {
        for(size_type i = 0 ; i < other.size() ; i++) data_begin[i] = other.data_begin[i];
        data_end = data_begin + other.size();
    }

    template<typename T>
    inline list<T>::list(std::initializer_list<value_type> ilist)
    :base_type(ilist.size())
    {
        //std::memcpy(data_begin, ilist.data(), ilist.size() * sizeof(T));
        for(size_type i = 0 ; i < ilist.size() ; i++) data_begin[i] = ilist[i];
        data_end = data_begin + ilist.size();
    }

    template <typename T>
    template <typename other_type>
    inline list<T>::list(const other_type* begin, const other_type* end)
    :base_type((size_type)( std::round(((double)(end - begin) * sizeof(other_type)) / sizeof(T) )))
    {
        std::memcpy(data_begin, begin, sizeof(other_type) * (size_type)(end - begin));
        data_end = data_begin + (size_type)( std::round(((double)(end - begin) * sizeof(other_type)) / sizeof(T) ));
    }

    template <typename T>
    inline list<T>::~list()
    {
        clear();
        // memory freed in parent class
    }

    template<typename T>
    inline typename list<T>::this_type& list<T>::operator=(const this_type& other)
    {
        if(this == &other) return *this;
        clear();
        if(other.size() > size_type(data_true_end - data_begin))
        {
            do_free(data_begin, (size_type)(data_true_end - data_begin));            
            data_begin = do_allocate(other.size());
            data_end = data_begin + other.size();
            data_true_end = data_end;
        }
        for(size_type i = 0 ; i < other.size() ; i++) data_begin[i] = other.data_begin[i];
        return *this;
    }

    template<typename T>
    inline typename list<T>::this_type& list<T>::operator=(std::initializer_list<value_type> other)
    {
        clear();
        if(other.size() > size_type(data_true_end - data_begin))
        {
            do_free(data_begin, (size_type)(data_true_end - data_begin));            
            data_begin = do_allocate(other.size());
            data_end = data_begin + other.size();
            data_true_end = data_end;
        }
        for(size_type i = 0 ; i < other.size() ; i++) data_begin[i] = other[i];
        return *this;
    }

    template<typename T>
    inline void list<T>::swap(this_type& other)
    {
        std::swap(data_begin, other.data_begin);
        std::swap(data_end, other.data_end);
        std::swap(data_true_end, other.data_true_end);
    }
    
    template<typename T>
    inline void list<T>::assign(size_type sz, const_reference value)
    {
        list<T> temp(sz, value);
        swap(temp);
    }

    template<typename T>
    template<typename other_type>
    inline void list<T>::assign(const other_type* st, const other_type* en)
    {
        list<T> temp(st, en);
        swap(temp);
    }

    template<typename T>
    inline void list<T>::assign(std::initializer_list<value_type> ilist)
    {
        list<T> temp(ilist);
        swap(temp);
    }

    template<typename T>
    inline typename list<T>::iterator list<T>::begin()
    {
        return data_begin;
    }

    template<typename T>
    inline typename list<T>::const_iterator list<T>::begin() const
    {
        return data_begin;
    }

    template<typename T>
    inline typename list<T>::const_iterator list<T>::cbegin() const
    {
        return data_begin;
    }

    template<typename T>
    inline typename list<T>::iterator list<T>::end()
    {
        return data_end;
    }

    template<typename T>
    inline typename list<T>::const_iterator list<T>::end() const
    {
        return data_end;
    }

    template<typename T>
    inline typename list<T>::const_iterator list<T>::cend() const
    {
        return data_end;
    }

    template<typename T>
    inline bool list<T>::empty() const
    {
        return data_begin == data_end;
    }

    template<typename T>
    inline typename list<T>::size_type list<T>::size() const
    {
        return (size_type)(data_end - data_begin);
    }
    
    template<typename T>
    inline typename list<T>::size_type list<T>::capacity() const
    {
        return (size_type)(data_true_end - data_begin);
    }
    
    template<typename T>
    inline void list<T>::resize(size_type sz, const_reference value)
    {
        if(sz < size())
        {
            for(size_type i = sz ; i < size() ; i++ ) data_begin[i].~T();
            data_end -= (size() - sz);
        }
        else if(sz > size())
        {
            pointer new_data = do_allocate(sz);
            std::memcpy(new_data, data_begin, sizeof(T) * size());
            do_free(data_begin, (size_type)(data_true_end - data_begin));
            for(size_type i = size() ; i < sz ; i++) new_data[i] = value;
            data_begin = new_data;
            data_end = data_begin + sz;
            data_true_end = data_end;
        }
    }

    template<typename T>
    inline void list<T>::resize(size_type sz)
    {
        resize(sz, T());
    }

    template<typename T>
    inline void list<T>::reserve(size_type sz)
    {
        if(sz <= capacity()) return;
        pointer new_data = do_allocate(sz);
        auto sz2 = size();
        std::memcpy(new_data, data_begin, sizeof(T) * size());
        do_free(data_begin, (size_type)(data_true_end - data_begin));
        data_begin = new_data;
        data_end = data_begin + sz2;
        data_true_end = data_begin + sz;
    }

    template<typename T>
    inline typename list<T>::pointer list<T>::data()
    {
        return data_begin;
    }

    template<typename T>
    inline typename list<T>::const_pointer list<T>::data() const
    {
        return data_begin;
    }

    template<typename T>
    inline typename list<T>::reference list<T>::operator[](size_type n)
    {
        return data_begin[n];
    }

    template<typename T>
    inline typename list<T>::const_reference list<T>::operator[](size_type n) const
    {
        return data_begin[n];
    }

    template<typename T>
    inline typename list<T>::reference list<T>::at(size_type n)
    {
        return data_begin[n];
    }

    template<typename T>
    inline typename list<T>::const_reference list<T>::at(size_type n) const
    {
        return data_begin[n];
    }

    template<typename T>
    inline typename list<T>::reference list<T>::back()
    {
        return *(data_end - 1);
    }

    template<typename T>
    inline typename list<T>::const_reference list<T>::back() const
    {
        return *(data_end - 1);
    }

    template<typename T>
    inline void list<T>::clear()
    {
        for(size_t i = 0 ; i < size() ; i++) data_begin[i].~T();
        data_end = data_begin;
    }

    template<typename T>
    inline void list<T>::push_back(const_reference value)
    {
        if(size() == capacity()) reserve(get_new_capacity(capacity()));
        *(data_end++) = value;
    }

    template<typename T>
    inline typename list<T>::reference list<T>::push_back()
    {
        push_back(T());
        return *(data_end - 1);
    }

    template<typename T>
    inline void list<T>::pop_back()
    {
        (--data_end)->~T();
        return *data_end;
    }

}
