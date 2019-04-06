#define SSH_NO_CPP_EXCEPTIONS

#include <iostream>
#include <cstdlib>
#include <errno.h>
#include <cstring>
#include <libssh/libsshpp.hpp>

int main() {
  //allocates a new ssh_session with ss_new() and 
  //assigns that to my_ssh_session variable
  ssh_session my_ssh_session = ssh_new();

  //verfy whether the allocation of the session succeeded
  if (my_ssh_session == NULL) {
    exit(-1);
  }

  int verbosity = SSH_LOG_PROTOCOL;
  int port = 22;

  //setting the options of the session
  ssh_options_set(my_ssh_session, SSH_OPTIONS_HOST, "mhisajib@matrix.senecacollege.ca");
  ssh_options_set(my_ssh_session, SSH_OPTIONS_PORT, &port);
  ssh_options_set(my_ssh_session, SSH_OPTIONS_LOG_VERBOSITY, &verbosity);

  //connecting to the host, connection will return an int value
  //on successful connection it will return SSH_OK
  //on unsuccessful connection it will return SSH_ERROR
  int connectionReturn;
  connectionReturn = ssh_connect(my_ssh_session);

  //checking whether the connection was established successfully
  if (connectionReturn != SSH_OK) {
    //print the error messages to standard error output
    std::cout << "Error Connecting to Matrix" << std::endl;
    std::cerr << ssh_get_error(my_ssh_session);
    exit(-1);
  }
  else if (connectionReturn == SSH_OK) {
    std::cout << "Successfully connected to Matrix";
  }

  //disconnect the connection
  ssh_disconnect(my_ssh_session);

  //deallocate the session allocated with ssh_free
  ssh_free(my_ssh_session);
}

