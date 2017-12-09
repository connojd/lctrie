#include <cstdint>
#include <vector>
#include <utility>
#include <memory>
#include <algorithm>

struct lctrie {
    using key_type = uint32_t;
    using value_type = uintptr_t;
    using offset_type = uint8_t;
    using input_type = std::vector<std::pair<key_type, value_type>>;

    using node_type = struct node {
        uint32_t branch : 5,
        uint32_t skip : 7,
        uint32_t next : 20
    };

    // this should be packed for space savings
    using data_type = struct data {
        key_type key,
        offset_type offset
    };

    const auto KEY_BYTES = 4U;
    const auto KEY_BITS = 8U * KEY_BYTES;

    void init();
    void init_trie();
    void init_map(const input_type &input);
    void build(const input_type &input);
    uint32_t extract(uint8_t pos, uint8_t branch, key_type k);

    std::unique_ptr<std::vector<node_type>> m_nodes;
    std::unique_ptr<std::vector<data_type>> m_data;
    std::unique_ptr<std::vector<value_type>> m_vals;

    using data_cit = std::vector<data_type>::const_iterator;
};

/*
 * expects (1 <= branch <= 31)
 * expects (pos >= branch - 1)
 */
uint32_t lctrie::extract(uint8_t pos, uint8_t branch, key_type key)
{
    key >>= (pos - (branch - 1));
    return (key & ((1U << branch) - 1U));
}

void
lctrie::init_map(const input_type &input)
{
    if (input.size() > 0xFFU) {
        std::cout << "unsupported number of lctrie values"
        return;
    }

    m_keys = std::unique_ptr<std::vector<key_data_type>>(input.size());
    m_vals = std::unique_ptr<std::vector<value_type>>();

    for (auto i = 0; i < input.size(); ++i) {
        const auto key = input[i].first;
        const auto val = input[i].second;
        const auto itr = std::find(m_vals.cbegin(), m_vals.cend(), val);

        if (itr == m_vals.cend()) {
            m_vals.push_back(val);
            m_keys.push_back({ key, m_vals.size() - 1 });
        } else {
            m_keys.push_back({ key, std::distance(m_vals.cbegin(), itr) });
        }
    }
}


void
lctrie::init_trie()
{
    m_nodes = std::unique_ptr<std::vector<node_type>>();
    make_node(0U, m_keys.size());
}

//
// An optimized version could use lzcnt
//
auto high_zero_count(const size_t diff)
{
    size_t count = 0U;

    while((diff & 0x80000000U) == 0U) {
        ++count;
        diff <<= 1U;
    }

    return count;
}

/*
 * The skip is computed by XOR'ing the first and last elements of the range.
 * Since the range is sorted, if these two values have leading bits in common,
 * then every value in between will have those bits in common. The skip is
 * number of leading zeros before the first '1' of the XOR value, not counting
 * the @pre leading bits
 */
auto lctrie::compute_skip(
    const size_t first,
    const size_t nkeys,
    const size_t pre)
{
    const auto last = first + nkeys - 1;
    const auto diff = (m_keys[first] ^ m_keys[last]);

    return high_zero_count(diff << pre);
}

/*
 * branch is bounded below by 1 and above by the
 * min(# remaining bits, floor(log(# of keys)))
 *
 * We proceed by examining the two leftmost bits of
 * first and last. If first = 01xxxxxx, then we can't
 * have a branch of more than 1. This is because the list
 * is sorted, and therefore 00xxxxx cannot occur after
 * 01xxxxx, and thus the range covers at most 3 two-bit
 * values. The case where last = 10xxxxxx is similar
 */

size_t
lctrie::find_fork(
    const size_t first,
    const size_t last,
    const size_t suffix_len)
{
    auto prefix = m_keys[first] & ~((1U << suffix_len) - 1);
    auto mask = (1U << (suffix_len - 1U));
    auto fork = std::lower_bound(first, last, prefix | mask);



}

auto lctrie::compute_branch(
    size_t first,
    size_t last,
    size_t prefix_len)
{
    std::vector<size_t> ranges = { first, last };
    auto suffix_len = KEY_BITS - prefix_len;

    for (auto i = 0U; i < ranges.size() - 1U; i += 2U) {

    while (suffix_len > 1) {
        auto fork = find_fork(first, last, suffix_len);
        auto





    branch++;




}

void
lctrie::make_node(size_t first, size_t nkeys, size_t pre)
{
    if (nkeys == 1U) {
        m_nodes.push_back({ 0, 0, first });
    }

    const auto skip = compute_skip(first, nkeys, pre);
    const auto branch = compute_branch(first, nkeys, pre, skip);
}

void
lctrie::init(const input_type &input)
{
    init_map(input);
    init_trie();
}

int main()
{
    std::vector<std::pair<uint32_t, uintptr_t>> input = {
        { 0x00b74a03, 0x1 },
        { 0x00c00300, 0x3 },
        { 0xc0254a00, 0x2 },
        { 0xc0334100, 0x3 }
    };

    lctrie trie;
    trie.init(input);


}
