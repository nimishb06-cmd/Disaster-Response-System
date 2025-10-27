#include <stdio.h>
#include <string.h>
#include "hospital.h"
#include "graph.h"
#include "fileio.h"
#include "patient.h"

/* initialize */
void initHospitals(Hospital hospitals[], int n) {
    for (int i = 0; i < n; i++) hospitals[i].occupied = 0;
}

void printHospitals(Hospital hospitals[], int n) {
    printf("\nHospitals:\n");
    for (int i = 0; i < n; i++) {
        printf("%d) %s (Node %d) Beds: %d/%d\n",
               i+1, hospitals[i].name, hospitals[i].nodeId,
               hospitals[i].occupied, hospitals[i].capacity);
    }
}

/* compute distances from patient node and admit to nearest available hospital */
int admitTopPatient(PatientHeap *ph, Hospital hospitals[], int hcount, int nodes) {
    if (isPatientHeapEmpty(ph)) {
        printf("No patients waiting.\n");
        return -1;
    }
    Patient p = popPatient(ph);
    int dist[MAXN];
    if (p.zoneNode < 0 || p.zoneNode >= nodes) {
        printf("Invalid patient zone (%d). Cannot compute route.\n", p.zoneNode);
        return -1;
    }
    dijkstra(nodes, p.zoneNode, dist);

    int bestIdx = -1;
    int bestDist = INF;
    for (int i = 0; i < hcount; i++) {
        if (hospitals[i].occupied < hospitals[i].capacity) {
            int hn = hospitals[i].nodeId;
            if (hn >=0 && hn < nodes && dist[hn] < bestDist) {
                bestDist = dist[hn];
                bestIdx = i;
            }
        }
    }

    char logbuf[256];
    if (bestIdx != -1) {
        hospitals[bestIdx].occupied++;
        sprintf(logbuf, "Patient %d (sev=%d) admitted to %s (Beds %d/%d), distance %d",
                p.id, p.severity, hospitals[bestIdx].name,
                hospitals[bestIdx].occupied, hospitals[bestIdx].capacity, bestDist);
        printf("%s\n", logbuf);
        logHospital(logbuf);
        return bestIdx;
    } else {
        sprintf(logbuf, "Patient %d (sev=%d) could not be admitted - all hospitals full",
                p.id, p.severity);
        printf("%s\n", logbuf);
        logHospital(logbuf);
        return -1;
    }
}
