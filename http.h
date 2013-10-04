#ifndef __HTTP_PARSER_H__
#define __HTTP_PARSER_H__
#include "RFC2616.h"
#include "fifo.h"
#include "log.h"
#include <sys/types.h>

/* forward state(next anticipated state) */
enum http_state
{
  s_start = 0,

  s_request_line,
  s_request_line_LF,

  s_header_line,
  s_header_line_LF,
  s_headers_LF,
#define PARSING_HEADER(state) (state <= s_headers_LF)

  s_message_body,
#define PARSING_INCOMPLETE(state) (state < s_done)
  s_done,

  s_dead,
};

enum http_status
{
  sc_200_ok = 0,

#define ERROR_STATUS(status) (status > sc_200_ok)
  sc_400_bad_request,
  sc_403_forbidden,
  sc_404_not_found,
  sc_411_length_required,
  sc_413_request_entity_too_large, 
  sc_414_request_uri_too_long,
  sc_500_server_internal_error,
  sc_501_not_implemented,
  sc_503_service_unavailable,
  sc_505_http_version_not_supported,

  sc_999_unknown,
};

enum http_method
{
  HTTP_METHOD_NOT_IMPLEMENTED = 0,
  HTTP_METHOD_GET,
  HTTP_METHOD_HEAD,
  HTTP_METHOD_POST,
};
#define HAS_BODY(method) (method == HTTP_METHOD_POST)

enum http_version
{
  HTTP_VERSION_NOT_IMPLEMENTED = 0,
  HTTP_VERSION_1_1,
};

typedef struct
{
  enum http_state state;

  ssize_t nread;
  ssize_t nread_body;

  char buf[HTTP_MAX_HEADER_SIZE];       /* used to hold uri/port/path/query string */
  ssize_t buf_index;

} http_parser_t;

typedef struct
{
  enum http_method method;
  char * uri;
  enum http_version version;

#define HTTP_HEADER_NAME 0
#define HTTP_HEADER_VALUE 1
  char *headers[HTTP_MAX_HEADER_NUM][2];
  ssize_t num_headers;

  char *message_body;
  ssize_t content_length;
} http_request_t;

typedef struct
{
  /* env var ( shallow copy: don't free them! ) */
  const char *www_folder_path;
  const char *cgi_folder_path;
  const char *client_ip;
  unsigned short client_port;
  unsigned short server_port;
  log_t *log;
  
  /* handle */
  http_parser_t parser;
  http_request_t request;
  enum http_status status;

} http_handle_t;

typedef struct
{
  const char *www_folder_path;
  const char *cgi_folder_path;

  const char *client_ip;
  unsigned short client_port;
  unsigned short server_port;

  log_t *log;
} http_setting_t;

http_handle_t * http_handle_new (http_setting_t * setting);
int http_handle_execute (http_handle_t *hh, char *request, ssize_t req_len,
                         fifo_t *send_buf, int * pipe_fd, pid_t * cgi_pid);
void http_handle_free (http_handle_t * hh);

#endif
