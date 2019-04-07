#define SSH_NO_CPP_EXCEPTIONS
#define _CRT_SECURE_NO_WARNINGS
#include <libssh/libsshpp.hpp>
#include <string>
#include <iostream>
//int verify_knownhost(ssh_session session);
int show_remote_files(ssh_session session)
{
  ssh_channel channel;
  int rc;
  char command[50];
  channel = ssh_channel_new(session);
  if (channel == NULL) return SSH_ERROR;
  rc = ssh_channel_open_session(channel);
  if (rc != SSH_OK)
  {
    ssh_channel_free(channel);
    return rc;
  }
  std::cout << "Please enter a command to run: ";
  std::cin >> command;
  //std::cin.getline(command, 49,'\n');
  //std::cout << command << std::endl;
  rc = ssh_channel_request_exec(channel, command);
  if (rc != SSH_OK)
  {
    ssh_channel_close(channel);
    ssh_channel_free(channel);
    return rc;
  }
  char buffer[256];
  int nbytes;
  nbytes = ssh_channel_read(channel, buffer, sizeof(buffer), 0);
  while (nbytes > 0)
  {
    if (fwrite(buffer, 1, nbytes, stdout) != nbytes)
    {
      ssh_channel_close(channel);
      ssh_channel_free(channel);
      return SSH_ERROR;
    }
    nbytes = ssh_channel_read(channel, buffer, sizeof(buffer), 0);
  }
  if (nbytes < 0)
  {
    ssh_channel_close(channel);
    ssh_channel_free(channel);
    return SSH_ERROR;
  }
  ssh_channel_send_eof(channel);
  ssh_channel_close(channel);
  ssh_channel_free(channel);
  return SSH_OK;
}



int main() {
  ssh_session my_ssh_session;
  int rc;
  char userName[30];// = "mhisajib";
  char password[30];
  char host[] = { "matrix.senecacollege.ca" };
  int port{ 22 };
  int verbosity{ SSH_LOG_PROTOCOL };

  //open SSH session and set options
  my_ssh_session = ssh_new();
  if (my_ssh_session == NULL) {
    exit(-1);
  }
  
  //setting the hostname
  ssh_options_set(my_ssh_session, SSH_OPTIONS_HOST, host);
  ssh_options_set(my_ssh_session, SSH_OPTIONS_PORT, &port);
  //ssh_options_set(my_ssh_session, SSH_OPTIONS_LOG_VERBOSITY, &verbosity);

  //connect to server
  rc = ssh_connect(my_ssh_session);
  if (rc != SSH_OK) {
    std::cerr << "Error connecting to " << host;
    std::cerr << "Error: " << ssh_get_error(my_ssh_session);
    ssh_free(my_ssh_session);
    exit(-1);
  }

  //verify the server's identity
  /*
  if (verify_knownhost(my_ssh_session) < 0) {
    ssh_disconnect(my_ssh_session);
    ssh_free(my_ssh_session);
    exit(-1);
  }
  */
  //authenticate ourselves
  std::cout << "Enter your Matrix Credentials" << std::endl;
  std::cout << "-----------------------------" << std::endl;
  std::cout << "Username: ";
  std::cin >> userName;
  std::cout << "Password: ";
  std::cin >> password;
  rc = ssh_userauth_password(my_ssh_session, userName, password);
  if (rc != SSH_AUTH_SUCCESS) {
    std::cerr << "Error authenticating with password" << std::endl;
    std::cerr << "Error: " << ssh_get_error(my_ssh_session);
    ssh_disconnect(my_ssh_session);
    ssh_free(my_ssh_session);
    exit(-1);
  }
  else {
    bool again { false };
    do {
      char choice{};
      show_remote_files(my_ssh_session);
      std::cout << "Would like to enter another command? : ";
      std::cin >> choice;
      (choice == 'Y' || choice == 'y') ? again = true : again = false;
    } while (again);
  }
  std::cout << "Goodbye!" << std::endl;
  ssh_disconnect(my_ssh_session);
  ssh_free(my_ssh_session);
  return 0;
}



/*
int verify_knownhost(ssh_session session) {
  enum ssh_known_hosts_e state;
  unsigned char* hash = NULL;
  ssh_key srv_pubkey = NULL;
  size_t hlen;
  char buf[10];
  char* hexa;
  char* p;
  int cmp;
  int rc;
  rc = ssh_get_server_publickey(session, &srv_pubkey);
  if (rc < 0) {
    return -1;
  }
  rc = ssh_get_publickey_hash(srv_pubkey, SSH_PUBLICKEY_HASH_SHA1, &hash, &hlen);
  ssh_key_free(srv_pubkey);
  if (rc < 0) {
    return -1;
  }
  state = ssh_session_is_known_server(session);
  switch (state) {
  case SSH_KNOWN_HOSTS_OK:
    //OK
    break;
  case SSH_KNOWN_HOSTS_CHANGED:
    std::cerr << "Host key for server changed: it is now:" << std::endl;
    ssh_print_hexa("Public key hash", hash, hlen);
    std::cerr << "For security reasons, connection will be stopped" << std::endl;
    ssh_clean_pubkey_hash(&hash);
    return -1;

  case SSH_KNOWN_HOSTS_OTHER:
    std::cerr << "The host key for this server was not found but an other type of key exists." << std::endl;
    std::cerr << "An attacker might change the default server key to confuse your client into thinking the key does not exist" << std::endl;
    ssh_clean_pubkey_hash(&hash);
    return -1;

  case SSH_KNOWN_HOSTS_NOT_FOUND:
    std::cerr << "Could not find known host file." << std::endl;
    std::cerr << "If you accept the host key here, the file will be automatically created." << std::endl;

  case SSH_KNOWN_HOSTS_UNKNOWN:
    hexa = ssh_get_hexa(hash, hlen);
    std::cerr << "The server is unknown. Do you trust the host key?" << std::endl;
    std::cerr << "Public key hash: " << hexa << std::endl;
    ssh_string_free_char(hexa);
    ssh_clean_pubkey_hash(&hash);
    p = fgets(buf, sizeof(buf), stdin);
    if (p == NULL) {
      return -1;
    }
    cmp = strncasecmp(buf, "yes", 3);
    if (cmp != 0) {
      return -1;
    }
    rc = ssh_session_update_known_hosts(session);
    if (rc < 0) {
      fprintf(stderr, "Error %s\n", strerror(errno));
      return -1;
    }
    break;

  case SSH_KNOWN_HOSTS_ERROR:
    std::cerr << "Error: ";
    std::cerr << ssh_get_error(session);
    ssh_clean_pubkey_hash(&hash);
    return -1;
    break;

  default:
    break;
  }
  ssh_clean_pubkey_hash(&hash);
  return 0;
}
*/