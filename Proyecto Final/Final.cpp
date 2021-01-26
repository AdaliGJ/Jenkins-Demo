#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>
#include <math.h>
#include <fstream>

using namespace std;

//Proyecto final
//Jeremy C�ceres y Adal� Garr�n

//Funci�n auxiliar 1; convertir un string en un int
int parseInt(const string& real) {

	//atoi: funci�n predefinida para convertir de string a int
	int resultado = atoi(real.c_str());//Devuelve 0 si no es v�lido

	return resultado;
}


//Funci�n auxiliar 2; separar un string cuando aparezca una ',' 
vector<string> separar(string str, char separador = ',') {

	int inicial = 0;
	int contador = 0;
	string separada;
	vector<string> resultados;

	while (contador >= 0) {
		contador = str.find(separador, inicial);
		separada = str.substr(inicial, contador - inicial);
		inicial = contador + 1;

		//Solo se incluye la substring en el vector si al pasar de string a int, es v�lido
		if (parseInt(separada) != 0) {
			resultados.push_back(separada);
		}
	}

	return resultados;
}


//Se define la clase Compra
class Compra {

public:

	//El constructor toma como par�metro como string con los productos y separa cada art�culo 
	Compra(string productos) {
		vector<string> separados = separar(productos);
		for (int i = 0; i < separados.size(); i++) {

			//El vector articulos contiene todos los n�meros de art�culos que se compraron
			//Utilizaci�n de funci�n auxiliar 1 y 2
			articulos.push_back(parseInt(separados[i]));
		}
	}

	~Compra() {
		articulos.clear();//Destructor
	}

	//Getters
	int getSize() {
		return articulos.size();
	}

	vector<int> getArreglo() {
		return articulos;
	}

private:
	vector<int> articulos;
};


//Funci�n auxiliar 3: funci�n auxiliar para encontrar el powerSet de un vector
void subCAux(vector<int> v, vector<vector<int>>& respuesta, vector<int>& subC, int indice) {
	respuesta.push_back(subC);
	for (int i = indice; i < v.size(); i++) {
		subC.push_back(v[i]);
		subCAux(v, respuesta, subC, i + 1);
		subC.pop_back();
	}
}


//Funci�n auxiliar 4: agrupa los productos en grupos de diferente tama�o 
vector<vector<int>> agrupar(Compra productos)
{
	vector<int> cliente;
	vector<vector<int>> compra;

	int i = 0;

	//Se llama a la funci�n auxiliar 3 con los par�metros definidos
	subCAux(productos.getArreglo(), compra, cliente, i);


	//Retorna un vector con todos los subconjuntos de la Compra
	return compra;
}

//Funci�n auxiliar 5: retorna un vector con los vectores agrupados de cada compra 
vector<vector<vector<int>>> pAgrupados(vector<Compra> compra) {
	vector<vector<vector<int>>> prodsAgrupados;

	for (int i = 0; i < compra.size(); i++) {
		prodsAgrupados.push_back(agrupar(compra[i]));
	}
	return prodsAgrupados;
}



//Funci�n auxiliar 6: utilizaci�n del m�todo burbuja para ordenar los elementos de un vector
vector<int> ordenarV(vector<int> vec) {
	int aux;

	for (int i = 0; i < vec.size() - 1; i++) {
		for (int j = i + 1; j < vec.size(); j++) {
			if (vec[i] > vec[j]) {
				aux = vec[i];
				vec[i] = vec[j];
				vec[j] = aux;
			}
		}
	}

	//Se retorna el vector ordenado
	return vec;
}



/*Definici�n del struct contador con un vector de productos
agrupados y un int con las veces que se repite*/
struct Contador {
	vector<int> agrupados;
	int repeticiones;
};


//Definici�n de la clase Conteo
class Conteo {
public:

	/*El constructor acepta un vector<vector<vector<int>>>
	y el n�mero m�ximo de productos que se quieren agrupar entre s�*/
	Conteo(vector<vector<vector<int>>> largo, int max) {
		vector<vector<int>> nuevo;//Definir un vector de vectores de int nuevo

		for (int i = 0; i < largo.size(); i++) {
			for (int j = 0; j < largo[i].size(); j++) {
				if (largo[i][j].size() > 1 && largo[i][j].size() <= max) {
					vector<int> ordenado = ordenarV(largo[i][j]);//utilizaci�n de funci�n auxiliar 6

					/*Se agregan al nuevo vector los subconjuntos de todas las compras
					sin contar los conjuntos vac�os y los vectores de un elemento, hasta
					llegar a los grupos con el m�ximo n�mero de productos deseados de agrupar*/
					nuevo.push_back(ordenado);
				}
			}
		}

		//Se cuenta la cantidad de veces que se repiten los grupos de productos
		for (int i = 0; i < nuevo.size(); i++) {
			int contador = 0;
			Contador c;
			c.agrupados = nuevo[i];
			for (int j = 0; j < nuevo.size(); j++) {
				if (nuevo[i] == nuevo[j]) {//Si son iguales, se aumenta la cuenta
					contador++;
				}
			}
			c.repeticiones = contador;

			//Solo se agregan los grupos al vector de conteo si compran en conjunto m�s de 1 vez 
			if (contador > 1) {
				contados.push_back(c);
			}
		}

	}

