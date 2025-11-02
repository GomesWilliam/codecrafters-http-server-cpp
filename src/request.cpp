#include "request.h"

#include <algorithm>
#include <iostream>

const std::vector<std::string> Request::valid_methods_ = {
    "GET",      // Retrieve data
    "POST",     // Submit data
    "PUT",      // Update/create resource
    "DELETE",   // Delete resource
    "HEAD",     // Get headers only (like GET but no body)
    "OPTIONS",  // Get allowed methods for resource
    "PATCH",    // Partial update
    "TRACE",    // Echo request for debugging
    "CONNECT"   // Establish tunnel (used for HTTPSS proxying)
};

const std::string Request::CRLF = "\r\n";
const std::string Request::LF = "\n";
const std::string Request::SPACE = " ";

bool Request::ParseRequest(const std::string &request) {
  size_t method_end = request.find(SPACE);
  if (method_end == std::string::npos) {
    std::cout << "Request invalid" << LF;
    return false;
  }

  std::string method = request.substr(0, method_end);
  if (!IsValidMethod(method)) {
    std::cout << "Method invalid" << LF;
    return false;
  }

  method_ = method;

  // Path (request) parser
  size_t path_start = method_end + 1;
  size_t path_end = request.find(SPACE, path_start);
  if (path_end == std::string::npos) {
    std::cout << "path request not found" << LF;
    return false;
  }

  std::string path = request.substr(path_start, path_end - path_start);
  path_ = path;

  // HTTP Version parser
  size_t version_start = path_end + 1;
  size_t version_end = request.find(CRLF, version_start);
  if (version_end == std::string::npos) {
    std::cout << "HTTP Version not found" << LF;
    return false;
  }

  std::string version =
      request.substr(version_start, version_end - version_start);
  http_version_ = version;

  // headers parser
  size_t headers_start = version_end + CRLF.length();
  size_t current_pos = headers_start;
  while (current_pos < request.length()) {
    size_t line_end = request.find(CRLF, current_pos);
    if (line_end == std::string::npos) {
      std::cout << "End header" << LF;
      break;
    }

    std::string header_line =
        request.substr(current_pos, line_end - current_pos);

    // header is a key-value pair
    size_t colon_pos = header_line.find(':');
    if (colon_pos != std::string::npos) {
      std::string key = header_line.substr(0, colon_pos);
      std::string value = header_line.substr(colon_pos + 1);

      headers_[key] = value;
    }

    current_pos = line_end + CRLF.length();
  }

  return true;
}

bool Request::IsValidMethod(const std::string &method) {
  return std::find(valid_methods_.begin(), valid_methods_.end(), method) !=
         valid_methods_.end();
}

std::string Request::GetMethod() const { return method_; };
std::string Request::GetPath() const { return path_; };
std::string Request::GetVersion() const { return http_version_; };
std::unordered_map<std::string, std::string> Request::GetHeaders() const {
  return headers_;
};