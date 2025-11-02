#pragma once
#include <string>
#include <unordered_map>
#include <vector>

class Request {
 public:
  Request() = default;

  bool ParseRequest(const std::string &request);

  std::string GetMethod() const;
  std::string GetPath() const;
  std::string GetVersion() const;
  std::unordered_map<std::string, std::string> GetHeaders() const;

 protected:
  bool IsValidMethod(const std::string &method);

 private:
  std::string method_;
  std::string path_;
  std::string http_version_;
  std::unordered_map<std::string, std::string> headers_;

  // All standard HTTP methods for validation
  static const std::vector<std::string> valid_methods_;
  static const std::string CRLF;
  static const std::string LF;
  static const std::string SPACE;
};
