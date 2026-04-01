#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>
#include "tp1.h"
#include "constantes.h"

const char IDS_TIPOS[MAX_TIPOS][MAX_NOM_TIPO] = { "ELEC", "FUEG", "PLAN",
						  "AGUA", "NORM", "FANT",
						  "PSI",  "LUCH" };

struct tp1 {
	struct pokemon **pokemones;
	int cantidad_pokemones;
};

int tipo_string_a_id(char *tipo)
{
	int i = 0;
	int id_tipo = -1;
	bool buscando = true;

	while (buscando && i < MAX_TIPOS) {
		if (strcmp(IDS_TIPOS[i], tipo) == 0) {
			id_tipo = i;
			buscando = false;
		}
		i++;
	}

	return id_tipo;
}

void tp1_ordenar_pokemon(tp1_t *tp1)
{
	if (tp1 == NULL) {
		return;
	}
	if (tp1->cantidad_pokemones <= 1) {
		return;
	}

	for (int i = 0; i < tp1->cantidad_pokemones - 1; i++) {
		for (int j = 0; j < tp1->cantidad_pokemones - i - 1; j++) {
			if (strcasecmp(tp1->pokemones[j]->nombre,
				       tp1->pokemones[j + 1]->nombre) > 0) {
				struct pokemon *aux = tp1->pokemones[j];
				tp1->pokemones[j] = tp1->pokemones[j + 1];
				tp1->pokemones[j + 1] = aux;
			}
		}
	}
}

bool pokemon_es_valido(struct pokemon pokemon, int tipo_aux)
{
	return pokemon.nombre != NULL && tipo_aux >= 0 && tipo_aux <= 7 &&
	       pokemon.ataque > 0 && pokemon.defensa > 0 &&
	       pokemon.velocidad > 0;
}

size_t contar_campos(char *linea)
{
	size_t i = 0;

	char *actual = strchr(linea, CHAR_DIVISOR_CAMPOS);
	while (actual != NULL) {
		i++;
		actual = strchr(actual + 1, CHAR_DIVISOR_CAMPOS);
	}

	return i + 1;
}

bool linea_es_valida(int leido, size_t cnt_campos)
{
	return leido == CANT_DESEADA_LECTURA &&
	       cnt_campos == CANT_DESEADA_LECTURA;
}

int tp1_parsear_linea(char *linea, struct pokemon **nuevo_pokemon)
{
	char *buffer_tipo = calloc(1, strlen(linea) + 1 * sizeof(char));
	if (buffer_tipo == NULL) {
		return ERR_MEMORIA;
	}
	char *buffer_nombre = calloc(1, strlen(linea) + 1 * sizeof(char));
	if (buffer_nombre == NULL) {
		free(buffer_tipo);
		return ERR_MEMORIA;
	}

	size_t cnt_campos = contar_campos(linea);

	struct pokemon *poke = calloc(1, sizeof(struct pokemon));

	int leido = sscanf(linea, FORMATO_LECTURA, buffer_nombre, buffer_tipo,
			   &(poke->ataque), &(poke->defensa),
			   &(poke->velocidad));
	if (!linea_es_valida(leido, cnt_campos)) {
		free(buffer_tipo);
		free(buffer_nombre);
		free(poke);
		return LINEA_INVALIDA;
	}

	poke->nombre = buffer_nombre;
	int tipo_aux = tipo_string_a_id(buffer_tipo);
	poke->tipo = tipo_aux;

	if (!pokemon_es_valido(*poke, tipo_aux)) {
		free(buffer_tipo);
		free(buffer_nombre);
		free(poke);
		return LINEA_INVALIDA;
	}

	*nuevo_pokemon = poke;
	free(buffer_tipo);

	return SIN_ERRORES;
}

int tp1_leer_linea(FILE *archivo, char **linea)
{
	char *buffer = calloc(1, TAM_INICIAL_BUFFER * sizeof(char));
	if (buffer == NULL) {
		return ERR_MEMORIA;
	}
	char *ptr_aux_buffer = NULL;
	unsigned long tam_actual_buffer = TAM_INICIAL_BUFFER;

	int i = 0;
	int leido = fgetc(archivo);

	while (leido != INDICADOR_NUEVA_LINEA && leido != EOF) {
		buffer[i] = (char)leido;
		i++;

		if (i >= tam_actual_buffer) {
			tam_actual_buffer *= AJUSTE_TAM_BUFFER;
			ptr_aux_buffer = realloc(buffer, tam_actual_buffer *
								 sizeof(char));
			if (ptr_aux_buffer == NULL) {
				return ERR_MEMORIA;
			} else {
				buffer = ptr_aux_buffer;
			}
		}

		leido = fgetc(archivo);
	}

	if (i == 0) {
		if (leido == INDICADOR_NUEVA_LINEA) {
			free(buffer);
			return LINEA_VACIA;
		} else if (leido == EOF) {
			free(buffer);
			return EOF;
		}
	}

	buffer[i] = '\0';
	*linea = buffer;

	return SIN_ERRORES;
}

