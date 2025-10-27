#ifndef RESOURCE_H
#define RESOURCE_H

#define MAX_ZONES 50

typedef struct {
    int id;
    int population;
    int severity;   // 1..5
    int foodNeed;
    int medNeed;
    int teamNeed;
    int priority;   // computed = population * severity
} Zone;

/* priority helpers */
void calculatePriority(Zone *z);

/* heap for zones */
void buildZoneHeap(Zone arr[], int n);
void zoneHeapify(Zone arr[], int n, int i);
Zone extractMaxZone(Zone arr[], int *n);

/* allocation */
void allocateResources(Zone zones[], int n, int *foodStock, int *medStock, int *teamStock);

#endif
