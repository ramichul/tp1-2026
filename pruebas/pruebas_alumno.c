#include "pa2m.h"
#include "../src/tp1.h"
#include "../src/constantes.h"
#include <string.h>

bool potenciar_pokemones(struct pokemon *poke, void *extra)
{
	poke->ataque *= 2;

	return true;
}

bool potenciar_algunos_pokemones(struct pokemon *poke, void *extra)
{
	if (poke->ataque == 49) {
		poke->ataque *= 2;
		return true;
	}

	return false;
}

void tp1_cantidad_da_cantidad_correcta()
{
	tp1_t *tp1 = tp1_leer_archivo(ARCHIVO_PRUEBA_NORMAL);
	pa2m_afirmar(tp1_cantidad(tp1) == CANTIDAD_CORRECTA_NORMAL,
		     "Se cuenta la cantidad correcta de Pokémon presentes");

	tp1_destruir(tp1);
}

void tp1_cantidad_ignora_pokedex_inexistente()
{
	pa2m_afirmar(
		tp1_cantidad(NULL) == 0,
		"No se intenta contar la cantidad de Pokémon en una Pokédex inexistente");
}

void tp1_leer_archivo_ignora_nombre_null()
{
	tp1_t *tp1 = tp1_leer_archivo(NULL);
	pa2m_afirmar(tp1 == NULL, "No se intenta leer con un nombre nulo");
}

void tp1_leer_archivo_ignora_archivo_inexistente()
{
	tp1_t *tp1 = tp1_leer_archivo(ARCHIVO_PRUEBA_INEXISTENTE);
	pa2m_afirmar(tp1 == NULL, "No se intenta leer un archivo inexistente");
}

void tp1_leer_archivo_devuelve_pokedex_valida_cuando_el_archivo_esta_vacio()
{
	tp1_t *tp1 = tp1_leer_archivo(ARCHIVO_PRUEBA_VACIO);
	pa2m_afirmar(tp1 != NULL,
		     "Abrir un archivo vacío devuelve una Pokédex sin errores");

	tp1_destruir(tp1);
}

void tp1_leer_archivo_devuelve_pokedex_vacia_cuando_el_archivo_esta_vacio()
{
	tp1_t *tp1 = tp1_leer_archivo(ARCHIVO_PRUEBA_VACIO);
	pa2m_afirmar(tp1_cantidad(tp1) == 0,
		     "Abrir un archivo vacio devuelve una Pokédex vacía");

	tp1_destruir(tp1);
}

void tp1_leer_archivo_devuelve_pokedex_valida_cuando_el_archivo_no_tiene_pokemon_validos()
{
	tp1_t *tp1 = tp1_leer_archivo(ARCHIVO_PRUEBA_INVALIDO_1);
	pa2m_afirmar(
		tp1 != NULL,
		"Abrir un archivo con un formato válido pero sin campos válidos devuelve una Pokédex sin errores");

	tp1_destruir(tp1);
}

void tp1_leer_archivo_devuelve_pokedex_valida_cuando_el_archivo_no_tiene_formato_valido()
{
	tp1_t *tp1 = tp1_leer_archivo(ARCHIVO_PRUEBA_INVALIDO_2);
	pa2m_afirmar(
		tp1 != NULL,
		"Abrir un archivo con un formato inválido devuelve una Pokédex sin errores");

	tp1_destruir(tp1);
}

void tp1_leer_archivo_devuelve_pokedex_vacia_cuando_el_archivo_no_tiene_formato_valido()
{
	tp1_t *tp1 = tp1_leer_archivo(ARCHIVO_PRUEBA_INVALIDO_2);
	pa2m_afirmar(
		tp1 != NULL && tp1_cantidad(tp1) == 0,
		"Abrir un archivo con un formato inválido devuelve una Pokédex vacía");

	tp1_destruir(tp1);
}

void tp1_leer_archivo_devuelve_pokedex_valida_cuando_el_archivo_tiene_campos_extra()
{
	tp1_t *tp1 = tp1_leer_archivo(ARCHIVO_PRUEBA_INVALIDO_3);

	pa2m_afirmar(
		tp1 != NULL,
		"Abrir un archivo con un formato válido pero más campos de los esperados devuelve una Pokédex sin errores");

	tp1_destruir(tp1);
}