int agregar_pokemon(tp1_t *tp1, int *tam_buffer, struct pokemon *nuevo_pokemon)
{
	struct pokemon **ptr_aux_buffer = NULL;

	if (tp1->cantidad_pokemones >= *tam_buffer) {
		*tam_buffer *= AJUSTE_TAM_BUFFER;

		ptr_aux_buffer = realloc(tp1->pokemones,
					 (size_t)(*tam_buffer) *
						 sizeof(struct pokemon *));
		if (ptr_aux_buffer == NULL) {
			return ERR_MEMORIA;
		} else {
			tp1->pokemones = ptr_aux_buffer;
		}
	}

	tp1->pokemones[tp1->cantidad_pokemones] = nuevo_pokemon;
	tp1->cantidad_pokemones++;

	return SIN_ERRORES;
}

bool pokemon_es_repetido(tp1_t *tp1, struct pokemon *nuevo_pokemon)
{
	if (tp1 == NULL) {
		return false;
	}

	bool es_repetido = false;
	int i = 0;

	while (i < tp1->cantidad_pokemones && !es_repetido) {
		if (strcasecmp(nuevo_pokemon->nombre,
			       tp1->pokemones[i]->nombre) == 0) {
			es_repetido = true;
		}
		i++;
	}

	return es_repetido;
}

bool puedo_seguir_leyendo(int resultado_lectura)
{
	return resultado_lectura != LINEA_VACIA && resultado_lectura != EOF;
}

tp1_t *tp1_leer_archivo(const char *nombre)
{
	if (nombre == NULL) {
		return NULL;
	}

	FILE *archivo = fopen(nombre, "r");
	if (archivo == NULL) {
		return NULL;
	}

	tp1_t *tp1 = calloc(1, sizeof(tp1_t));
	if (tp1 == NULL) {
		fclose(archivo);
		return NULL;
	}

	tp1->pokemones =
		calloc(1, TAM_INICIAL_BUFFER * sizeof(struct pokemon *));
	if (tp1->pokemones == NULL) {
		fclose(archivo);
		free(tp1);
		return NULL;
	}
	int tam_actual_buffer = TAM_INICIAL_BUFFER;

	bool leyendo = true;

	while (leyendo) {
		char *linea = NULL;
		int resultado_lectura = tp1_leer_linea(archivo, &linea);

		if (resultado_lectura == ERR_MEMORIA) {
			tp1_destruir(tp1);
			fclose(archivo);
			return NULL;
		}
		if (resultado_lectura == EOF) {
			leyendo = false;
		}

		else if (puedo_seguir_leyendo(resultado_lectura)) {
			struct pokemon *nuevo_pokemon;

			int resultado_parseo =
				tp1_parsear_linea(linea, &nuevo_pokemon);

			if (resultado_parseo == ERR_MEMORIA) {
				tp1_destruir(tp1);
				fclose(archivo);
				return NULL;
			}
			if (resultado_parseo == SIN_ERRORES) {
				if (!pokemon_es_repetido(tp1, nuevo_pokemon)) {
					int resultado_agregar = agregar_pokemon(
						tp1, &tam_actual_buffer,
						nuevo_pokemon);

					if (resultado_agregar == ERR_MEMORIA) {
						tp1_destruir(tp1);
						fclose(archivo);
						return NULL;
					}
				} else {
					free(nuevo_pokemon->nombre);
					free(nuevo_pokemon);
				}
			}
		}

		free(linea);
	}

	fclose(archivo);

	if (tp1->cantidad_pokemones == 0) {
		free(tp1->pokemones);
		tp1->pokemones = NULL;
	} else {
		tp1_ordenar_pokemon(tp1);
	}

	return tp1;
}

size_t tp1_cantidad(tp1_t *tp1)
{
	if (tp1 == NULL || tp1->cantidad_pokemones < 0) {
		return 0;
	}

	return (size_t)tp1->cantidad_pokemones;
}

tp1_t *tp1_guardar_archivo(tp1_t *tp1, const char *nombre)
{
	if (tp1 == NULL || nombre == NULL) {
		return NULL;
	}

	FILE *archivo = fopen(nombre, "w");
	if (archivo == NULL) {
		return NULL;
	}

	for (int i = 0; i < tp1->cantidad_pokemones; i++) {
		fprintf(archivo, FORMATO_ESCRITURA, tp1->pokemones[i]->nombre,
			IDS_TIPOS[tp1->pokemones[i]->tipo],
			tp1->pokemones[i]->ataque, tp1->pokemones[i]->defensa,
			tp1->pokemones[i]->velocidad);
	}

	fclose(archivo);
	return tp1;
}

