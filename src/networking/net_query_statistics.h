#pragma once

namespace minicrypto::networking
{
  class NetQueryStatistics
  {
    int api_call_count;
    size_t bytes_downloaded;

   public:
    NetQueryStatistics();
    void increment_api_call_count();
    void add_bytes_downloaded(size_t bytes);
    int get_api_call_count() const;
    size_t get_bytes_downloaded() const;
  };
}
