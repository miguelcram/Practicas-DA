// ###### Config options ################

//#define PRINT_DEFENSE_STRATEGY 1    // generate map images

// #######################################

#define BUILDING_DEF_STRATEGY_LIB 1

#include "../simulador/Asedio.h"
#include "../simulador/Defense.h"

#ifdef PRINT_DEFENSE_STRATEGY
#include "ppm.h"
#endif

#ifdef CUSTOM_RAND_GENERATOR
RAND_TYPE SimpleRandomGenerator::a;
#endif

// CAMBIOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOS PRUEBA
using namespace Asedio;

// Devuelve la posición en el mapa del centro de la celda (i,j)
// i - fila
// j - columna
// cellWidth - ancho de las celdas
// cellHeight - alto de las celdas
Vector3 cellCenterToPosition(int i, int j, float cellWidth, float cellHeight){ return Vector3((j * cellWidth) + cellWidth * 0.5f, (i * cellHeight) + cellHeight * 0.5f, 0); }

// Devuelve la celda a la que corresponde una posición en el mapa
// pos - posición que se quiere convertir
// i_out - fila a la que corresponde la posición pos (resultado)
// j_out - columna a la que corresponde la posición pos (resultado)
// cellWidth - ancho de las celdas
// cellHeight - alto de las celdas
void positionToCell(const Vector3 pos, int &i_out, int &j_out, float cellWidth, float cellHeight){ i_out = (int)(pos.y * 1.0f/cellHeight); j_out = (int)(pos.x * 1.0f/cellWidth); }


float cellValue(int row, int col, bool** freeCells, int nCellsWidth, int nCellsHeight
	, float mapWidth, float mapHeight, List<Object*> obstacles, List<Defense*> defenses) {
	return 0; // implemente aqui la funci�n que asigna valores a las celdas
}

// FUNCION DE FACTIBILIDAD
bool factibilidad(Vector3 posiblePosicion, Defense *miDefensa, bool** freeCells, std::list<Object*> obstacles 
              , std::list<Defense*> defenses, float mapWidth, float mapHeight)
{
    bool esFactible = true;
    //TESTEANDO COSAS
    // Que no se salga del mapa
    if((posiblePosicion.x + miDefensa->radio > 60 || posiblePosicion.x - miDefensa->radio < 0) || 
    (posiblePosicion.y + miDefensa->radio > 60 || posiblePosicion.y - miDefensa->radio < 0))
        esFactible = false;
    // Que choque con un obstaculo: que la suma de los radios sea menor que la distancia entre los puntos
    List<obstacles*>::iterator currentObstacle = obstacles.begin();
    while(currentObstacle != obstacles.end() && esFactible){
        if(_distance(posiblePosicion, (*currentObstaculo)->position) < miDefensa->radio + (*currentObstaculo)->radio)
            esFactible = false;
        ++currentObstaculo;
    }
    // Que choque con otra defensa
    List<Defense*>::iterator currentDefense = defenses.begin();
    while(currentDefense != defenses.end() && esFactible){
        if(_distance(posiblePosicion, (*currentObstaculo)->position) < miDefensa->radio + (*currentObstaculo)->radio)
            esFactible = false;
        ++currentDefense;
    }

    return esFactible;
}

void DEF_LIB_EXPORTED placeDefenses(bool** freeCells, int nCellsWidth, int nCellsHeight, float mapWidth, float mapHeight
              , std::list<Object*> obstacles, std::list<Defense*> defenses) {

    float cellWidth = mapWidth / nCellsWidth;
    float cellHeight = mapHeight / nCellsHeight;

    int maxAttemps = 1000;
    List<Defense*>::iterator currentDefense = defenses.begin();
    while(currentDefense != defenses.end() && maxAttemps > 0) {

        (*currentDefense)->position.x = ((int)(_RAND2(nCellsWidth))) * cellWidth + cellWidth * 0.5f;
        (*currentDefense)->position.y = ((int)(_RAND2(nCellsHeight))) * cellHeight + cellHeight * 0.5f;
        (*currentDefense)->position.z = 0; 
        ++currentDefense;
    }

#ifdef PRINT_DEFENSE_STRATEGY

    float** cellValues = new float* [nCellsHeight]; 
    for(int i = 0; i < nCellsHeight; ++i) {
       cellValues[i] = new float[nCellsWidth];
       for(int j = 0; j < nCellsWidth; ++j) {
           cellValues[i][j] = ((int)(cellValue(i, j))) % 256;
       }
    }
    dPrintMap("strategy.ppm", nCellsHeight, nCellsWidth, cellHeight, cellWidth, freeCells
                         , cellValues, std::list<Defense*>(), true);

    for(int i = 0; i < nCellsHeight ; ++i)
        delete [] cellValues[i];
	delete [] cellValues;
	cellValues = NULL;

#endif
}
