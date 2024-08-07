#include "net_query_statistics.h"

using namespace minicrypto::networking;

NetQueryStatistics::NetQueryStatistics() : api_call_count(0), bytes_downloaded(0) {}

void NetQueryStatistics::increment_api_call_count()
{
  api_call_count++;
}

void NetQueryStatistics::add_bytes_downloaded(size_t bytes)
{
  bytes_downloaded += bytes;
}

int NetQueryStatistics::get_api_call_count() const
{
  return api_call_count;
}

size_t NetQueryStatistics::get_bytes_downloaded() const
{
  return bytes_downloaded;
}

