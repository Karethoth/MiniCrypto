#include "frequency_analysis.h"

#include <algorithm>

using namespace minicrypto;


LetterFrequencyInfo::LetterFrequencyInfo(
  uint32_t letter,
  float frequency
) : letter(letter), frequency(frequency)
{
};

inline bool LetterFrequencyInfo::small_to_large(
  const LetterFrequencyInfo& a,
  const LetterFrequencyInfo& b
)
{
  return a.frequency < b.frequency;
}

inline bool LetterFrequencyInfo::large_to_small(
  const LetterFrequencyInfo& a,
  const LetterFrequencyInfo& b
)
{
  return a.frequency > b.frequency;
}

LetterFrequencyAnalyzer::LetterFrequencyAnalyzer(
  std::vector<LetterFrequencyInfo> _frequencies
) : frequencies(_frequencies), indices()
{
  std::sort(
    frequencies.begin(),
    frequencies.end(),
    LetterFrequencyInfo::large_to_small
  );

  uint32_t index = 0;
  for (const auto& freq : frequencies)
  {
    indices[freq.letter] = index++;
  }
}

float LetterFrequencyAnalyzer::evaluate(
  const byte_string& input
) const
{
  if (input.length() <= 0)
  {
    return 0.f;
  }

  float score = 0;

  // Calculate letter frequencies
  const auto input_analysis = LetterFrequencyAnalyzer::generate_from_byte_string(input);
  const auto input_frequencies = input_analysis.get_frequencies();
  const auto input_indices = input_analysis.get_indices();

  for (const auto& ind : input_indices)
  {
    const auto upper = std::toupper(ind.first);
    if (indices.find(upper) == indices.end())
    {
      if (upper == ' ' || upper == '\r' || upper == '\n' || std::isalnum(upper))
      {
        continue;
      }
      if (std::iscntrl(upper))
      {
        score -= 100;
      }
      else if (!std::isalnum(upper))
      {
        score -= 10;
      }
      continue;
    }

    const auto local_index = indices.at(upper);
    const auto diff = std::abs((float)local_index - ind.second);
    score += 100 / (diff+1);
  }

  score /= input.size();

  return score;
}

std::vector<LetterFrequencyInfo>
LetterFrequencyAnalyzer::get_frequencies() const
{
  return frequencies;
}

std::unordered_map<uint32_t, uint32_t>
LetterFrequencyAnalyzer::get_indices() const
{
  return indices;
}

LetterFrequencyAnalyzer
LetterFrequencyAnalyzer::generate_from_byte_string(
  const byte_string& input
)
{
  std::unordered_map<uint32_t, uint32_t> occurences{};
  for (const unsigned char& c : input)
  {
    occurences[c]++;
    if (c > 10000)
    {
      continue;
    }
  }

  const float input_count = static_cast<float>(input.length());

  std::vector<LetterFrequencyInfo> freq{};
  for (const auto& pair : occurences)
  {
    freq.emplace_back(pair.first, pair.second / input_count * 100);
  }

  return { freq };
}

LetterFrequencyAnalyzer
LetterFrequencyAnalyzer::generate_english()
{
  std::vector<LetterFrequencyInfo> freq{};
  freq.emplace_back('E', 13);
  freq.emplace_back('T', 9.1);
  freq.emplace_back('A', 8.2);
  freq.emplace_back('O', 7.5);
  freq.emplace_back('I', 7);
  freq.emplace_back('N', 6.7);
  freq.emplace_back('S', 6.3);
  freq.emplace_back('H', 6);
  freq.emplace_back('R', 6);
  freq.emplace_back('D', 4.3);
  freq.emplace_back('L', 4);
  freq.emplace_back('C', 2.8);
  freq.emplace_back('U', 2.8);
  freq.emplace_back('M', 2.4);
  freq.emplace_back('W', 2.4);
  freq.emplace_back('F', 2.2);
  freq.emplace_back('G', 2);
  freq.emplace_back('Y', 2);
  freq.emplace_back('P', 1.9);
  freq.emplace_back('B', 1.5);
  freq.emplace_back('V', 0.98);
  freq.emplace_back('K', 0.77);
  freq.emplace_back('J', 0.15);
  freq.emplace_back('X', 0.15);
  freq.emplace_back('Q', 0.095);
  freq.emplace_back('Z', 0.074);
  return { freq };
}


LetterFrequencyAnalyzer
LetterFrequencyAnalyzer::generate_english_dict()
{
  std::vector<LetterFrequencyInfo> freq{};
  freq.emplace_back('E', 11);
  freq.emplace_back('S', 8.7);
  freq.emplace_back('I', 8.6);
  freq.emplace_back('A', 7.8);
  freq.emplace_back('R', 7.3);
  freq.emplace_back('N', 7.2);
  freq.emplace_back('T', 6.7);
  freq.emplace_back('O', 6.1);
  freq.emplace_back('L', 5.3);
  freq.emplace_back('C', 4);
  freq.emplace_back('D', 3.8);
  freq.emplace_back('U', 3.3);
  freq.emplace_back('G', 3);
  freq.emplace_back('P', 2.8);
  freq.emplace_back('M', 2.7);
  freq.emplace_back('H', 2.3);
  freq.emplace_back('B', 2);
  freq.emplace_back('Y', 1.6);
  freq.emplace_back('F', 1.4);
  freq.emplace_back('V', 1);
  freq.emplace_back('K', 0.97);
  freq.emplace_back('W', 0.91);
  freq.emplace_back('Z', 0.44);
  freq.emplace_back('X', 0.27);
  freq.emplace_back('J', 0.21);
  freq.emplace_back('Q', 0.19);
  return { freq };
}

