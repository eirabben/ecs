#pragma once

#include <cstddef>
#include <tuple>
#include <utility>

namespace util {

    // Tuple loop code from here: http://codereview.stackexchange.com/a/67394

    template <typename Tuple, typename F, std::size_t ...Indices>
    void forEachImpl(Tuple&& tuple, F&& f, std::index_sequence<Indices...>) {
        using swallow = int[];
        (void)swallow{1,
            (f(std::get<Indices>(std::forward<Tuple>(tuple))), void(), int{})...
        };
    }

    template <typename Tuple, typename F>
    void forTuple(Tuple&& tuple, F&& f) {
        constexpr std::size_t N = std::tuple_size<std::remove_reference_t<Tuple>>::value;
        forEachImpl(std::forward<Tuple>(tuple), std::forward<F>(f),
                                  std::make_index_sequence<N>{});
    }

}
