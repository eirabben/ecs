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

    // Metaprogramming rename functionality

    template <template <typename...> class TNewName, typename T>
    struct RenameHelper;

    // "Renames" `TOldName<Ts...>` to `TNewName<Ts...>`.
    template <template <typename...> class TNewName,
        template <typename...> class TOldName, typename... Ts>
    struct RenameHelper<TNewName, TOldName<Ts...>>
    {
        using type = TNewName<Ts...>;
    };

    template <template <typename...> class TNewName, typename T>
    using Rename = typename RenameHelper<TNewName, T>::type;

}
