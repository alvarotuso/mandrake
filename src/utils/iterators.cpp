#include "iterators.hpp"

#include <utility>

namespace mandrake::utils::iterators {
    StringSplitGenerator::StringSplitGenerator(std::string original_string, std::string split):
        original_string{std::move(original_string)}, split{std::move(split)}  {}

    StringSplitIterator StringSplitGenerator::begin() {
        return StringSplitIterator(this->original_string, this->split, 0);
    }

    StringSplitIterator StringSplitGenerator::end() {
        return StringSplitIterator();
    }

    void StringSplitIterator::get_next_value() {
        if (this->start_index > this->original_string.size()) {
            this->start_index = -1;
            this->end_index = -1;
            return;
        }
        this->end_index = this->original_string.find(this->split, this->start_index);
        if (this->end_index < 0) {
            this->end_index = static_cast<int>(this->original_string.size());
        }
        this->current_value = this->original_string.substr(this->start_index,
                this->end_index - this->start_index);
        this->start_index = this->end_index + 1;
        this->end_index = 0;
    }

    StringSplitIterator::StringSplitIterator(std::string original_string, std::string split,
            int start_index): original_string{std::move(original_string)}, split{std::move(split)},
            start_index{start_index}, end_index{0} {
        this->get_next_value();
    }

    StringSplitIterator::StringSplitIterator(): start_index {-1}, end_index {-1} {}

    bool StringSplitIterator::operator!=(StringSplitIterator const& other) const {
        return this->start_index != other.start_index || this->end_index != other.end_index;
    }

    void StringSplitIterator::operator++() {
        this->get_next_value();
    }

    std::string StringSplitIterator::operator*() {
        return this->current_value;
    }
}