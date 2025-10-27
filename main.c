#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "resource.h"
#include "graph.h"
#include "hospital.h"
#include "patient.h"
#include "fileio.h"

int main() {
    Zone zones[MAX_ZONES];
    int zoneCount = 0;
    int foodStock=0, medStock=0, teamStock=0;

    int nodes = 0;
    int distArr[MAXN];

    Hospital hospitals[20];
    int hospitalCount = 0;

    PatientHeap pheap;
    initPatientHeap(&pheap);

    int choice;
    while (1) {
        printf("\n====== Disaster Response System ======\n");
        printf("1. Load Sample Data (sample_data.txt)\n");
        printf("2. Input Zones & Resources Manually\n");
        printf("3. Allocate Resources (Heap)\n");
        printf("4. Input Transport Graph (adjacency matrix)\n");
        printf("5. Show Shortest Paths from Base (node 0)\n");
        printf("6. Setup Hospitals\n");
        printf("7. Add Patient to Priority Queue\n");
        printf("8. Admit Top Priority Patient (to nearest hospital)\n");
        printf("9. View Hospital Status\n");
        printf("10. View Logs\n");
        printf("11. Exit\n");
        printf("Enter choice: ");
        if (scanf("%d", &choice) != 1) { scanf("%*s"); continue; }

        if (choice == 1) {
            /* parse sample_data.txt (basic parser, ignore comments starting '#') */
            FILE *f = fopen("sample_data.txt", "r");
            if (!f) { printf("sample_data.txt not found.\n"); continue; }
            char line[256];

            // read nodes
            while (fgets(line, sizeof(line), f)) {
                if (line[0]=='#' || strlen(line)<=1) continue;
                nodes = atoi(line);
                break;
            }
            if (nodes <=0 || nodes > MAXN) { printf("Invalid nodes count in sample.\n"); fclose(f); continue; }

            // read adjacency matrix
            for (int i=0;i<nodes;i++) {
                int read = 0;
                while (fgets(line,sizeof(line),f)) {
                    if (line[0]=='#' || strlen(line)<=1) continue;
                    // parse this row
                    char *p = line;
                    for (int j=0;j<nodes;j++) {
                        int val = atoi(p);
                        graph[i][j] = val;
                        // advance p to next token
                        while (*p && *p!=' ' && *p!='\t' && *p!='\n') p++;
                        while (*p==' ' || *p=='\t') p++;
                    }
                    read = 1;
                    break;
                }
                if (!read) { printf("Error reading matrix row %d\n", i); }
            }

            // hospitals
            while (fgets(line,sizeof(line),f)) { if (line[0]=='#' || strlen(line)<=1) continue; hospitalCount = atoi(line); break; }
            for (int i=0;i<hospitalCount;i++) {
                while (fgets(line,sizeof(line),f)) {
                    if (line[0]=='#' || strlen(line)<=1) continue;
                    // format: Name,node,capacity
                    char name[64]; int nid, cap;
                    char *tok = strtok(line, ",");
                    if (!tok) continue;
                    strncpy(name, tok, sizeof(name)); name[sizeof(name)-1]=0;
                    tok = strtok(NULL, ",");
                    nid = tok ? atoi(tok) : 0;
                    tok = strtok(NULL, ",");
                    cap = tok ? atoi(tok) : 0;
                    strncpy(hospitals[i].name, name, sizeof(hospitals[i].name));
                    hospitals[i].nodeId = nid;
                    hospitals[i].capacity = cap;
                    hospitals[i].occupied = 0;
                    break;
                }
            }

            // zones
            while (fgets(line,sizeof(line),f)) { if (line[0]=='#' || strlen(line)<=1) continue; zoneCount = atoi(line); break; }
            for (int i=0;i<zoneCount;i++) {
                while (fgets(line,sizeof(line),f)) {
                    if (line[0]=='#' || strlen(line)<=1) continue;
                    int id,pop,sev,fNeed,mNeed,tNeed;
                    sscanf(line, "%d %d %d %d %d %d", &id, &pop, &sev, &fNeed, &mNeed, &tNeed);
                    zones[i].id = id;
                    zones[i].population = pop;
                    zones[i].severity = sev;
                    zones[i].foodNeed = fNeed;
                    zones[i].medNeed = mNeed;
                    zones[i].teamNeed = tNeed;
                    calculatePriority(&zones[i]);
                    break;
                }
            }

            // central stock
            while (fgets(line,sizeof(line),f)) { if (line[0]=='#' || strlen(line)<=1) continue;
                int a,b,c; if (sscanf(line,"%d %d %d",&a,&b,&c)==3) { foodStock=a; medStock=b; teamStock=c; break; }
            }

            // patients
            int patientCount = 0;
            while (fgets(line,sizeof(line),f)) { if (line[0]=='#' || strlen(line)<=1) continue; patientCount = atoi(line); break; }
            for (int i=0;i<patientCount;i++) {
                while (fgets(line,sizeof(line),f)) {
                    if (line[0]=='#' || strlen(line)<=1) continue;
                    int pid, sev, zn;
                    if (sscanf(line, "%d %d %d", &pid, &sev, &zn)==3) {
                        Patient p = {pid, sev, zn};
                        pushPatient(&pheap, p);
                        break;
                    }
                }
            }

            initHospitals(hospitals, hospitalCount);
            printf("Sample data loaded: nodes=%d, hospitals=%d, zones=%d, patients queued=%d\n",
                   nodes, hospitalCount, zoneCount, pheap.size);
            fclose(f);
        }
        else if (choice == 2) {
            printf("Enter number of zones: ");
            if (scanf("%d", &zoneCount)!=1) { scanf("%*s"); zoneCount=0; continue; }
            for (int i=0;i<zoneCount;i++) {
                zones[i].id = i+1;
                printf("Zone %d -> population severity foodNeed medNeed teamNeed: ", i+1);
                scanf("%d %d %d %d %d", &zones[i].population, &zones[i].severity,
                      &zones[i].foodNeed, &zones[i].medNeed, &zones[i].teamNeed);
                calculatePriority(&zones[i]);
            }
            printf("Enter central stock (food med teams): ");
            scanf("%d %d %d", &foodStock, &medStock, &teamStock);
        }
        else if (choice == 3) {
            if (zoneCount<=0) { printf("No zones loaded.\n"); continue; }
            allocateResources(zones, zoneCount, &foodStock, &medStock, &teamStock);
        }
        else if (choice == 4) {
            printf("Enter number of nodes: "); scanf("%d", &nodes);
            printf("Enter adjacency matrix (%d x %d):\n", nodes, nodes);
            for (int i=0;i<nodes;i++) for (int j=0;j<nodes;j++) scanf("%d", &graph[i][j]);
            printf("Graph saved.\n");
        }
        else if (choice == 5) {
            if (nodes<=0) { printf("Set graph first (option 4 or load sample).\n"); continue; }
            dijkstra(nodes, 0, distArr);
            printf("\n--- Shortest paths from base (0) ---\n");
            for (int i=1;i<nodes;i++) {
                if (distArr[i] >= INF) printf("To %d: no path\n", i);
                else printf("To %d: %d\n", i, distArr[i]);
            }
        }
        else if (choice == 6) {
            printf("Enter number of hospitals: "); scanf("%d", &hospitalCount);
            for (int i=0;i<hospitalCount;i++) {
                printf("Hospital %d name: ", i+1); getchar();
                fgets(hospitals[i].name, sizeof(hospitals[i].name), stdin);
                hospitals[i].name[strcspn(hospitals[i].name, "\n")] = 0;
                printf("Node id (0..%d): ", nodes-1); scanf("%d", &hospitals[i].nodeId);
                printf("Capacity (beds): "); scanf("%d", &hospitals[i].capacity);
                hospitals[i].occupied = 0;
            }
            initHospitals(hospitals, hospitalCount);
        }
        else if (choice == 7) {
            Patient p;
            printf("Enter patient id: "); scanf("%d", &p.id);
            printf("Enter severity (3-Critical,2-Serious,1-Normal): "); scanf("%d", &p.severity);
            printf("Enter patient zone node (0..%d): ", nodes-1); scanf("%d", &p.zoneNode);
            pushPatient(&pheap, p);
            printf("Patient %d queued (sev=%d, zone=%d). Queue size=%d\n", p.id, p.severity, p.zoneNode, pheap.size);
        }
        else if (choice == 8) {
            if (hospitalCount<=0) { printf("No hospitals set.\n"); continue; }
            if (nodes<=0) { printf("Graph not set.\n"); continue; }
            if (isPatientHeapEmpty(&pheap)) { printf("No patients waiting.\n"); continue; }
            admitTopPatient(&pheap, hospitals, hospitalCount, nodes);
        }
        else if (choice == 9) {
            printHospitals(hospitals, hospitalCount);
        }
        else if (choice == 10) {
            int c; printf("1) allocation_log.txt  2) hospital_log.txt\nEnter choice: "); scanf("%d", &c);
            if (c==1) viewFile("allocation_log.txt"); else if (c==2) viewFile("hospital_log.txt"); else printf("Invalid.\n");
        }
        else if (choice == 11) {
            printf("Exiting.\n"); break;
        }
        else {
            printf("Invalid option.\n");
        }
    }

    return 0;
}
