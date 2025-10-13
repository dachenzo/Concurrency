#pragma once
#include <cstddef>
#include <array>
#include <atomic>
#include <optional>



namespace Cx {

    template <typename T, std::size_t S>
    requires(S >= 2)
    struct SpscRing {

        SpscRing() = default;
        SpscRing(SpscRing& other) = delete;
        SpscRing& operator=(SpscRing& other) = delete;
        SpscRing(SpscRing&& other) = delete;
        SpscRing& operator=(SpscRing&& other) = delete;

        bool write(T& obj) {
            std::size_t head = head_.load(std::memory_order_relaxed);
            std::size_t next = (head+1)%S;
            std::size_t tail = tail_.load(std::memory_order_acquire);
            if (next == tail) return false;
            buff_[head] = obj;
            head_.store(next, std::memory_order_release);
            return true;
        }

        
        bool write(T&& obj) {
            std::size_t head = head_.load(std::memory_order_relaxed);
            std::size_t next = (head+1)%S;
            std::size_t tail = tail_.load(std::memory_order_acquire);
            if (next == tail) return false;
            buff_[head] = std::move(obj);
            head_.store(next, std::memory_order_release);
            return true;
        }

        
        bool write(const T& obj) {
            std::size_t head = head_.load(std::memory_order_relaxed);
            std::size_t next = (head+1)%S;
            std::size_t tail = tail_.load(std::memory_order_acquire);
            if (next == tail) return false;
            buff_[head] = obj;
            head_.store(next, std::memory_order_release);
            return true;
        }
        template<typename ...Args>
        bool emplace(Args&&... args) {
            std::size_t head = head_.load(std::memory_order_relaxed);
            std::size_t next = (head+1)%S;
            std::size_t tail = tail_.load(std::memory_order_acquire);
            if (next == tail) return false;
            buff_[head] = T(std::forward<Args>(args)...);
            head_.store(next, std::memory_order_release);
            return true;
        }


        std::optional<T> read() {            
            std::size_t tail = tail_.load(std::memory_order_relaxed);
            std::size_t head = head_.load(std::memory_order_acquire);
            if (head == tail) return {};
            std::size_t next = (tail+1)%S;
            auto value = buff_[tail];
            tail_.store(next, std::memory_order_release);
            return std::move(value);

        }

        
        private:
        std::array<T, S> buff_;
        std::atomic<std::size_t> head_{0};
        std::atomic<std::size_t> tail_{0};
        std::size_t cap_{S};
        

       
    };
}