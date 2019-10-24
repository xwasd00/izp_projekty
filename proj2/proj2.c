/******************************************************************************************************/
/*                                                                                                    */
/*                               2. projekt do IZP - Iterační výpočty                                 */
/*                                     vytvořil: Michal Sova					                      */
/*                                        login: xsovam00                                             */
/*                                    vytvořeno: listopad 2018                                        */
/*                                                                                                    */
/******************************************************************************************************/

#include<stdio.h>
#include<math.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>

double taylor_log(double x, unsigned int n);
double cfrac_log(double x, unsigned int n);
double taylor_pow(double x, double y, unsigned int n);
double taylorcf_pow(double x, double y, unsigned int n);
int load_double(char *str, double *p_num, int arg_num);
int load_long_int(char *str, long int *p_num, int arg_num);
int print_log(double x, unsigned int num_of_iterations);
int print_pow(double x, double y, unsigned int num_of_iterations);

int main(int argc, char *argv[]) {
	//pokud je zadán nesprávný počet argumentů, program skončí s chybovým hlášením
	if(argc < 4 || argc > 5) {
		fprintf(stderr, "Zadán nesprávný počet argumentů.(Musí být zadáno 3 až 4 argumenty.)\n");
		return -1;
	}
	long int num_of_iterations = 0;
	double x = 0.0;
	double y = 0.0;
	//tato podmínka je pro logaritmické funkce - tzn. 1. argument musí být '--log' a další dva čísla
	if(strcmp(argv[1], "--log") == 0 && argc == 4) {
		//načtení proměnné typu double z argumentu
		//poslední parametr funkce load_double je pro chybové hlášení
		//v případě že argument není číslo, vrací funkce load_double -1
		if(load_double(argv[2], &x, 2) == -1) 
			return -1;
		//načtení proměnné, tentokrát typu long int - pro počet iterací
		//v případě nenačtení vrací funkce load_long_int -1
		if(load_long_int(argv[3], &num_of_iterations, 3) == -1)
			return -1;
		//v této funkci se vypočítají a následně vytisknou všechny implementované funkce log
		//long int se přetypovává na unsigned int
		print_log(x, (unsigned)num_of_iterations);
		return 0;
	}
	if(strcmp(argv[1], "--pow") == 0 && argc == 5) {
		//načtení 1. proměnné
		if(load_double(argv[2], &x, 2) == -1)
                        return -1;
		//načtení 2. proměnné
		if(load_double(argv[3], &y, 3) == -1)
                        return -1;
		//načtení počtu iterací
                if(load_long_int(argv[4], &num_of_iterations, 4) == -1)
                        return -1;
		//v této funkci se vypočítají a následně tisknou všechny implementované funkce pow
		print_pow(x, y, (unsigned)num_of_iterations);
		return 0;
	}
	//pokud nejou zadány argumenty ve správném tvaru, program se ukončí a vypíše chybovým hlášením
	fprintf(stderr, "Zadány nesprávné argumenty.\n");
	fprintf(stderr, "Argumenty musí být zadány ve tvaru '--log X N' nebo '--pow X Y N', ");
	fprintf(stderr, "kde X, Y a N jsou čísla.\n");
	return -1;
}

double taylor_log(double x, unsigned int n) {
	//1. a 2. podmínka vycházejí z průběhu dané funkce
	if(x == INFINITY)
		return x;
	if(x == 0)
		return -INFINITY;
	//3. podmínka nastane, když x není číslo nebo x je záporné
	//protože -x není pro funkci log(x) definováno
	if(x < 0 || isnan(x) != 0)
		return NAN;

	double log = 0.0;
        double T = 1.0;
	double constant_num = 0.0;
	//pro 0 < x < 1 je použit tento předpis
	if(x > 0 && x < 1) {
		constant_num = 1 - x;
		for(unsigned i = 0; i < n; i++) {
			T = T * constant_num;
			log = log - T / (i + 1);
		}
	}
	//pro 1 < x je použit tento předpis
	if(x > 1) {
		constant_num = (x - 1) / x;
		for(unsigned i = 0; i < n; i++) {
			T = T * constant_num;
			log = log + T / (i + 1);
		}
	}
	return log;
}