void tp1_leer_archivo_devuelve_pokedex_vacia_cuando_el_archivo_tiene_campos_extra()
{
	tp1_t *tp1 = tp1_leer_archivo(ARCHIVO_PRUEBA_INVALIDO_3);
	pa2m_afirmar(
		tp1 != NULL && tp1_cantidad(tp1) == 0,
		"Abrir un archivo con un formato válido pero más campos de los esperados devuelve una Pokédex vacía");

	tp1_destruir(tp1);
}

void tp1_leer_archivo_lee_archivo_mixto_correctamente()
{
	tp1_t *tp1 = tp1_leer_archivo(ARCHIVO_PRUEBA_MIXTO);
	pa2m_afirmar(
		tp1_cantidad(tp1) == 5,
		"Al abrir un archivo con errores se extraen correctamente los datos de las líneas válidas, ignorando las inválidas");

	tp1_destruir(tp1);
}

void tp1_leer_archivo_devuelve_pokedex_vacia_cuando_el_archivo_no_tiene_pokemon_validos()
{
	tp1_t *tp1_1 = tp1_leer_archivo(ARCHIVO_PRUEBA_INVALIDO_1);
	tp1_t *tp1_2 = tp1_leer_archivo(ARCHIVO_PRUEBA_INVALIDO_2);
	pa2m_afirmar(
		tp1_cantidad(tp1_1) == 0 && tp1_cantidad(tp1_2) == 0,
		"Abrir un archivo sin Pokémon válidos devuelve una Pokédex vacía");

	tp1_destruir(tp1_1);
	tp1_destruir(tp1_2);
}

void tp1_leer_archivo_ignora_pokemon_repetidos()
{
	tp1_t *tp1_1 = tp1_leer_archivo(ARCHIVO_PRUEBA_REPETIDOS_1);
	tp1_t *tp1_2 = tp1_leer_archivo(ARCHIVO_PRUEBA_REPETIDOS_2);
	pa2m_afirmar(tp1_cantidad(tp1_1) == CANTIDAD_CORRECTA_REPETIDOS_1 &&
			     tp1_cantidad(tp1_2) ==
				     CANTIDAD_CORRECTA_REPETIDOS_2,
		     "Se ignoran Pokémon repetidos al rellenar la Pokédex");

	tp1_destruir(tp1_1);
	tp1_destruir(tp1_2);
}

void tp1_leer_archivo_no_distingue_por_mayusculas()
{
	tp1_t *tp1 = tp1_leer_archivo(ARCHIVO_PRUEBA_REPETIDOS_3);
	pa2m_afirmar(
		tp1_cantidad(tp1) == CANTIDAD_CORRECTA_REPETIDOS_3,
		"Al buscar Pokémon repetidos, sus nombres no se distinguen por mayúsculas (Pikachu = pikachu)");

	tp1_destruir(tp1);
}

bool campos_parseados_correctamente(struct pokemon parseado,
				    char *nombre_esperado,
				    enum tipo_pokemon tipo_esperado,
				    int ataque_esperado, int defensa_esperada,
				    int velocidad_esperada)
{
	return strcmp(parseado.nombre, nombre_esperado) == 0 &&
	       parseado.tipo == tipo_esperado &&
	       parseado.ataque == ataque_esperado &&
	       parseado.defensa == defensa_esperada &&
	       parseado.velocidad == velocidad_esperada;
}

void tp1_leer_archivo_parsea_campos_correctamente()
{
	tp1_t *tp1 = tp1_leer_archivo(ARCHIVO_PRUEBA_PARSEO);
	struct pokemon *parseado = tp1_buscar_orden(tp1, 0);

	pa2m_afirmar(
		campos_parseados_correctamente(*parseado, "Pikachu", TIPO_ELEC,
					       55, 40, 90),
		"Al leer un archivo, se parsea cada campo y sus datos se guardan en un struct correctamente");

	tp1_destruir(tp1);
}

void tp1_guardar_archivo_ignora_pokedex_inexistente()
{
	tp1_t *resultado_guardar =
		tp1_guardar_archivo(NULL, NOMBRE_SALIDA_PRUEBA);
	pa2m_afirmar(resultado_guardar == NULL,
		     "No se intenta guardar desde una Pokédex inexistente");
}

void tp1_guardar_archivo_ignora_nombre_nulo()
{
	tp1_t *tp1 = tp1_leer_archivo(ARCHIVO_PRUEBA_PARSEO);
	tp1_t *resultado_guardar = tp1_guardar_archivo(tp1, NULL);
	pa2m_afirmar(
		resultado_guardar == NULL,
		"No se intenta guardar con un nombre de archivo de salida nulo");

	tp1_destruir(tp1);
}

