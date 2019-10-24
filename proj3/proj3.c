/**
 * Kostra programu pro 3. projekt IZP 2017/18
 *
 * Jednoducha shlukova analyza
 * Unweighted pair-group average
 * https://is.muni.cz/th/172767/fi_b/5739129/web/web/usrov.html
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h> // sqrtf
#include <limits.h> // INT_MAX
#include <string.h>

/*****************************************************************
 * Ladici makra. Vypnout jejich efekt lze definici makra
 * NDEBUG, napr.:
 *   a) pri prekladu argumentem prekladaci -DNDEBUG
 *   b) v souboru (na radek pred #include <assert.h>
 *      #define NDEBUG
 */
#ifdef NDEBUG
#define debug(s)
#define dfmt(s, ...)
#define dint(i)
#define dfloat(f)
#else

// vypise ladici retezec
#define debug(s) printf("- %s\n", s)

// vypise formatovany ladici vystup - pouziti podobne jako printf
#define dfmt(s, ...) printf(" - "__FILE__":%u: "s"\n",__LINE__,__VA_ARGS__)

// vypise ladici informaci o promenne - pouziti dint(identifikator_promenne)
#define dint(i) printf(" - " __FILE__ ":%u: " #i " = %d\n", __LINE__, i)

// vypise ladici informaci o promenne typu float - pouziti
// dfloat(identifikator_promenne)
#define dfloat(f) printf(" - " __FILE__ ":%u: " #f " = %g\n", __LINE__, f)

#endif

/*****************************************************************
 * Deklarace potrebnych datovych typu:
 *
 * TYTO DEKLARACE NEMENTE
 *
 *   struct obj_t - struktura objektu: identifikator a souradnice
 *   struct cluster_t - shluk objektu:
 *      pocet objektu ve shluku,
 *      kapacita shluku (pocet objektu, pro ktere je rezervovano
 *          misto v poli),
 *      ukazatel na pole shluku.
 */

struct obj_t {
    int id;
    float x;
    float y;
};

struct cluster_t {
    int size;
    int capacity;
    struct obj_t *obj;
};

/*****************************************************************
 * Deklarace potrebnych funkci.
 *
 * PROTOTYPY FUNKCI NEMENTE
 *
 * IMPLEMENTUJTE POUZE FUNKCE NA MISTECH OZNACENYCH 'TODO'
 *
 */

/*
 Inicializace shluku 'c'. Alokuje pamet pro cap objektu (kapacitu).
 Ukazatel NULL u pole objektu znamena kapacitu 0.
*/
void init_cluster(struct cluster_t *c, int cap)
{
    assert(c != NULL);
    assert(cap >= 0);

    c->size = 0;
    c->capacity = cap;
    c->obj = malloc(cap * sizeof(struct obj_t));
    if(c->obj == NULL) {
	    c->capacity = 0;
    }
}

/*
 Odstraneni vsech objektu shluku a inicializace na prazdny shluk.
 */
void clear_cluster(struct cluster_t *c)
{
    if(c->obj == NULL) {
	    return;
    }
    free(c->obj);
    c->size = 0;
    c->capacity = 0;
    c->obj = NULL;
}

/// Chunk of cluster objects. Value recommended for reallocation.
const int CLUSTER_CHUNK = 10;

/*
 Zmena kapacity shluku 'c' na kapacitu 'new_cap'.
 */
struct cluster_t *resize_cluster(struct cluster_t *c, int new_cap)
{
    // TUTO FUNKCI NEMENTE
    assert(c);
    assert(c->capacity >= 0);
    assert(new_cap >= 0);

    if (c->capacity >= new_cap)
        return c;

    size_t size = sizeof(struct obj_t) * new_cap;

    void *arr = realloc(c->obj, size);
    if (arr == NULL)
        return NULL;

    c->obj = (struct obj_t*)arr;
    c->capacity = new_cap;
    return c;
}

/*
 Prida objekt 'obj' na konec shluku 'c'. Rozsiri shluk, pokud se do nej objekt
 nevejde.
 */
void append_cluster(struct cluster_t *c, struct obj_t obj)
{
    if(c->capacity <= c->size) {
	    c = resize_cluster(c, c->capacity + CLUSTER_CHUNK);
    }
    c->obj[c->size] = obj;
    c->size++;
}

