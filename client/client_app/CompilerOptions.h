#ifndef COMPILER_OPTIONS
#define COMPILER_OPTIONS

#define CLIENT_APP

#define WIN32_LEAN_AND_MEAN // to avoid winsock errors

#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable: 4251 4275 4244 4267 4003 4996 4018) 

#define STATIC_LIBMONGOCLIENT

#endif