#pragma once

#include <string>
#include <optional>
#include <curl/curl.h>
#include "../net_query_statistics.h"

namespace minicrypto::networking::map
{
  class TileFetcher
  {
    minicrypto::networking::NetQueryStatistics& statistics;
    double latitude;
    double longitude;
    int zoom;
    std::string base_url;

    static size_t write_callback(void* contents, size_t size, size_t nmemb, std::string* s);
    std::string construct_tile_url() const;
    void latlong_to_tile(double lat, double lon, int zoom, int& x, int& y) const;

  public:
    TileFetcher(minicrypto::networking::NetQueryStatistics& stats);
    ~TileFetcher();

    void set_coordinates(double lat, double lon);
    void set_zoom(int zoom);
    std::optional<std::string> fetch_tile();
  };
}

