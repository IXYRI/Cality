#pragma once
#include <assert.h>
#include <stdarg.h>
#include <threads.h>
#include <time.h>
#include "datom.h"

/* @brief Check if an error occurred */
bool  err(void);
/* @brief Get the error message or set a new one
 * @param setmsg The error message to set, null to dismiss
 * @note Getting message clears the error state and vice versa
 * @warning Terminates the program if two errors meet */
char *errmsg(char *setmsg);
/* @brief Terminate the program due to an error, if present */
void  efail(void);

#include "silo.h"
#include "nexus.h"
#include "recepta.h"
#include "logos.h"