void tp1_guardar_archivo_guarda_archivo_vacio()
{
	tp1_t *tp1 = tp1_leer_archivo(ARCHIVO_PRUEBA_VACIO);
	tp1_guardar_archivo(tp1, NOMBRE_SALIDA_PRUEBA);
	tp1_t *lectura_guardado = tp1_leer_archivo(NOMBRE_SALIDA_PRUEBA);
	pa2m_afirmar(lectura_guardado != NULL &&
			     tp1_cantidad(lectura_guardado) == 0,
		     "Se guarda un archivo vacío dado una Pokédex vacía");

	tp1_destruir(tp1);
	tp1_destruir(lectura_guardado);
}

void tp1_guardar_archivo_guarda_archivo_valido()
{
	tp1_t *tp1 = tp1_leer_archivo(ARCHIVO_PRUEBA_PARSEO);
	pa2m_afirmar(
		tp1_guardar_archivo(tp1, NOMBRE_SALIDA_PRUEBA) != NULL,
		"Se guarda un archivo sin errores a partir de una Pokédex válida");

	tp1_destruir(tp1);
}

void tp1_guardar_archivo_guarda_archivo_valido_con_salida_legible()
{
	tp1_t *tp1 = tp1_leer_archivo(ARCHIVO_PRUEBA_PARSEO);
	tp1_t *resultado_guardado =
		tp1_guardar_archivo(tp1, NOMBRE_SALIDA_PRUEBA);
	tp1_t *lectura_guardado = tp1_leer_archivo(NOMBRE_SALIDA_PRUEBA);
	pa2m_afirmar(
		resultado_guardado == tp1 && lectura_guardado != NULL,
		"Se guarda un archivo sin errores a partir de una Pokédex válida, y su formato es legible por "
		"tp1_leer_archivo()");

	tp1_destruir(tp1);
	tp1_destruir(lectura_guardado);
}

void tp1_guardar_archivo_mantiene_datos_de_pokemon()
{
	tp1_t *tp1 = tp1_leer_archivo(ARCHIVO_PRUEBA_PARSEO);
	tp1_guardar_archivo(tp1, NOMBRE_SALIDA_PRUEBA);
	tp1_t *lectura_guardado = tp1_leer_archivo(NOMBRE_SALIDA_PRUEBA);
	struct pokemon *parseado = tp1_buscar_orden(lectura_guardado, 0);

	pa2m_afirmar(
		campos_parseados_correctamente(*parseado, "Pikachu", TIPO_ELEC,
					       55, 40, 90),
		"No se opera sobre un archivo válido, se guarda con éxito y la "
		"salida mantiene los datos previos");

	tp1_destruir(tp1);
	tp1_destruir(lectura_guardado);
}

void tp1_salida_de_guardar_archivo_tiene_datos_correctos()
{
	tp1_t *tp1 = tp1_leer_archivo(ARCHIVO_PRUEBA_PARSEO);
	tp1_con_cada_pokemon(tp1, potenciar_pokemones, NULL);
	tp1_guardar_archivo(tp1, NOMBRE_SALIDA_PRUEBA);
	tp1_t *lectura_guardado = tp1_leer_archivo(NOMBRE_SALIDA_PRUEBA);
	struct pokemon *parseado = tp1_buscar_orden(lectura_guardado, 0);

	pa2m_afirmar(
		campos_parseados_correctamente(*parseado, "Pikachu", TIPO_ELEC,
					       110, 40, 90),
		"Despues de una operación, se exportan correctamente los datos actualizados del Pokémon al guardar el archivo");

	tp1_destruir(tp1);
	tp1_destruir(lectura_guardado);
}

void tp1_buscar_nombre_no_busca_en_pokedex_inexistente()
{
	struct pokemon *resultado_busqueda = tp1_buscar_nombre(NULL, "Pikachu");

	pa2m_afirmar(
		resultado_busqueda == NULL,
		"No se intenta buscar por nombre en una Pokédex inexistente");
}

void tp1_buscar_nombre_no_intenta_buscar_con_nombre_nulo()
{
	tp1_t *tp1 = tp1_leer_archivo(ARCHIVO_PRUEBA_NORMAL);

	struct pokemon *resultado_busqueda = tp1_buscar_nombre(tp1, NULL);

	pa2m_afirmar(resultado_busqueda == NULL,
		     "No se intenta buscar con un nombre nulo");

	tp1_destruir(tp1);
}