double cfrac_log(double x, unsigned int n) {
	//1. a 2. podmínka vycházejí z průběhu funkce log
	if(x == INFINITY)
		return x;
	if(x == 0)
                return -INFINITY;
	//3. pomínka nastane, když x není číslo nebo x je záporné
	if(x < 0 || isnan(x) != 0)
		return NAN;

	double log = 0.0;
	double z = (x - 1) / (x + 1);
	for(unsigned i = n - 1; i > 0; i--) {
		log = (i*i * z*z) / ((2 * i + 1) - log);
	}
	return (2 * z)/(1 - log);
}

double taylor_pow(double x, double y, unsigned int n) {
	//1. pomínka nastane, když x nebo y není číslo
	if(isnan(x) != 0 || isnan(y) != 0)
                return NAN;
	//2. a 3. podmínka vycházejí z průběhu funkce x^y
	if(y == -INFINITY)
                return 0.0;
	if(x == INFINITY || y == INFINITY)
		return INFINITY;

	double pow = 1.0;
	double T = 1.0;
	double t_log = taylor_log(x, n);
	for(unsigned i = 1; i < n; i++) {
		T = T * ((y * t_log) / i);
		pow = pow + T;
	}
	return pow;
}

double taylorcf_pow(double x, double y, unsigned int n) {
	//1. pomínka nastane, když x nebo y není číslo
	if(isnan(x) != 0 || isnan(y) != 0)
                return NAN;
	//2. a 3. podmínka vycházejí z průběhu funkce x^y
	if(y == -INFINITY)
                return 0.0;
	if(x == INFINITY || y == INFINITY)
                return INFINITY;

	double pow = 1.0;
        double T = 1.0;
	double cf_log = cfrac_log(x, n);
        for(unsigned i = 1; i < n; i++) {
                T = T * ((y * cf_log) / i);
                pow = pow + T;
        }
        return pow;
}

int load_double(char *str, double *p_num, int arg_num) {
	char *end;
	errno = 0;
	*p_num = strtod(str, &end);
	if(end[0] != '\0') {
		fprintf(stderr, "%d. argument je zadán špatně.\n", arg_num);
		fprintf(stderr, "%d. argument musí být u této funkce číslo.\n", arg_num);
		return -1;
	}
	if(errno != 0){
		fprintf(stderr, "Číslo ve %d. argumentu je mimo rozsah.\n", arg_num);
		return -1;
	}
	return 0;
}

int load_long_int(char *str, long int *p_num, int arg_num) {
	char *end;
	errno = 0;
	*p_num = strtol(str, &end, 10);
	if(end[0] != '\0' || *p_num <= 0) {
		fprintf(stderr, "%d. argument je zadán špatně.\n", arg_num);
		fprintf(stderr, "%d. argument musí být u této funkce celé kladné číslo.\n", arg_num);
		return -1;
	}
	if(errno != 0) {
		fprintf(stderr,"Číslo ve %d. argumentu je mimo rozsah.\n", arg_num);
		return -1;
	}
	return 0;
}

int print_log(double x, unsigned int num_of_iterations) {
	double result = 0.0;
	result = log(x);
	printf("       log(%g) = %.12g\n", x, result);
	result = cfrac_log(x, num_of_iterations);
	printf(" cfrac_log(%g) = %.12g\n", x, result);
	result = taylor_log(x, num_of_iterations);
	printf("taylor_log(%g) = %.12g\n", x, result);
	return 0;
}

int print_pow(double x, double y, unsigned int num_of_iterations) {
	double result = 0.0;
	result = pow(x, y);
	printf("         pow(%g,%g) = %.12g\n", x, y, result);
	result = taylor_pow(x, y, num_of_iterations);
	printf("  taylor_pow(%g,%g) = %.12g\n", x, y, result);
	result = taylorcf_pow(x, y, num_of_iterations);
	printf("taylorcf_pow(%g,%g) = %.12g\n", x, y, result);
	return 0;
}
