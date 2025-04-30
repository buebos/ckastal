#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_VERTICES 100
#define MAX_EDGES 1000

typedef struct AdjListNode {
    int vertex;
    int weight;
} AdjListNode;

typedef struct AdjList {
    AdjListNode* array[MAX_VERTICES];
    int size[MAX_VERTICES];
} AdjList;

typedef struct HeapNode {
    int paths;
    int vertex;
} HeapNode;

typedef struct MinHeap {
    HeapNode* nodes[MAX_EDGES];
    int size;
} MinHeap;

typedef struct Path {
    int cost;
    int nodes[MAX_VERTICES];
    int nodes_len;
} Path;

typedef struct {
    char label;
    int index;
} LabelMap;

int get_or_add_vertex_index(char label, LabelMap* map, int* map_size) {
    // Normalize label to uppercase for case-insensitive mapping
    if (label >= 'a' && label <= 'z') label -= 32;

    for (int i = 0; i < *map_size; i++) {
        if (map[i].label == label)
            return map[i].index;
    }
    map[*map_size].label = label;
    map[*map_size].index = *map_size;
    (*map_size)++;
    return *map_size - 1;
}

char get_label_from_index(int index, LabelMap* map, int map_size) {
    for (int i = 0; i < map_size; i++) {
        if (map[i].index == index)
            return map[i].label;
    }
    return '?';
}

MinHeap* create_min_heap() {
    MinHeap* heap = (MinHeap*)malloc(sizeof(MinHeap));
    heap->size = 0;
    return heap;
}

void swap(HeapNode** a, HeapNode** b) {
    HeapNode* temp = *a;
    *a = *b;
    *b = temp;
}

void heapify_up(MinHeap* heap, int idx) {
    while (idx > 0) {
        int parent = (idx - 1) / 2;
        if (heap->nodes[parent]->paths <= heap->nodes[idx]->paths) break;
        swap(&heap->nodes[parent], &heap->nodes[idx]);
        idx = parent;
    }
}

void heapify_down(MinHeap* heap, int idx) {
    int smallest = idx;
    int left = 2 * idx + 1, right = 2 * idx + 2;

    if (left < heap->size && heap->nodes[left]->paths < heap->nodes[smallest]->paths)
        smallest = left;
    if (right < heap->size && heap->nodes[right]->paths < heap->nodes[smallest]->paths)
        smallest = right;

    if (smallest != idx) {
        swap(&heap->nodes[smallest], &heap->nodes[idx]);
        heapify_down(heap, smallest);
    }
}

void push(MinHeap* heap, int paths, int vertex) {
    HeapNode* node = (HeapNode*)malloc(sizeof(HeapNode));
    node->paths = paths;
    node->vertex = vertex;
    heap->nodes[heap->size] = node;
    heapify_up(heap, heap->size);
    heap->size++;
}

HeapNode* pop(MinHeap* heap) {
    if (heap->size == 0) return NULL;
    HeapNode* min = heap->nodes[0];
    heap->nodes[0] = heap->nodes[--heap->size];
    heapify_down(heap, 0);
    return min;
}

void add_edge(AdjList* adj, int u, int v, int wt) {
    adj->array[u][adj->size[u]].vertex = v;
    adj->array[u][adj->size[u]].weight = wt;
    adj->size[u]++;

    adj->array[v][adj->size[v]].vertex = u;
    adj->array[v][adj->size[v]].weight = wt;
    adj->size[v]++;
}

void dijkstra(int v_count, int edge_list[][3], int edge_count, int src, Path* paths) {
    AdjList adj;

    for (int i = 0; i < v_count; ++i) {
        adj.array[i] = (AdjListNode*)malloc(MAX_VERTICES * sizeof(AdjListNode));
        adj.size[i] = 0;
    }

    for (int i = 0; i < edge_count; ++i) {
        add_edge(&adj, edge_list[i][0], edge_list[i][1], edge_list[i][2]);
    }

    for (int i = 0; i < v_count; ++i) {
        paths[i].cost = INT_MAX;
        paths[i].nodes_len = 0;
    }

    paths[src].cost = 0;

    MinHeap* heap = create_min_heap();
    push(heap, 0, src);

    while (heap->size > 0) {
        HeapNode* node = pop(heap);
        int u = node->vertex;
        free(node);

        for (int i = 0; i < adj.size[u]; ++i) {
            int v = adj.array[u][i].vertex;
            int weight = adj.array[u][i].weight;

            if (paths[v].cost > paths[u].cost + weight) {
                paths[v].cost = paths[u].cost + weight;

                paths[v].nodes_len = 0;
                for (int j = 0; j < paths[u].nodes_len; j++) {
                    paths[v].nodes[j] = paths[u].nodes[j];
                    paths[v].nodes_len++;
                }

                paths[v].nodes[paths[v].nodes_len] = u;
                paths[v].nodes_len++;

                push(heap, paths[v].cost, v);
            }
        }
    }

    for (int i = 0; i < v_count; ++i) {
        free(adj.array[i]);
    }

    free(heap);
}

int main(void) {
    int edge_count;
    int edge_list[MAX_EDGES][3];
    Path paths[MAX_VERTICES];
    LabelMap label_map[MAX_VERTICES];
    int label_count = 0;
    char u_label, v_label, src_label;
    int src;

    printf("Enter number of edges: ");
    scanf("%d", &edge_count);

    printf("Enter edges (u v weight):\n");
    for (int i = 0; i < edge_count; ++i) {
        int weight;
        scanf(" %c %c %d", &u_label, &v_label, &weight);
        int u = get_or_add_vertex_index(u_label, label_map, &label_count);
        int v = get_or_add_vertex_index(v_label, label_map, &label_count);
        edge_list[i][0] = u;
        edge_list[i][1] = v;
        edge_list[i][2] = weight;
    }

    printf("Enter source vertex: ");
    scanf(" %c", &src_label);
    src = get_or_add_vertex_index(src_label, label_map, &label_count);

    dijkstra(label_count, edge_list, edge_count, src, paths);

    printf("\n%-10s | %-10s | Path\n", "Target", "Cost");
    printf("--------------------------------------------\n");

    for (int i = 0; i < label_count; ++i) {
        Path* path = &paths[i];
        printf("%-10c | %-10d | ", get_label_from_index(i, label_map, label_count), path->cost);

        for (int j = 0; j < path->nodes_len; j++) {
            printf("%c -> ", get_label_from_index(path->nodes[j], label_map, label_count));
        }

        printf("%c\n", get_label_from_index(i, label_map, label_count));
    }

    return 0;
}
