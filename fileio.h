#ifndef FILEIO_H
#define FILEIO_H

#include "resource.h"
#include "patient.h"

void logRunHeader(const char *filename, const char *title);
void logAllocation(Zone z, int f, int m, int t, int unmetF, int unmetM, int unmetT);
void logHospital(const char *message);
void viewFile(const char *filename);

#endif
