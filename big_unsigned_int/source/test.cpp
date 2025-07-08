#include <cassert>

#include "big_uint.hpp"

namespace big_uint {
std::string test(std::vector<uint64_t> digits) {
    // Выберем делитель – большую степень 10.
    // Значение 10⁹ даёт блоки длиной до 9 цифр.
    const uint64_t base = 1000000000ULL;

    // Перенос (carry), исходно 0.
    uint64_t carry = 0;

    // Будем накапливать блоки результата (каждый блок – число, меньше base)
    std::vector<uint64_t> blocks;
    blocks.reserve(digits.size());

    // Обрабатываем каждый элемент вектора, начиная с наименее значащего.
    // Важно: перенос (carry) каждого элемента добавляется к следующему.
    for (size_t i = 0; i < digits.size(); ++i) {
        // Суммируем текущий элемент с переносом из предыдущего.
        uint64_t current = digits[i] + carry;
        // Вычисляем остаток как текущий блок.
        uint64_t remainder = current % base;
        // Вычисляем перенос для следующего элемента.
        carry = current / base;
        blocks.push_back(remainder);
    }
    // Если после обработки всех элементов перенос не нулевой,
    // добавляем его в качестве еще одного блока.
    if (carry > 0) {
        blocks.push_back(carry);
    }

    // Формируем итоговую строку.
    // Так как наши блоки получены в порядке от наименее значимого к наиболее значимому,
    // для вывода числа в правильном виде их нужно объединить в обратном порядке.
    // Наиболее значимый блок выводим как есть (без дополнения нулями),
    // остальные дополняем ведущими нулями до 9 цифр.
    std::string result = std::to_string(blocks.back());
    // Обрабатываем оставшиеся блоки (в обратном порядке)
    for (int i = static_cast<int>(blocks.size()) - 2; i >= 0; --i) {
        std::string block_str = std::to_string(blocks[i]);
        // Длина блока должна быть ровно 9 цифр
        int missing_zeros = 9 - static_cast<int>(block_str.size());
        assert(missing_zeros >= 0);
        // Дополняем ведущими нулями
        result += std::string(missing_zeros, '0') + block_str;
    }

    return result;
}
}  // namespace big_uint
