/*
 * Topic: BitPack
 * Description: Bit Packing compresses multiple small values into a single larger integer by
 *   allocating specific bit ranges to each value. Used to optimize memory and speed
 *   up state representations in DP.
 * minimizing memory overhead. Usage: BitPack<4, uint64_t> b = {1, 2, 3, 4}; b.pack(0, 5); cout << b.unpack(0);
 */
#include "../core.h"

template <size_t N, typename T = uint64_t>
struct BitPack {
    static constexpr size_t BitsPerValue = (sizeof(T) * 8) / N;
    static_assert(N * BitsPerValue == sizeof(T) * 8);
    T data = 0;

    static constexpr T mask() { return (T(1) << BitsPerValue) - 1; }

    BitPack() = default;

    BitPack(initializer_list<T> lst) {
        data = 0;
        size_t i = 0;
        for (T v : lst) {
            if (i >= N) break;
            pack(i++, v);
        }
    }

    void pack(size_t idx, T value) {
        data &= ~(mask() << (idx * BitsPerValue));
        data |= ((value & mask()) << (idx * BitsPerValue));
    }

    T unpack(size_t idx) const { return (data >> (idx * BitsPerValue)) & mask(); }

    T operator[](size_t idx) const { return unpack(idx); }

    BitPack& operator=(initializer_list<T> lst) {
        data = 0;
        size_t i = 0;
        for (T v : lst) {
            if (i >= N) break;
            pack(i++, v);
        }
        return *this;
    }

    bool operator==(const BitPack& o) const { return data == o.data; }

    bool operator!=(const BitPack& o) const { return data != o.data; }

    bool operator<(const BitPack& o) const {
        for (size_t i = 0; i < N; i++) {
            T a = unpack(i), b = o.unpack(i);
            if (a != b) return a < b;
        }
        return false;
    }

    bool operator>(const BitPack& o) const { return o < *this; }

    bool operator<=(const BitPack& o) const { return !(o < *this); }

    bool operator>=(const BitPack& o) const { return !(*this < o); }

    friend ostream& operator<<(ostream& out, const BitPack& p) {
        for (size_t i = 0; i < N; i++) {
            if (i) out << ",";
            out << p.unpack(i);
        }
        return out;
    }

    friend istream& operator>>(istream& in, BitPack& p) {
        for (size_t i = 0; i < N; i++) {
            T v;
            in >> v;
            p.pack(i, v);
        }
        return in;
    }

    operator T() const { return data; }
};

template <size_t N, typename T>
struct tuple_size<BitPack<N, T>> : integral_constant<size_t, N> {};

template <size_t I, size_t N, typename T>
struct tuple_element<I, BitPack<N, T>> {
    using type = T;
};

template <size_t N, typename T>
struct hash<BitPack<N, T>> {
    size_t operator()(const BitPack<N, T>& p) const noexcept { return hash<T>()((T)p); }
};

template <size_t I, size_t N, typename T>
T get(const BitPack<N, T>& p) {
    static_assert(I < N);
    return p.unpack(I);
}