int copiar_pokemon(tp1_t *tp1, struct pokemon *pokemon)
{
	struct pokemon *poke_copia = calloc(1, sizeof(struct pokemon));
	if (poke_copia == NULL) {
		return ERR_MEMORIA;
	}

	poke_copia->nombre =
		calloc(1, strlen(pokemon->nombre) + 1 * sizeof(char));
	if (poke_copia->nombre == NULL) {
		free(poke_copia);
		return ERR_MEMORIA;
	}

	strcpy(poke_copia->nombre, pokemon->nombre);
	poke_copia->ataque = pokemon->ataque;
	poke_copia->defensa = pokemon->defensa;
	poke_copia->velocidad = pokemon->velocidad;
	poke_copia->tipo = pokemon->tipo;

	tp1->pokemones[tp1->cantidad_pokemones] = poke_copia;
	tp1->cantidad_pokemones++;

	return SIN_ERRORES;
}

tp1_t *tp1_filtrar_tipo(tp1_t *un_tp, enum tipo_pokemon tipo)
{
	if (un_tp == NULL || tipo < 0 || tipo > 7) {
		return NULL;
	}
	if (un_tp->cantidad_pokemones <= 0) {
		return NULL;
	}

	tp1_t *nuevo_tp1 = calloc(1, sizeof(tp1_t));
	if (nuevo_tp1 == NULL) {
		return NULL;
	}

	nuevo_tp1->pokemones =
		calloc(1, tp1_cantidad(un_tp) * sizeof(struct pokemon *));
	if (nuevo_tp1->pokemones == NULL) {
		free(nuevo_tp1);
		return NULL;
	}
	struct pokemon **pokemones_a_copiar =
		calloc(1, tp1_cantidad(un_tp) * sizeof(struct pokemon *));
	if (pokemones_a_copiar == NULL) {
		free(nuevo_tp1->pokemones);
		free(nuevo_tp1);
		return NULL;
	}

	int total_a_copiar = 0;
	for (int i = 0; i < un_tp->cantidad_pokemones; i++) {
		if (un_tp->pokemones[i]->tipo == tipo) {
			pokemones_a_copiar[total_a_copiar] =
				un_tp->pokemones[i];
			total_a_copiar++;
		}
	}

	for (int i = 0; i < total_a_copiar; i++) {
		int resultado_copia =
			copiar_pokemon(nuevo_tp1, pokemones_a_copiar[i]);

		if (resultado_copia == ERR_MEMORIA) {
			tp1_destruir(nuevo_tp1);
			free(pokemones_a_copiar);
			return NULL;
		}
	}

	free(pokemones_a_copiar);

	return nuevo_tp1;
}

struct pokemon *tp1_buscar_nombre(tp1_t *tp, const char *nombre)
{
	if (tp == NULL || nombre == NULL) {
		return NULL;
	}
	if (tp->cantidad_pokemones <= 0) {
		return NULL;
	}

	int inicio = 0;
	int fin = tp->cantidad_pokemones - 1;

	while (inicio <= fin) {
		int centro = (inicio + fin) / 2;

		int cmp = strcasecmp(tp->pokemones[centro]->nombre, nombre);

		if (cmp == 0) {
			return tp->pokemones[centro];
		} else if (cmp < 0) {
			inicio = centro + 1;
		} else {
			fin = centro - 1;
		}
	}

	return NULL;
}

struct pokemon *tp1_buscar_orden(tp1_t *tp, int n)
{
	if (tp == NULL || n < 0 || n >= tp->cantidad_pokemones) {
		return NULL;
	}
	if (tp->cantidad_pokemones <= 0) {
		return NULL;
	}

	return tp->pokemones[n];
}

size_t tp1_con_cada_pokemon(tp1_t *un_tp, bool (*f)(struct pokemon *, void *),
			    void *extra)
{
	size_t i = 0;

	if (un_tp == NULL || f == NULL) {
		return i;
	}

	bool iterando = true;

	while (i < tp1_cantidad(un_tp) && iterando) {
		if ((*f)(un_tp->pokemones[i], extra) == false) {
			iterando = false;
		}
		i++;
	}

	return i;
}

void tp1_destruir(tp1_t *tp1)
{
	if (tp1 == NULL) {
		return;
	}

	for (int i = 0; i < tp1->cantidad_pokemones; i++) {
		free(tp1->pokemones[i]->nombre);
	}
	for (int i = 0; i < tp1->cantidad_pokemones; i++) {
		free(tp1->pokemones[i]);
	}
	free(tp1->pokemones);
	free(tp1);
}