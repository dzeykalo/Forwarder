#pragma once
#include <tuple>

std::tuple<uint16_t, std::string, std::string> get_options(const char* arg, std::size_t min_host_name_len = 4) {
  char *ptr;
  auto local_port = std::strtol(arg, &ptr, 10);
  if (0 >= local_port || local_port == LONG_MAX) {
    return {0, "", ""};
  }
  std::string remote_host(ptr+1);
  auto inx = remote_host.find(':');
  if (std::string::npos == inx) {
    return {0, "", ""};
  }
  remote_host.erase(inx);
  if (min_host_name_len > remote_host.size()) {
    return {0, "", ""};
  }
  auto remote_port = std::strtol(ptr+1+inx+1, nullptr, 10);
  if (0 >= remote_port || remote_port == LONG_MAX) {
    return {0, "", ""};
  }
  return {local_port, remote_host, std::to_string(remote_port)};
}