void tp1_buscar_nombre_no_intenta_buscar_con_pokedex_vacia()
{
	tp1_t *tp1 = tp1_leer_archivo(ARCHIVO_PRUEBA_VACIO);

	struct pokemon *resultado_busqueda = tp1_buscar_nombre(tp1, "Pikachu");

	pa2m_afirmar(resultado_busqueda == NULL,
		     "No se intenta buscar por nombre en una Pokédex vacía");

	tp1_destruir(tp1);
}

void tp1_buscar_nombre_devuelve_null_pokemon_no_presente()
{
	tp1_t *tp1 = tp1_leer_archivo(ARCHIVO_PRUEBA_NORMAL);

	struct pokemon *resultado_busqueda =
		tp1_buscar_nombre(tp1, NOMBRE_POKEMON_NO_PRESENTE);

	pa2m_afirmar(resultado_busqueda == NULL,
		     "Buscar un Pokémon no presente por nombre devuelve NULL");

	tp1_destruir(tp1);
}

void tp1_buscar_nombre_devuelve_puntero_correspondiente()
{
	tp1_t *tp1 = tp1_leer_archivo(ARCHIVO_PRUEBA_NORMAL);

	struct pokemon *resultado_busqueda =
		tp1_buscar_nombre(tp1, NOMBRE_POKEMON_PRESENTE);

	pa2m_afirmar(
		strcmp(resultado_busqueda->nombre, NOMBRE_POKEMON_PRESENTE) ==
			0,
		"Se encuentra un Pokémon presente y se devuelve el puntero "
		"correspondiente");

	tp1_destruir(tp1);
}

void tp1_buscar_orden_no_busca_en_pokedex_inexistente()
{
	struct pokemon *resultado_busqueda = tp1_buscar_orden(NULL, 0);

	pa2m_afirmar(
		resultado_busqueda == NULL,
		"No se intenta buscar por orden alfabético en una Pokédex inexistente");
}

void tp1_buscar_orden_no_intenta_buscar_con_pokedex_vacia()
{
	tp1_t *tp1 = tp1_leer_archivo(ARCHIVO_PRUEBA_VACIO);

	struct pokemon *resultado_busqueda = tp1_buscar_orden(tp1, 0);

	pa2m_afirmar(
		resultado_busqueda == NULL,
		"No se intenta buscar por orden alfabético en una Pokédex vacía");

	tp1_destruir(tp1);
}

void tp1_buscar_orden_no_intenta_buscar_fuera_de_limite_inferior()
{
	tp1_t *tp1 = tp1_leer_archivo(ARCHIVO_PRUEBA_PARSEO);

	struct pokemon *resultado_busqueda = tp1_buscar_orden(tp1, -1);

	pa2m_afirmar(
		resultado_busqueda == NULL,
		"No se intenta buscar por orden alfabético fuera del límite inferior de Pokémon");

	tp1_destruir(tp1);
}

void tp1_buscar_orden_no_intenta_buscar_fuera_de_limite_superior()
{
	tp1_t *tp1 = tp1_leer_archivo(ARCHIVO_PRUEBA_PARSEO);

	struct pokemon *resultado_busqueda = tp1_buscar_orden(tp1, 1);

	pa2m_afirmar(
		resultado_busqueda == NULL,
		"No se intenta buscar por orden alfabético fuera del límite superior de Pokémon");

	tp1_destruir(tp1);
}

void tp1_buscar_orden_devuelve_resultado_correctamente()
{
	tp1_t *tp1 = tp1_leer_archivo(ARCHIVO_PRUEBA_NORMAL);

	struct pokemon *resultado_busqueda_1 = tp1_buscar_orden(tp1, 0);
	struct pokemon *resultado_busqueda_2 = tp1_buscar_orden(tp1, 5);

	pa2m_afirmar(
		strcmp(resultado_busqueda_1->nombre,
		       NOMBRE_PRIMER_POKEMON_ALFABETICO) == 0 &&
			strcmp(resultado_busqueda_2->nombre,
			       NOMBRE_ULTIMO_POKEMON_ALFABETICO) == 0,
		"Se encuentra cada Pokémon en el orden alfabético correcto");

	tp1_destruir(tp1);
}

