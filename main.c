#include "src/tp1.h"
#include "src/constantes.h"
#include <stdio.h>
#include <string.h>
#include <strings.h>

const char *COMANDOS_VALIDOS[CANT_COMANDOS_VALIDOS] = { "buscar", "mostrar" };
const char *INSTRUCCIONES_VALIDAS[CANT_INSTRUCCIONES_VALIDAS] = { "nombre",
								  "tipo" };
const char *NOMBRES_COMPLETOS_TIPOS[] = { "⚡Eléctrico",
					  "🔥Fuego",
					  "🌿Planta",
					  "💧Agua",
					  "⬜Normal",
					  "👻Fantasma",
					  "👁️‍🗨️Psíquico",
					  "✊Lucha" };

typedef struct vector {
	void *elementos;
	size_t tope;
} vector_t;

int reconocer_comando_o_instruccion(char *comando, int cantidad_validos,
				    const char *listado_validos[])
{
	int id = -1;
	int i = 0;
	bool es_valido = false;

	while (i < cantidad_validos && !es_valido) {
		if (strcasecmp(listado_validos[i], comando) == 0) {
			es_valido = true;
			id = i;
		}
		i++;
	}

	return id;
}

int controlar_argumentos_iniciales(int argc, char *argv[])
{
	if (argc < CANT_MINIMA_ARGUMENTOS) {
		printf(MSJ_INSTRUCCIONES);
		return ERR_CONTROL;
	}
	if (reconocer_comando_o_instruccion(argv[2], CANT_COMANDOS_VALIDOS,
					    COMANDOS_VALIDOS) == -1) {
		printf(MSJ_ERR_COMANDO, argv[2]);
		printf(MSJ_INSTRUCCIONES);
		return ERR_CONTROL;
	}

	return 0;
}

bool imprimir_pokemon(struct pokemon *pokemon, void *extra)
{
	if (pokemon == NULL || pokemon->nombre == NULL) {
		return false;
	}

	printf(MSJ_DATOS, pokemon->nombre,
	       NOMBRES_COMPLETOS_TIPOS[pokemon->tipo], pokemon->ataque,
	       pokemon->defensa, pokemon->velocidad);

	return true;
}

int controlar_instruccion(char *argv[], int *id_instruccion)
{
	*id_instruccion = reconocer_comando_o_instruccion(
		argv[3], CANT_INSTRUCCIONES_VALIDAS, INSTRUCCIONES_VALIDAS);
	if (*id_instruccion == -1) {
		printf(MSJ_ERR_INSTRUCCION, argv[2], argv[3]);
		return ERR_CONTROL;
	}

	return 0;
}

int ejecutar_comando_buscar(int argc, char *argv[], tp1_t *tp1)
{
	if (argc != CANT_ESPERADA_ARG) {
		printf(MSJ_ERR_ARGUMENTOS, argv[2]);
		return ERR_BUSCAR;
	}

	struct pokemon *busqueda = tp1_buscar_nombre(tp1, argv[3]);
	if (busqueda != NULL) {
		imprimir_pokemon(busqueda, NULL);
	} else {
		printf(MSJ_POKEMON_NO_ENCONTRADO);
		return ERR_BUSCAR;
	}

	return 0;
}

int controlar_argumentos_operacion(int argc, char *argv[])
{
	if (argc != 5) {
		printf(MSJ_ERR_ARGUMENTOS, argv[2]);
		return ERR_CONTROL;
	}

	return 0;
}

bool insertar_ordenadamente_por_tipo(struct pokemon *pokemon,
				     void *vector_contenedor)
{
	vector_t *vec = (vector_t *)vector_contenedor;
	struct pokemon **pokemones = (struct pokemon **)vec->elementos;

	if (vec->tope == 0) {
		pokemones[vec->tope] = pokemon;
		vec->tope++;
		return true;
	} else {
		size_t pos_a_insertar = 0;
		while (pos_a_insertar < vec->tope &&
		       pokemon->tipo > pokemones[pos_a_insertar]->tipo) {
			pos_a_insertar++;
		}

		for (size_t j = vec->tope; j > pos_a_insertar; j--) {
			pokemones[j] = pokemones[j - 1];
		}

		pokemones[pos_a_insertar] = pokemon;
		vec->tope++;
		return true;
	}
}

int mostrar_por_tipo(tp1_t *tp1)
{
	vector_t vector_aux;
	vector_aux.elementos =
		malloc(tp1_cantidad(tp1) * sizeof(struct pokemon *));
	if (vector_aux.elementos == NULL) {
		return ERR_MOSTRAR;
	}
	vector_aux.tope = 0;

	tp1_con_cada_pokemon(tp1, insertar_ordenadamente_por_tipo, &vector_aux);

	struct pokemon **pokemones_a_mostrar =
		(struct pokemon **)(vector_aux.elementos);
	for (size_t i = 0; i < vector_aux.tope; i++) {
		imprimir_pokemon(pokemones_a_mostrar[i], NULL);
	}

	free(vector_aux.elementos);

	return 0;
}

int ejecutar_comando_mostrar(int argc, char *argv[], tp1_t *tp1)
{
	if (argc != CANT_ESPERADA_ARG) {
		printf(MSJ_ERR_ARGUMENTOS, argv[2]);
		return ERR_MOSTRAR;
	}
	if (tp1_cantidad(tp1) == 0) {
		printf(MSJ_VACIA);
		return 0;
	}

	int id_instruccion;
	if (controlar_instruccion(argv, &id_instruccion) == ERR_CONTROL) {
		return ERR_MOSTRAR;
	}
	if (id_instruccion == 0) {
		tp1_con_cada_pokemon(tp1, imprimir_pokemon, NULL);
	}
	if (id_instruccion == 1) {
		mostrar_por_tipo(tp1);
	}

	return 0;
}

int ejecutar_comandos(int argc, char *argv[], tp1_t *tp1)
{
	if (strcasecmp(argv[2], COMANDOS_VALIDOS[0]) == 0) {
		if (ejecutar_comando_buscar(argc, argv, tp1) == ERR_BUSCAR) {
			return ERR_COMANDO;
		}
	}
	if (strcasecmp(argv[2], COMANDOS_VALIDOS[1]) == 0) {
		if (ejecutar_comando_mostrar(argc, argv, tp1) == ERR_MOSTRAR) {
			return ERR_COMANDO;
		}
	}

	return 0;
}

int main(int argc, char *argv[])
{
	printf(MSJ_BIENVENIDA);
	if (controlar_argumentos_iniciales(argc, argv) == ERR_CONTROL) {
		return ERR_CONTROL;
	}

	tp1_t *tp1 = tp1_leer_archivo(argv[1]);
	if (tp1 == NULL) {
		printf(MSJ_ERR_LECTURA);
		return ERR_LECTURA;
	}

	int resultado_ejecucion = ejecutar_comandos(argc, argv, tp1);

	tp1_destruir(tp1);

	if (resultado_ejecucion == 0) {
		printf(MSJ_EXITO);
	}

	return 0;
}