/** @mainpage proj3.h
 * @brief documentation of proj3.c
 * @author Michal Sova
 */


/**
 * @brief Structure of the object obj_t.
 * @param id an integer identificator
 * @param x,y a float coordinates
 */
struct obj_t {
    int id;
    float x;
    float y;
};


/**
 * @brief Structure of the cluster cluster_t.
 * @param size count of objects in the cluster
 * @param capacity capacity of the cluster
 * @param obj pointer on array of objects @ref obj_t
 */
struct cluster_t {
    int size;
    int capacity;
    struct obj_t *obj;
};


/**
 * @brief initialize cluster
 * allocate space objects @ref obj_t
 * @param c pointer on the cluster @ref cluster_t
 * @param cap capacity of the cluster @ref cluster_t
 * @exception <c == NULL> {if c is not initialized program ends}
 */
void init_cluster(struct cluster_t *c, int cap);


/**
 * @brief clear cluster
 * function free space for array of objects @ref obj_t, initialize on empety cluster
 * @param c pointer on the cluster @ref cluster_t 
 */
void clear_cluster(struct cluster_t *c);


/**
 * Chunk of cluster objects. Value recommended for reallocation.
 */
extern const int CLUSTER_CHUNK;


/**
 * @brief Change capacity of cluster 'c' to 'new_cap'.
 * @param c pointer to cluster @ref cluster_t
 * @param new_cap new capacity of the cluster 'c'
 * @returns resized cluster
 */
struct cluster_t *resize_cluster(struct cluster_t *c, int new_cap);


/**
 * @brief append object 'obj' to the end of the cluster
 * @param c pointer to cluster @ref cluster_t
 * @param obj object @ref obj_t to be added to the end of the cluster
 */
void append_cluster(struct cluster_t *c, struct obj_t obj);


/**
 * @brief to cluster 'c1' appends cluster 'c2'
 * cluster 'c1' will be resized, if it is needed, objects in 'c1' will be sorted by @ref obj_t id, cluster 'c2' will stay untouched
 * @param c1 pointer to the first cluster @ref cluster_t
 * @param c2 pointer to the second cluster @ref cluster_t
 */
void merge_clusters(struct cluster_t *c1, struct cluster_t *c2);


/**
 * @brief remove cluster from array of clusters 'carr'
 * array contains 'narr' clusters, cluster to be reboved is on position 'idx'
 * @param carr pointer to the first cluster @ref cluster_t in the array of clusters
 * @param narr count of clusters in array
 * @param idx index(position) of the cluster to be removed
 * @returns new count of clusters in array
 */
int remove_cluster(struct cluster_t *carr, int narr, int idx);


/**
 * @brief counts Euclideus distance between two objects
 * @param o1 pointer to the first object
 * @param o2 pointer to the second object
 * @returns distance between two objects
 */
float obj_distance(struct obj_t *o1, struct obj_t *o2);


/*
 * @brief counts distance between two clusters
 * @param c1 pointer to the first cluster
 * @param c2 pointer to the second cluster
 * @returns distance between two clusters
 */
float cluster_distance(struct cluster_t *c1, struct cluster_t *c2);


/**
 * @brief find two closest clusters
 * in array 'carr' of size 'narr' finds two closest clusters, saves to memory on adress 'c1' and 'c2' their index
 * @param carr pointer to the first cluster @ref cluster_t in the array of clusters
 * @param narr count of clusters in array
 * @param c1 pointer to the first index of cluster in array
 * @param c2 pointer to the second index of cluster in array
 */
void find_neighbours(struct cluster_t *carr, int narr, int *c1, int *c2);


/**
 * @brief sort clusters by id from the least id to the biggest
 * @param c pointer to the first cluster @ref cluster_t in the array of clusters
 */
void sort_cluster(struct cluster_t *c);


/**
 * @brief print cluster 'c' on stdout 
 * @param c pointer to the cluster @ref cluster_t
 */
void print_cluster(struct cluster_t *c);


/**
 * @brief load objects from file 'filename'
 * for every object saves to the cluster in the array of clusters, allocates space for array and saves to adress of 'arr' pointer to first cluster in array, in case of error saves to adress of 'arr' NULL pointer
 * @param filename pointer to file with objects
 * @param arr pointer to array of clusters
 * @returns count of loaded objects
 */
int load_clusters(char *filename, struct cluster_t **arr);


/**
 * @brief print array of clusters
 * prints first 'narr' clusters in array
 * @param carr pointer to the first cluster @ref cluster_t in the array of clusters
 * @param narr count of clusters in array
 */
void print_clusters(struct cluster_t *carr, int narr);
