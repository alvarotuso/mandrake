#include <iterator>
#include <string>

namespace mandrake::utils::iterators {
class StringSplitIterator {
private:
    int start_index;
    int end_index;
    std::string original_string;
    std::string current_value;
    std::string split;
    void get_next_value();
public:
    StringSplitIterator(std::string original_string, std::string split, int start_index);
    bool operator!=(StringSplitIterator const& other) const;
    std::string operator*();
    void operator++();
    using iterator_category = std::forward_iterator_tag;
    using value_type = std::string;
    using difference_type = int;
    using pointer = std::string*;
    using reference = std::string&;
};


class StringSplitGenerator {
private:
    std::string original_string;
    std::string split;
public:
    StringSplitGenerator(std::string original_string, std::string split);
    StringSplitIterator begin();
    StringSplitIterator end();
};
}