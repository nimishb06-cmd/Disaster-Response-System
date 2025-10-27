#include "patient.h"
#include <stdio.h>

static void swapP(Patient *a, Patient *b) {
    Patient t = *a; *a = *b; *b = t;
}

void initPatientHeap(PatientHeap *h) {
    h->size = 0;
}

int isPatientHeapEmpty(PatientHeap *h) {
    return h->size == 0;
}

void pushPatient(PatientHeap *h, Patient p) {
    if (h->size >= MAX_PATIENTS) {
        printf("Patient heap full!\n");
        return;
    }
    int i = h->size++;
    h->arr[i] = p;
    /* bubble up by severity then id (higher severity first) */
    while (i != 0) {
        int parent = (i - 1) / 2;
        if (h->arr[parent].severity < h->arr[i].severity) {
            swapP(&h->arr[parent], &h->arr[i]);
            i = parent;
        } else break;
    }
}

Patient popPatient(PatientHeap *h) {
    Patient ret = {0,0,0};
    if (h->size == 0) {
        return ret;
    }
    ret = h->arr[0];
    h->arr[0] = h->arr[--h->size];
    /* heapify down */
    int i = 0;
    while (1) {
        int l = 2*i + 1;
        int r = 2*i + 2;
        int largest = i;
        if (l < h->size && h->arr[l].severity > h->arr[largest].severity) largest = l;
        if (r < h->size && h->arr[r].severity > h->arr[largest].severity) largest = r;
        if (largest != i) {
            swapP(&h->arr[i], &h->arr[largest]);
            i = largest;
        } else break;
    }
    return ret;
}
