#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>

using namespace std;

const int SIZE = 9;
vector<vector<int>> board(SIZE, vector<int>(SIZE, 0));
int score = 0; // Puntaje inicial

struct Piece
{
    vector<vector<int>> shape;
    char symbol;
};

vector<Piece> generatePieces()
{
    vector<Piece> pieces;

    vector<vector<vector<int>>> shapes = {
        {{1, 1, 1}},              // Línea horizontal
        {{1}, {1}, {1}},          // Línea vertical
        {{1, 1}, {1, 1}},         // Cuadrado
        {{1, 0}, {1, 0}, {1, 1}}, // Z invertida
        {{0, 1}, {1, 1}, {1, 0}}  // Z
    };

    char symbols[] = {'#', '*', '+', '@', '%'};

    for (int i = 0; i < 3; i++)
    {
        int index = rand() % shapes.size();
        pieces.push_back({shapes[index], symbols[index]});
    }

    return pieces;
}

void printBoard()
{
    cout << "Score: " << score << endl; // Mostrar el puntaje
    cout << "   ";
    for (int i = 0; i < SIZE; i++)
        cout << i << " ";
    cout << endl;
    for (int i = 0; i < SIZE; i++)
    {
        cout << i << "  ";
        for (int j = 0; j < SIZE; j++)
        {
            cout << (board[i][j] ? '#' : '.') << " ";
        }
        cout << endl;
    }
}

bool canPlace(const Piece &piece, int x, int y)
{
    int h = piece.shape.size();
    int w = piece.shape[0].size();
    if (x + h > SIZE || y + w > SIZE)
        return false;

    for (int i = 0; i < h; i++)
        for (int j = 0; j < w; j++)
            if (piece.shape[i][j] && board[x + i][y + j])
                return false;

    return true;
}

void placePiece(const Piece &piece, int x, int y)
{
    int h = piece.shape.size();
    int w = piece.shape[0].size();
    for (int i = 0; i < h; i++)
        for (int j = 0; j < w; j++)
            if (piece.shape[i][j])
                board[x + i][y + j] = 1;
}

void clearLines()
{
    // Horizontal
    for (int i = 0; i < SIZE; i++)
    {
        bool full = true;
        for (int j = 0; j < SIZE; j++)
            if (!board[i][j])
                full = false;
        if (full)
        {
            score += 50; // Incrementar 50 puntos por línea completa
            for (int j = 0; j < SIZE; j++)
                board[i][j] = 0;
        }
    }

    // Vertical
    for (int j = 0; j < SIZE; j++)
    {
        bool full = true;
        for (int i = 0; i < SIZE; i++)
            if (!board[i][j])
                full = false;
        if (full)
        {
            score += 50; // Incrementar 50 puntos por línea completa
            for (int i = 0; i < SIZE; i++)
                board[i][j] = 0;
        }
    }

    // Diagonal principal (↘)
    for (int k = 0; k <= 2 * (SIZE - 1); ++k)
    {
        bool full = true;
        vector<pair<int, int>> coords;
        for (int i = 0; i < SIZE; ++i)
        {
            int j = k - i;
            if (j >= 0 && j < SIZE)
            {
                coords.push_back({i, j});
                if (!board[i][j])
                    full = false;
            }
        }
        if (coords.size() >= SIZE && full)
        {
            score += 50; // Incrementar 50 puntos por línea completa
            for (auto [i, j] : coords)
                board[i][j] = 0;
        }
    }

    // Diagonal secundaria (↙)
    for (int k = -SIZE + 1; k < SIZE; ++k)
    {
        bool full = true;
        vector<pair<int, int>> coords;
        for (int i = 0; i < SIZE; ++i)
        {
            int j = i - k;
            if (j >= 0 && j < SIZE)
            {
                coords.push_back({i, j});
                if (!board[i][j])
                    full = false;
            }
        }
        if (coords.size() >= SIZE && full)
        {
            score += 50; // Incrementar 50 puntos por línea completa
            for (auto [i, j] : coords)
                board[i][j] = 0;
        }
    }
}

bool anyPlacementPossible(const Piece &piece)
{
    for (int i = 0; i < SIZE; i++)
        for (int j = 0; j < SIZE; j++)
            if (canPlace(piece, i, j))
                return true;
    return false;
}

int main()
{
    srand(time(0));

    while (true)
    {
        printBoard();
        vector<Piece> pieces = generatePieces();
        bool allPlaced = true;

        for (int i = 0; i < 3; i++)
        {
            Piece &p = pieces[i];
            cout << "\nPieza #" << i + 1 << ":\n";
            for (auto &row : p.shape)
            {
                for (auto val : row)
                    cout << (val ? p.symbol : '.') << " ";
                cout << endl;
            }

            if (!anyPlacementPossible(p))
            {
                allPlaced = false;
                break;
            }

            int x, y;
            while (true)
            {
                cout << "Ingresa posición X Y para colocar pieza #" << i + 1 << ": ";
                cin >> x >> y;
                if (canPlace(p, x, y))
                {
                    placePiece(p, x, y);
                    clearLines();
                    printBoard(); // Muestra el tablero después de cada colocación
                    break;
                }
                else
                {
                    cout << "No se puede colocar ahí. Intenta de nuevo.\n";
                }
            }
        }

        if (!allPlaced)
        {
            cout << "¡No puedes colocar todas las piezas! Fin del juego.\n";
            break;
        }
    }

    return 0;
}
