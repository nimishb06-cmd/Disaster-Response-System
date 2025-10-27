#ifndef PATIENT_H
#define PATIENT_H

#define MAX_PATIENTS 200

typedef struct {
    int id;
    int severity; /* 3=Critical, 2=Serious, 1=Normal */
    int zoneNode; /* where patient is (graph node) */
} Patient;

/* patient max-heap (priority queue) */
typedef struct {
    Patient arr[MAX_PATIENTS];
    int size;
} PatientHeap;

void initPatientHeap(PatientHeap *h);
int isPatientHeapEmpty(PatientHeap *h);
void pushPatient(PatientHeap *h, Patient p);
Patient popPatient(PatientHeap *h);

#endif