void tp1_con_cada_pokemon_no_intenta_iterar_sobre_pokedex_inexistente()
{
	pa2m_afirmar(tp1_con_cada_pokemon(NULL, potenciar_pokemones, NULL) == 0,
		     "No se intenta iterar sobre una Pokédex inexistente");
}

void tp1_con_cada_pokemon_no_intenta_iterar_con_funcion_nula()
{
	tp1_t *tp1 = tp1_leer_archivo(ARCHIVO_PRUEBA_NORMAL);

	pa2m_afirmar(tp1_con_cada_pokemon(tp1, NULL, NULL) == 0,
		     "No se intenta iterar con una función inexistente");

	tp1_destruir(tp1);
}

void tp1_con_cada_pokemon_aplica_funcion_correctamente()
{
	tp1_t *tp1 = tp1_leer_archivo(ARCHIVO_PRUEBA_PARSEO);
	tp1_con_cada_pokemon(tp1, potenciar_pokemones, NULL);
	struct pokemon *resultado_busqueda = tp1_buscar_orden(tp1, 0);

	pa2m_afirmar(
		resultado_busqueda->ataque == 110,
		"Se modifican los datos correctamente al aplícar una función");

	tp1_destruir(tp1);
}

void tp1_con_cada_pokemon_cuenta_iteracion_sin_interrumpir_correctamente()
{
	tp1_t *tp1 = tp1_leer_archivo(ARCHIVO_PRUEBA_NORMAL);
	size_t i = tp1_con_cada_pokemon(tp1, potenciar_pokemones, NULL);

	pa2m_afirmar(
		i == 6,
		"No se interrumpe la iteración y se cuenta la cantidad de Pokémon afectados por f correctamente");

	tp1_destruir(tp1);
}

void tp1_con_cada_pokemon_cuenta_iteracion_interrumpida_correctamente()
{
	tp1_t *tp1 = tp1_leer_archivo(ARCHIVO_PRUEBA_NORMAL);
	size_t i = tp1_con_cada_pokemon(tp1, potenciar_algunos_pokemones, NULL);

	pa2m_afirmar(
		i == 2,
		"Se interrumpe la iteración y se cuenta la cantidad de Pokémon afectados por f correctamente");

	tp1_destruir(tp1);
}

void tp1_filtrar_tipo_ignora_pokedex_inexistente()
{
	pa2m_afirmar(tp1_filtrar_tipo(NULL, TIPO_ELEC) == NULL,
		     "No se intenta filtrar una Pokédex inexistente");
}

void tp1_filtrar_tipo_ignora_tipo_invalido()
{
	tp1_t *tp1 = tp1_leer_archivo(ARCHIVO_PRUEBA_VACIO);

	tp1_t *resultado_filtrado = tp1_filtrar_tipo(tp1, -1);

	pa2m_afirmar(resultado_filtrado == NULL,
		     "No se intenta filtrar una Pokédex por un tipo inválido");

	tp1_destruir(tp1);
}

void tp1_filtrar_tipo_ignora_pokedex_vacia()
{
	tp1_t *tp1 = tp1_leer_archivo(ARCHIVO_PRUEBA_VACIO);

	tp1_t *resultado_filtrado = tp1_filtrar_tipo(tp1, TIPO_ELEC);

	pa2m_afirmar(resultado_filtrado == NULL,
		     "No se intenta filtrar una Pokédex vacía");

	tp1_destruir(tp1);
}

void tp1_filtrar_tipo_devuelve_pokedex_con_cantidad_correcta()
{
	tp1_t *tp1 = tp1_leer_archivo(ARCHIVO_PRUEBA_NORMAL);

	tp1_t *resultado_filtrado = tp1_filtrar_tipo(tp1, TIPO_ELEC);

	pa2m_afirmar(
		tp1_cantidad(resultado_filtrado) == 1,
		"La Pokédex filtrada tiene la cantidad correcta de Pokémon");

	tp1_destruir(tp1);
	tp1_destruir(resultado_filtrado);
}

void tp1_filtrar_tipo_devuelve_pokedex_con_tipo_correcto()
{
	tp1_t *tp1 = tp1_leer_archivo(ARCHIVO_PRUEBA_NORMAL);

	tp1_t *resultado_filtrado = tp1_filtrar_tipo(tp1, TIPO_ELEC);
	struct pokemon *resultado_busqueda =
		tp1_buscar_orden(resultado_filtrado, 0);

	pa2m_afirmar(
		resultado_busqueda->tipo == TIPO_ELEC,
		"Los Pokémon de la Pokédex filtrada son del tipo correcto");

	tp1_destruir(tp1);
	tp1_destruir(resultado_filtrado);
}

