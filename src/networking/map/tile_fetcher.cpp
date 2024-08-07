// tile_fetcher.cpp
#include "tile_fetcher.h"
#include <iostream>
#include <curl/curl.h>
#include <string_view>
#include <optional>
#include <SDL_stdinc.h>

using namespace minicrypto::networking;
using namespace minicrypto::networking::map;

TileFetcher::TileFetcher(NetQueryStatistics& stats)
  : latitude(0.0), longitude(0.0), zoom(0), base_url("https://a.tile.openstreetmap.org/"), statistics(stats)
{
}

TileFetcher::~TileFetcher() {}

void TileFetcher::set_coordinates(double lat, double lon)
{
  latitude = lat;
  longitude = lon;
}

void TileFetcher::set_zoom(int zoom)
{
  this->zoom = zoom;
}

void TileFetcher::latlong_to_tile(double lat, double lon, int zoom, int& x, int& y) const
{
  constexpr double pi = M_PI;
  x = static_cast<int>(std::floor((lon + 180.0) / 360.0 * (1 << zoom)));
  y = static_cast<int>(std::floor((1.0 - std::log(std::tan(lat * pi / 180.0) + 1.0 / std::cos(lat * pi / 180.0)) / pi) / 2.0 * (1 << zoom)));
}

std::string TileFetcher::construct_tile_url() const
{
  int x, y;
  latlong_to_tile(latitude, longitude, zoom, x, y);
  return base_url + std::to_string(zoom) + "/" + std::to_string(x) + "/" + std::to_string(y) + ".png";
}

size_t TileFetcher::write_callback(void* contents, size_t size, size_t nmemb, std::string* s)
{
  size_t new_length = size * nmemb;
  try
  {
    s->append(static_cast<char*>(contents), new_length);
  }
  catch (const std::bad_alloc&)
  {
    // Handle memory problem
    return 0;
  }
  return new_length;
}

std::optional<std::string> TileFetcher::fetch_tile()
{
  CURL* curl = curl_easy_init();
  if (!curl)
  {
    return std::nullopt;
  }

  std::string tile_data;
  std::string url = construct_tile_url();
  curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, &tile_data);

  CURLcode res = curl_easy_perform(curl);
  if (res != CURLE_OK)
  {
    std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
    curl_easy_cleanup(curl);
    curl_global_cleanup();
    return std::nullopt;
  }

  // Update tracking variables using the injected Statistics object
  statistics.increment_api_call_count();
  statistics.add_bytes_downloaded(tile_data.size());

  curl_easy_cleanup(curl);
  curl_global_cleanup();

  return tile_data;
}

