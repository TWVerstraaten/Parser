//
// Created by pc on 18-08-21.
//

#ifndef GEN_VARIANTTEMPLATES_H
#define GEN_VARIANTTEMPLATES_H

namespace gen {

    template <class... Ts, class Type>
    [[nodiscard]] static bool S_VARIANT_EQUALS(const std::variant<Ts...>& variant, Type type) {
        return std::holds_alternative<Type>(variant) && std::get<Type>(variant) == type;
    }

} // namespace gen

#endif // GEN_VARIANTTEMPLATES_H
