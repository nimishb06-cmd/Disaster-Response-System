#ifndef HOSPITAL_H
#define HOSPITAL_H

#include "patient.h"

typedef struct {
    char name[64];
    int nodeId;    /* graph node where hospital is */
    int capacity;  /* total beds */
    int occupied;  /* current */
} Hospital;

void initHospitals(Hospital hospitals[], int n);
void printHospitals(Hospital hospitals[], int n);

/* admit top priority patient from heap to nearest available hospital */
int admitTopPatient(PatientHeap *ph, Hospital hospitals[], int hcount, int nodes);

#endif
