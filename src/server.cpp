#include <arpa/inet.h>
#include <netdb.h>
#include <request.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>

// Function declarations
std::string GetHttpStatusLine(int status_code);
std::string GetBasicStatusMessage();
std::string ReadHttpRequest(int client_fd);

int main(int argc, char **argv) {
  // Flush after every std::cout / std::cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;

  // You can use print statements as follows for debugging, they'll be visible
  // when running tests.
  std::cout << "Logs from your program will appear here!\n";

  // Uncomment this block to pass the first stage
  //
  int server_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (server_fd < 0) {
    std::cerr << "Failed to create server socket\n";
    return 1;
  }

  // Since the tester restarts your program quite often, setting SO_REUSEADDR
  // ensures that we don't run into 'Address already in use' errors
  int reuse = 1;
  if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) <
      0) {
    std::cerr << "setsockopt failed\n";
    return 1;
  }

  struct sockaddr_in server_addr;
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = INADDR_ANY;
  server_addr.sin_port = htons(4221);

  if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) !=
      0) {
    std::cerr << "Failed to bind to port 4221\n";
    return 1;
  }

  int connection_backlog = 5;
  if (listen(server_fd, connection_backlog) != 0) {
    std::cerr << "listen failed\n";
    return 1;
  }

  struct sockaddr_in client_addr;
  int client_addr_len = sizeof(client_addr);

  std::cout << "Waiting for a client to connect...\n";

  int client_fd = accept(server_fd, (struct sockaddr *)&client_addr,
                         (socklen_t *)&client_addr_len);
  if (client_fd < 0) {
    std::cerr << "Failed to accept client connection\n";
    return 1;
  }

  std::cout << "Client connected\n";

  std::string raw_request = ReadHttpRequest(client_fd);

  if (raw_request.length() == 0) {
    std::cerr << "Failed to read from client\n";
    close(client_fd);
    close(server_fd);
    return 1;
  };

  std::cout << "Received request:\n" << raw_request << std::endl;

  Request request;

  if (!request.ParseRequest(raw_request)) {
    std::cerr << "Failed to parse request\n";
  }

  // TODO: remove later
  std::cout << "This is the path: " << request.GetPath() << "\n";

  int status_code;
  if (request.GetPath() == "/") {
    status_code = 200;
  } else {
    status_code = 404;
  }

  // Send a basic HTTP response to the client
  std::string http_response = GetHttpStatusLine(status_code);
  send(client_fd, http_response.c_str(), http_response.length(), 0);
  std::cout << "HTTP response sent\n";

  close(client_fd);
  close(server_fd);

  return 0;
}

std::string GetHttpStatusLine(int status_code) {
  switch (status_code) {
    case 200:
      return "HTTP/1.1 200 OK\r\n\r\n";
    case 404:
      return "HTTP/1.1 404 Not Found\r\n\r\n";
    case 500:
      return "HTTP/1.1 500 Internal Server Error\r\n\r\n";
    default:
      return "HTTP/1.1 500 Internal Server Error\r\n\r\n";
  }
}

std::string GetBasicStatusMessage() { return "HTTP/1.1 200 OK\r\n\r\n"; }

// Simple http read, assuming no body in the request.
std::string ReadHttpRequest(int client_fd) {
  std::string request;
  char buffer[4096];
  ssize_t bytes_read;

  while ((bytes_read = read(client_fd, buffer, sizeof(buffer))) > 0) {
    request.append(buffer, bytes_read);

    if (request.find("\r\n\r\n") != std::string::npos) {
      break;
    }

    if (request.size() > 64 * 1024) {  // 64KB limit
      throw std::runtime_error("HTTP request too large");
    }
  }

  return request;
}