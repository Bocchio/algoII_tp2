#ifndef ERRORS_HPP__
#define ERRORS_HPP__

#define ERROR_MSG_CANT_OPEN_FILE    "While opening stream: "
#define ERROR_MSG_UNKNOWN_METHOD    "Unknown method."
#define ERROR_MSG_CORRUPTED_DATA    "Input data was corrupt."
#define ERROR_MSG_INVALID_ARG       "Invalid non-option argument: "
#define ERROR_MSG_OPT_REQ_ARG       "Option requires argument: "
#define ERROR_MSG_UNKNOWN_OPT       "Unknown option: "
#define ERROR_MSG_MANDATORY_OPT     "Option is mandatory."
#define HELP_MSG                    "cmdline [-i file] [-o file] [-m method]"

#ifndef EXIT_FAILURE
    #define EXIT_FAILURE 1
#endif

#ifndef EXIT_SUCCESS
    #define EXIT_SUCCESS  0
#endif

#endif  // ERRORS_HPP__
