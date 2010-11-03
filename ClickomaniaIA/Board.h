/* 
 * File:   Board.h
 * Author: juen
 *
 * Created on 24 de octubre de 2010, 16:12
 */

#ifndef BOARD_H
#define	BOARD_H
#include <string>
#include <list>
#include <utility>
#include <set>

class Board {
public:

    int* getBoard() const;
    int getColors() const;
    void setColors(int colors);
    int getColumns() const;
    void setColumns(int columns);
    int getRows() const;
    void setRows(int rows);

    Board();
    Board(const std::string path);
    Board(const Board& orig);
    virtual ~Board();
    Board & operator=(const Board& orig);
    bool operator==(const Board& o);

    /*
     * hay que hacer todo a traves de los get y set porque el tablero es un
     * vector, no una matriz, y si no es un coñazo.
     */
    inline int getPosition(int column, int row);
    inline int setPosition(int column, int row, int color);


    /*
     * aplica gravedad vertical y lateral
     */
    void gravity();

    /*
     * elimina (pone a 0) las baldosas que pertenecen a un grupo
     * Tambien reajusta el tablero segun la gravedad
     */
    int removeGroup(std::set<std::pair<int, int> > tiles);

    //calula el movimiento(grupo) que hay en las coordenadas
    std::set<std::pair<int, int> > getGroupMove(int x, int y);

    //calcula todos los movimientos(grupos) posibles dado un tablero
    std::list<std::set<std::pair<int, int> > > getGroupMoves();

    /*
     * Devuelve la coordenada que define el movimiento (podria ser una cualquiera)
     * pero el guion pide un formato determinado que, por construccion, corresponde
     * con la primera baldosa del grupo.
     */
    std::pair<int, int> getMove(const std::set<std::pair<int, int> > groupMove);

    //calcula la puntuacion asociada a un grupo
    //deberia ser static
    static int score(std::set<std::pair<int, int> > tiles);

    /// Realiza una estimación optimista del valor del tablero en el mejor caso.
    /**
     * Se presupone que los grupos de baldosas del mismo color son adyacentes.
     * \return Devuelve un valor entero con la estimación optimista del tablero.
     */
    int funcionCotaEntropia() const;
    int funcionCota() const;

    //broza para mostrar por pantalla que deberia ser static
    static void showMoves(std::list<std::set<std::pair<int, int> > > lista);
    static void showMove(std::set<std::pair<int, int> > move);
    void show();




private:

    int *board;
    int *rows;
    int *columns;
    int *colors;

};

#endif	/* BOARD_H */