/*
 Seradi objekty ve shluku 'c' vzestupne podle jejich identifikacniho cisla.
 */
void sort_cluster(struct cluster_t *c);

/*
 Do shluku 'c1' prida objekty 'c2'. Shluk 'c1' bude v pripade nutnosti rozsiren.
 Objekty ve shluku 'c1' budou serazeny vzestupne podle identifikacniho cisla.
 Shluk 'c2' bude nezmenen.
 */

void merge_clusters(struct cluster_t *c1, struct cluster_t *c2)
{
    assert(c1 != NULL);
    assert(c2 != NULL);
    for(int i = 0; i < c2->size; i++) {
	    append_cluster(c1, c2->obj[i]);
    }
    sort_cluster(c1);
}


/**********************************************************************/
/* Prace s polem shluku */

/*
 Odstrani shluk z pole shluku 'carr'. Pole shluku obsahuje 'narr' polozek
 (shluku). Shluk pro odstraneni se nachazi na indexu 'idx'. Funkce vraci novy
 pocet shluku v poli.
*/


int remove_cluster(struct cluster_t *carr, int narr, int idx)
{
    assert(idx < narr);
    assert(narr > 0);
	clear_cluster(&carr[idx]);
	for(int i = idx; i < narr - 1; i++) {
		carr[i] = carr[i + 1];
	}
    return narr - 1;
}




/*
 Pocita Euklidovskou vzdalenost mezi dvema objekty.
 */

float obj_distance(struct obj_t *o1, struct obj_t *o2)
{
    assert(o1 != NULL);
    assert(o2 != NULL);
    double a, b;
    a = pow(o1->x - o2->x, 2.0);
    b = pow(o1->y - o2->y, 2.0);
    return sqrt(a + b);
}


/*
 Pocita vzdalenost dvou shluku.
*/

float cluster_distance(struct cluster_t *c1, struct cluster_t *c2)
{
    assert(c1 != NULL);
    assert(c1->size > 0);
    assert(c2 != NULL);
    assert(c2->size > 0);
    float min = INFINITY;
    for(int i = 0; i < c1->size; i++) {
	    for(int j = 0; j < c2->size; j++) {
		    min = fminf(min, obj_distance(&c1->obj[i], &c2->obj[j]));
	    }
    }
    return min;
}


/*
 Funkce najde dva nejblizsi shluky. V poli shluku 'carr' o velikosti 'narr'
 hleda dva nejblizsi shluky. Nalezene shluky identifikuje jejich indexy v poli
 'carr'. Funkce nalezene shluky (indexy do pole 'carr') uklada do pameti na
 adresu 'c1' resp. 'c2'.
*/

void find_neighbours(struct cluster_t *carr, int narr, int *c1, int *c2)
{
    assert(narr > 0);
    float distance = INFINITY;
    float min = distance;
    for(int i = 0; i < narr - 1; i++) {
	    for(int j = i + 1; j < narr; j++) {
		    if(carr[i].size == 0 || carr[j].size == 0) {
			    continue;
		    }
		    distance = cluster_distance(&carr[i], &carr[j]);
		    if(min > distance) {
			    min = distance;
			    *c1 = i;
			    *c2 = j;
		    }
	    }
    }
}


// pomocna funkce pro razeni shluku
static int obj_sort_compar(const void *a, const void *b)
{
    // TUTO FUNKCI NEMENTE
    const struct obj_t *o1 = (const struct obj_t *)a;
    const struct obj_t *o2 = (const struct obj_t *)b;
    if (o1->id < o2->id) return -1;
    if (o1->id > o2->id) return 1;
    return 0;
}

/*
 Razeni objektu ve shluku vzestupne podle jejich identifikatoru.
*/
void sort_cluster(struct cluster_t *c)
{
    // TUTO FUNKCI NEMENTE
    qsort(c->obj, c->size, sizeof(struct obj_t), &obj_sort_compar);
}

/*
 Tisk shluku 'c' na stdout.
*/
void print_cluster(struct cluster_t *c)
{
    // TUTO FUNKCI NEMENTE
    for (int i = 0; i < c->size; i++)
    {
        if (i) putchar(' ');
        printf("%d[%g,%g]", c->obj[i].id, c->obj[i].x, c->obj[i].y);
    }
    putchar('\n');
}

