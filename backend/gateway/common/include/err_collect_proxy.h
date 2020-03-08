

#pragma once



#include <string>
#include <stdio.h>
#include <stdlib.h>

extern "C" void init_proxy(int argc, char* argv[]);

extern "C" int write_message_string(std::string &project, std::string &modulename, std::string &functionname,
							 std::string &err_type, std::string &params, std::string &message);

extern "C" int write_message_char(char *char_project, char * char_modulename, char * char_functionname,
								  char *char_errtype, char *char_params, char *char_message);

extern "C" int set_default_log(char *char_project, char * char_modulename, char * char_functionname,
							   char *char_errtype, char *char_params, char *char_message);

extern "C" int write_message_simple(char *char_errtype, char *char_message);

extern "C" int write_msg_func_param(char *char_functionname, char *char_params, char *char_message);