	~Conteo() {//Destructor
		for (int i = 0; i < contados.size(); i++) {
			contados[i].agrupados.clear();
			contados[i].repeticiones = 0;
		}
		contados.clear();
	}

	//M�todo para eliminar los grupos repetidos en el vector
	void eliminarRep() {
		for (int i = 0; i < contados.size() - 1; i++) {
			for (int j = 0; j < contados.size(); j++) {
				if (contados[i].agrupados == contados[j].agrupados && i != j) {
					contados.erase(contados.begin() + j);//erase: elimina un elemento del vector seg�n su �ndice
					j = 0;
				}
			}
		}
	}

	/*M�todo para ordenar el vector con los productos agrupados,
	primero por tama�o de grupo, luego por repeticiones*/
	void ordenar() {
		Contador aux;

		//Ordenar por tama�o de grupo: m�todo burbuja
		for (int i = 0; i < contados.size() - 1; i++) {
			for (int j = i + 1; j < contados.size(); j++) {
				if (contados[i].agrupados.size() < contados[j].agrupados.size()) {
					aux = contados[i];
					contados[i] = contados[j];
					contados[j] = aux;
				}
			}
		}

		/*Ordenar por cantidad de veces comprado manteniendo
		el orden por tama�o de grupo: m�todo burbuja*/
		for (int i = 0; i < contados.size() - 1; i++) {
			for (int j = i + 1; j < contados.size(); j++) {
				if (contados[i].repeticiones < contados[j].repeticiones
					&& contados[i].agrupados.size() <= contados[j].agrupados.size()) {
					aux = contados[i];
					contados[i] = contados[j];
					contados[j] = aux;
				}
			}
		}
	}

	//M�todo para imprimir los grupos y las veces que se compraron en conjunto
	void imprimirV() {
		cout << "Articulos frecuentemente comprados en conjunto: " << endl;

		for (int i = 0; i < contados.size(); i++) {
			cout << "-Articulos ";
			for (int j = 0; j < contados[i].agrupados.size() - 1; j++) {
				cout << contados[i].agrupados[j] << ", ";
			}
			cout << contados[i].agrupados[contados[i].agrupados.size() - 1] << ". ";

			printf("Veces comprados en conjunto: %i\n", contados[i].repeticiones);
		}
	}

	//M�todo para guardar los datos en un archivo.txt
	void guardarDatos(string fecha) {
		ofstream datos;

		datos.open("Datos.txt", ios::out);//Crear y abrir archivo

		if (datos.fail()) {
			cout << "No es posible abrir el archivo";
			exit(1);//Si falla, se sale del archivo
		}

		datos<<"Art�culos comprados en conjunto"<<endl;
		for (int i = 0; i < contados.size(); i++) {
			datos << "-Articulos ";
			for (int j = 0; j < contados[i].agrupados.size() - 1; j++) {
				datos << contados[i].agrupados[j] << ", ";
			}
			datos << contados[i].agrupados[contados[i].agrupados.size() - 1] << ". ";

			datos<<"Veces comprados en conjunto:  " << contados[i].repeticiones<<endl;
		}
		datos <<endl<< "Datos actualizados el: " << fecha << endl;
		datos << "-------------------------------------------------------------"<<endl;

		datos.close();//Cerrar el archivo

	}

	//Getters
	int getSize() {
		return contados.size();
	}
	vector<Contador> getProds() {
		return contados;
	}

private:
	vector<Contador> contados;
};



int main()
{
	vector<Compra> artClientes;//Vector con todas las compras que se ingresar�n
	string productos;//parametro de la Compra

	cout << "Prgrama de supermercado: Productos comprados en conjunto \n";
	cout << "Jeremy Caceres y Adali Garran \n\n\n";
	cout << "Ingrese los productos comprados por cada cliente (1 cliente por linea):\n";

	while (productos != "\n") {//Ciclo continuo que no acabar� hasta que no se ingresen productos
		getline(cin, productos);//Ingresar cada producto

		if (!productos.empty()) {
			artClientes.push_back(Compra(productos));//Cada vez que se ingresa una l�nea de productos, se agrega al arreglo de Compras
		}
		else {
			break;//Al dar Enter (vector vac�o) se rompe el ciclo
		}
	}

	vector<vector<vector<int>>> prodsAgrupados = pAgrupados(artClientes);


	int max;
	cout << "Seleccione el numero maximo de articulos que desea agrupar: ";
	cin >> max;
	cout << endl;

	//Definir una lista de clase conteo, con los productos agrupados
	Conteo lista(prodsAgrupados, max);
	lista.ordenar();
	lista.eliminarRep();
	lista.imprimirV();

	string fecha;
	cout << endl;
	cout << "Ingrese la fecha para guardar los datos: ";
	cin >> fecha;
	lista.guardarDatos(fecha);

	char x;
	cin >> x;

	return 0;
}