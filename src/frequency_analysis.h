#pragma once

#include "utils.h"

#include <memory>
#include <vector>
#include <unordered_map>

namespace minicrypto
{
  struct LetterFrequencyInfo
  {
    uint32_t letter;
    float frequency;

    LetterFrequencyInfo(uint32_t letter, float frequency);

    static inline bool small_to_large(
      const LetterFrequencyInfo& a,
      const LetterFrequencyInfo& b
    );
    static inline bool large_to_small(
      const LetterFrequencyInfo& a,
      const LetterFrequencyInfo& b
    );
  };

  class LetterFrequencyAnalyzer
  {
   protected:
    std::vector<LetterFrequencyInfo> frequencies;
    std::unordered_map<uint32_t, uint32_t> indices;

   public:
    LetterFrequencyAnalyzer(std::vector<LetterFrequencyInfo> _frequencies);

    float evaluate(const byte_string& input) const;

    std::vector<LetterFrequencyInfo>       get_frequencies() const;
    std::unordered_map<uint32_t, uint32_t> get_indices() const;

    static LetterFrequencyAnalyzer generate_from_byte_string(const byte_string& input);
    static LetterFrequencyAnalyzer generate_english();
    static LetterFrequencyAnalyzer generate_english_dict();
  };
};