int main()
{
	pa2m_nuevo_grupo("Pruebas de conteo de Pokémon");
	tp1_cantidad_da_cantidad_correcta();
	tp1_cantidad_ignora_pokedex_inexistente();

	pa2m_nuevo_grupo("Pruebas de lectura de archivos");
	tp1_leer_archivo_ignora_nombre_null();
	tp1_leer_archivo_ignora_archivo_inexistente();
	tp1_leer_archivo_devuelve_pokedex_valida_cuando_el_archivo_esta_vacio();
	tp1_leer_archivo_devuelve_pokedex_vacia_cuando_el_archivo_esta_vacio();
	tp1_leer_archivo_devuelve_pokedex_valida_cuando_el_archivo_no_tiene_pokemon_validos();
	tp1_leer_archivo_devuelve_pokedex_vacia_cuando_el_archivo_no_tiene_pokemon_validos();
	tp1_leer_archivo_devuelve_pokedex_valida_cuando_el_archivo_no_tiene_formato_valido();
	tp1_leer_archivo_devuelve_pokedex_vacia_cuando_el_archivo_no_tiene_formato_valido();
	tp1_leer_archivo_devuelve_pokedex_valida_cuando_el_archivo_tiene_campos_extra();
	tp1_leer_archivo_devuelve_pokedex_vacia_cuando_el_archivo_tiene_campos_extra();

	tp1_leer_archivo_ignora_pokemon_repetidos();
	tp1_leer_archivo_no_distingue_por_mayusculas();
	tp1_leer_archivo_lee_archivo_mixto_correctamente();
	tp1_leer_archivo_parsea_campos_correctamente();

	pa2m_nuevo_grupo("Pruebas de guardado de archivos");
	tp1_guardar_archivo_ignora_pokedex_inexistente();
	tp1_guardar_archivo_ignora_nombre_nulo();
	tp1_guardar_archivo_guarda_archivo_vacio();
	tp1_guardar_archivo_guarda_archivo_valido();
	tp1_guardar_archivo_guarda_archivo_valido_con_salida_legible();
	tp1_guardar_archivo_mantiene_datos_de_pokemon();
	tp1_salida_de_guardar_archivo_tiene_datos_correctos();

	pa2m_nuevo_grupo("Pruebas de filtrado de Pokédex");
	tp1_filtrar_tipo_ignora_pokedex_inexistente();
	tp1_filtrar_tipo_ignora_tipo_invalido();
	tp1_filtrar_tipo_ignora_pokedex_vacia();
	tp1_filtrar_tipo_devuelve_pokedex_con_cantidad_correcta();
	tp1_filtrar_tipo_devuelve_pokedex_con_tipo_correcto();

	pa2m_nuevo_grupo("Pruebas de funcionalidades de busqueda");
	tp1_buscar_nombre_no_busca_en_pokedex_inexistente();
	tp1_buscar_nombre_no_intenta_buscar_con_nombre_nulo();
	tp1_buscar_nombre_no_intenta_buscar_con_pokedex_vacia();
	tp1_buscar_nombre_devuelve_null_pokemon_no_presente();
	tp1_buscar_nombre_devuelve_puntero_correspondiente();

	tp1_buscar_orden_no_busca_en_pokedex_inexistente();
	tp1_buscar_orden_no_intenta_buscar_con_pokedex_vacia();
	tp1_buscar_orden_no_intenta_buscar_fuera_de_limite_inferior();
	tp1_buscar_orden_no_intenta_buscar_fuera_de_limite_superior();
	tp1_buscar_orden_devuelve_resultado_correctamente();

	pa2m_nuevo_grupo("Pruebas de iteración interna sobre la Pokédex");
	tp1_con_cada_pokemon_no_intenta_iterar_sobre_pokedex_inexistente();
	tp1_con_cada_pokemon_no_intenta_iterar_con_funcion_nula();
	tp1_con_cada_pokemon_aplica_funcion_correctamente();
	tp1_con_cada_pokemon_cuenta_iteracion_sin_interrumpir_correctamente();
	tp1_con_cada_pokemon_cuenta_iteracion_interrumpida_correctamente();

	remove(NOMBRE_SALIDA_PRUEBA);

	return pa2m_mostrar_reporte();
}
