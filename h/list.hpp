//
// Created by marko on 20.4.22..
//

#ifndef OS1_VEZBE07_RISCV_CONTEXT_SWITCH_2_INTERRUPT_LIST_HPP
#define OS1_VEZBE07_RISCV_CONTEXT_SWITCH_2_INTERRUPT_LIST_HPP

#include "../lib/mem.h"

template<typename T>
class List
{
private:
    uint64  count;
protected:
    struct Elem
    {
        T *data;
        Elem *next;

        Elem(T *data, Elem *next) : data(data), next(next) {}

        void *operator new(size_t size) { return __mem_alloc(size); }
        void operator delete(void *ptr) { __mem_free(ptr); }
    };

    void insertAfter(Elem *node, T *data)
    {
        if (!node) return;
        Elem *next = node->next;
        node->next = new Elem(data, next);
        count++;
    }

    uint64 size() {return count;}

    Elem *head, *tail;

public:
    List() : head(0), tail(0) {}

    List(const List<T> &) = delete;

    List<T> &operator=(const List<T> &) = delete;

    void addFirst(T *data)
    {
        Elem *elem = new Elem(data, head);
        head = elem;
        if (!tail) { tail = head; }
        count++;
    }

    void addLast(T *data)
    {
        Elem *elem = new Elem(data, 0);
        if (tail)
        {
            tail->next = elem;
            tail = elem;
        } else
        {
            head = tail = elem;
        }
        count++;
    }

    T *removeFirst()
    {
        if (!head) { return 0; }

        Elem *elem = head;
        head = head->next;
        if (!head) { tail = 0; }

        T *ret = elem->data;
        delete elem;
        count--;
        return ret;
    }

    T *peekFirst()
    {
        if (!head) { return 0; }
        return head->data;
    }

    T *removeLast()
    {
        if (!head) { return 0; }

        Elem *prev = 0;
        for (Elem *curr = head; curr && curr != tail; curr = curr->next)
        {
            prev = curr;
        }

        Elem *elem = tail;
        if (prev) { prev->next = 0; }
        else { head = 0; }
        tail = prev;

        T *ret = elem->data;
        delete elem;
        count--;
        return ret;
    }

    T *peekLast()
    {
        if (!tail) { return 0; }
        return tail->data;
    }
};

#endif //OS1_VEZBE07_RISCV_CONTEXT_SWITCH_2_INTERRUPT_LIST_HPP