#include <stdio.h>
#include "resource.h"
#include "fileio.h"

void calculatePriority(Zone *z) {
    z->priority = z->population * z->severity;
}

static void swapZone(Zone *a, Zone *b) {
    Zone t = *a; *a = *b; *b = t;
}

void zoneHeapify(Zone arr[], int n, int i) {
    int largest = i;
    int l = 2*i + 1;
    int r = 2*i + 2;
    if (l < n && arr[l].priority > arr[largest].priority) largest = l;
    if (r < n && arr[r].priority > arr[largest].priority) largest = r;
    if (largest != i) {
        swapZone(&arr[i], &arr[largest]);
        zoneHeapify(arr, n, largest);
    }
}

void buildZoneHeap(Zone arr[], int n) {
    for (int i = n/2 - 1; i >= 0; i--) zoneHeapify(arr, n, i);
}

Zone extractMaxZone(Zone arr[], int *n) {
    Zone root = arr[0];
    arr[0] = arr[*n - 1];
    (*n)--;
    zoneHeapify(arr, *n, 0);
    return root;
}

void allocateResources(Zone zones[], int n, int *foodStock, int *medStock, int *teamStock) {
    if (n <= 0) {
        printf("No zones to allocate.\n");
        return;
    }
    Zone temp[MAX_ZONES];
    for (int i = 0; i < n; i++) temp[i] = zones[i];
    buildZoneHeap(temp, n);

    logRunHeader("allocation_log.txt", "Resource Allocation Run");

    printf("\n--- Allocation Results ---\n");
    while (n > 0 && (*foodStock > 0 || *medStock > 0 || *teamStock > 0)) {
        Zone z = extractMaxZone(temp, &n);
        int f = (z.foodNeed <= *foodStock) ? z.foodNeed : *foodStock;
        int m = (z.medNeed <= *medStock) ? z.medNeed : *medStock;
        int t = (z.teamNeed <= *teamStock) ? z.teamNeed : *teamStock;

        *foodStock -= f;
        *medStock -= m;
        *teamStock -= t;

        int unmetF = z.foodNeed - f;
        int unmetM = z.medNeed - m;
        int unmetT = z.teamNeed - t;

        printf("Zone %d -> Got F=%d M=%d T=%d | Unmet: %d %d %d\n",
               z.id, f, m, t, unmetF, unmetM, unmetT);
        logAllocation(z, f, m, t, unmetF, unmetM, unmetT);
    }

    if (*foodStock <=0 && *medStock <=0 && *teamStock <=0) {
        printf("\nCentral stock depleted.\n");
    } else {
        printf("\nRemaining stocks - Food: %d, Med: %d, Teams: %d\n",
               *foodStock, *medStock, *teamStock);
    }
}