void free_arr(struct cluster_t *carr, int narr) {
	assert(carr);
	for(int i = 0; i < narr; i++) {
		clear_cluster(&carr[i]);
	}
	free(carr);
}

int load_int(char *str, int *num) {
	*num = 0;
	for(unsigned i = 0; i < strlen(str); i++) {
		if(str[i] >= '0' && str[i] <= '9') {
			*num = *num * 10 + str[i] - '0';
		}
		else {
			return -1;
		}
	}
	return 0;
}

/*
 Ze souboru 'filename' nacte objekty. Pro kazdy objekt vytvori shluk a ulozi
 jej do pole shluku. Alokuje prostor pro pole vsech shluku a ukazatel na prvni
 polozku pole (ukalazatel na prvni shluk v alokovanem poli) ulozi do pameti,
 kam se odkazuje parametr 'arr'. Funkce vraci pocet nactenych objektu (shluku).
 V pripade nejake chyby uklada do pameti, kam se odkazuje 'arr', hodnotu NULL.
*/
int load_clusters(char *filename, struct cluster_t **arr)
{
    assert(arr != NULL);
    char buffer[100];
    int count = 0;
    FILE *source_file = fopen(filename, "r");
    struct obj_t object;
    if(source_file == NULL) {
	    fprintf(stderr, "Nepodařilo se otevřít soubor.\n ");
	    return 0;
    }
    if(fscanf(source_file, "%s", buffer) == 0) {
	    fprintf(stderr, "Nepodařilo se načíst 1. řádek.\n");
	    fclose(source_file);
	    return 0;
    }
    if(strncmp(buffer, "count=", 6) != 0) {
	    fprintf(stderr, "1. řádek je ve špatném tvaru.\n");
	    fclose(source_file);
	    return 0;
    }
    if(load_int(&buffer[6], &count) == -1) {
	    fprintf(stderr, "špatně zadané číslo (count=N).\n");
	    fclose(source_file);
	    return 0;
    }
    *arr = malloc(count * sizeof(struct cluster_t));
	if(*arr == NULL) {
		fprintf(stderr, "Nepodařilo se alokovat pole.\n");
	}
    for(int i = 0; i < count; i++) {
	    if(fscanf(source_file, "%d %f %f", &object.id, &object.x, &object.y) == 0) {
		    fprintf(stderr, "Nepodařilo se načíst %d.řádek.\n", i + 2);
		    fclose(source_file);
		    return i;
	    }
		init_cluster(&(*arr)[i], 1);
	    append_cluster(&(*arr)[i], object);
    }
    fclose(source_file);
    return count;
}

/*
 Tisk pole shluku. Parametr 'carr' je ukazatel na prvni polozku (shluk).
 Tiskne se prvnich 'narr' shluku.
*/
void print_clusters(struct cluster_t *carr, int narr)
{
    printf("Clusters:\n");
    for (int i = 0; i < narr; i++)
    {
        printf("cluster %d: ", i);
        print_cluster(&carr[i]);
    }
}

int main(int argc, char *argv[]) 
{
    struct cluster_t *clusters;
    int arr_index1 = -1;
    int	arr_index2 = -1;
    if(argc < 2) {
		fprintf(stderr, "Zadáno málo argumentů.\n");
	    return -1;
    }
    int result_clusters = 1;
    if(argc > 2 && load_int(argv[2], &result_clusters) == -1) {
		fprintf(stderr, "2. argument má být celé kladné číslo,\n");
		return -1;
    }
    int loaded_clusters = load_clusters(argv[1], &clusters);
    if(loaded_clusters < result_clusters) {
		fprintf(stderr, "Počet načtených objektů je menší, než by mělo být výsledných.\n");
		free_arr(clusters, loaded_clusters);
	    return -1;
    }
    while(loaded_clusters > result_clusters) {
	    find_neighbours(clusters, loaded_clusters, &arr_index1, &arr_index2);
		if(arr_index1 == -1 || arr_index2 == -1) {
			fprintf(stderr, "Something is wrong.\n");
			return -1;
		}
	    merge_clusters(&clusters[arr_index1], &clusters[arr_index2]);
	    loaded_clusters = remove_cluster(clusters, loaded_clusters, arr_index2);
		arr_index1 = -1;
		arr_index2 = -1;
    }
    print_clusters(clusters, result_clusters);
	free_arr(clusters, result_clusters);
    return 0;
}
