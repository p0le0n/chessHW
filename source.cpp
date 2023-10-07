#include <iostream>
#include <ctime>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <cctype>

const int BOARD_SIZE = 8;
const int count_of_moves = 3;

const char KING = 'K';
const char ROOK = 'R';
const char BISHOP = 'B';
const char KNIGHT = 'N';
const char PAWN = 'P';
const char EMPTY = '.';

class Piece {
public:
    char symbol;
    bool isWhite;

    // Конструктор по умолчанию
    Piece() : symbol(EMPTY), isWhite(true) {}

    // Конструктор с параметрами
    Piece(char sym, bool white) : symbol(sym), isWhite(white) {}

    // Конструктор копирования
    Piece(const Piece& other) : symbol(other.symbol), isWhite(other.isWhite) {}

    // Оператор присваивания
    Piece& operator=(const Piece& other) {
        if (this == &other) {
            return *this;
        }
        symbol = other.symbol;
        isWhite = other.isWhite;
        return *this;
    }
};

void generateRandomPosition(std::vector<std::vector<Piece>>& board) {
    for (int row = 0; row < BOARD_SIZE; ++row) {
        for (int col = 0; col < BOARD_SIZE; ++col) {
            board[row][col] = Piece(EMPTY, true);
        }
    }

    // Определение максимального количества фигур каждого вида
    const int maxPieces[] = { 0, 4, 4, 4, 16 }; // Король (генерируется отдельно), ладья, слон, конь, пешенки

    // Счетчики для фигур каждого вида
    int pieceCount[5] = { 0 }; // Король, ладья, слон, конь, пешенки

    // Проходимся по доске и случайно размещаем оставшиеся фигуры
    for (int row = 0; row < BOARD_SIZE; ++row) {
        for (int col = 0; col < BOARD_SIZE; ++col) {
            if (board[row][col].symbol == EMPTY) {
                // Генерируем случайное число для выбора фигуры
                int pieceChoice = std::rand() % 5; // 5 видов фигур

                // Проверяем, можно ли добавить еще фигуры этого вида
                if (pieceCount[pieceChoice] < maxPieces[pieceChoice]) {
                    char pieceSymbol = '.';
                    bool isWhite = true;

                    // Определяем символ фигуры и цвет
                    switch (pieceChoice) {
                    case 0:
                        pieceSymbol = KING;
                        break;
                    case 1:
                        pieceSymbol = ROOK;
                        break;
                    case 2:
                        pieceSymbol = BISHOP;
                        break;
                    case 3:
                        pieceSymbol = KNIGHT;
                        break;
                    case 4:
                        if (row == 0) {
                            pieceSymbol = PAWN;
                            pieceSymbol = std::tolower(pieceSymbol);
                            isWhite = false;
                        }
                        if (row == BOARD_SIZE) {
                            pieceSymbol = PAWN;
                            pieceSymbol = std::toupper(pieceSymbol);
                            isWhite = true;
                        }
                        else {
                            if (std::rand() % 2 == 0) {
                                pieceSymbol = std::tolower(PAWN); // Черные пешки
                                isWhite = false;
                            }
                            else {
                                pieceSymbol = std::toupper(PAWN); // Белые пешки
                                isWhite = true;
                            }
                        }
                        break;
                    default:
                        break;
                    }

                    // Размещаем фигуру случайным образом у белых или черных (пешки - отдельно)
                    if (std::rand() % 2 == 0 && pieceSymbol != std::toupper(PAWN) && pieceSymbol != std::tolower(PAWN)) {
                        pieceSymbol = std::tolower(pieceSymbol); // Черные фигуры
                        isWhite = false;
                    }

                    // Размещаем фигуру на доске и обновляем счетчик
                    board[row][col] = Piece(pieceSymbol, isWhite);
                    pieceCount[pieceChoice]++;
                }
            }
        }
    }

    // Размещаем по одному королю для каждой стороны
    int blackKingRow = std::rand() % 2; // Случайно выбираем 1 или 2 для строки чёрного короля
    int whiteKingRow = BOARD_SIZE - (std::rand() % 2) - 1; // Случайно выбираем последнюю или предпоследнюю для строки белого короля
    int blackKingCol = std::rand() % BOARD_SIZE; // Случайно выбираем столбец для чёрного короля
    int whiteKingCol = std::rand() % BOARD_SIZE; // Случайно выбираем столбец для белого короля

    board[whiteKingRow][whiteKingCol] = Piece(KING, true);
    board[blackKingRow][blackKingCol] = Piece('k', false);
}

void printBoard(const std::vector<std::vector<Piece>>& board) {
    std::cout << "\\ " << "A " << "B " << "C " << "D " << "E " << "F " << "G " << "H " << std::endl;
    for (int row = 0; row < BOARD_SIZE; ++row) {
        std::cout << row + 1 << ' ';
        for (int col = 0; col < BOARD_SIZE; ++col) {
            char symbol = board[row][col].symbol;
            if (board[row][col].isWhite) {
                std::cout << static_cast<char>(std::toupper(symbol)) << ' ';
            }
            else {
                std::cout << static_cast<char>(std::tolower(symbol)) << ' ';
            }
        }
        std::cout << std::endl;
    }
}

bool isValidCell(int row, int col) {
    return row >= 0 && row < BOARD_SIZE && col >= 0 && col < BOARD_SIZE;
}

bool isValidPawnMove(const std::vector<std::vector<Piece>>& board, int fromRow, int fromCol, int toRow, int toCol, bool isWhite) {
    // Пешка может двигаться вперед на одну клетку
    if (isWhite && toRow == fromRow - 1 && toCol == fromCol && board[toRow][toCol].symbol == EMPTY) {
        return true;
    }
    else if (!isWhite && toRow == fromRow + 1 && toCol == fromCol && board[toRow][toCol].symbol == EMPTY) {
        return true;
    }

    // Пешка может двигаться вперед на две клетки при первом ходе
    if (isWhite && fromRow == 6 && toRow == 4 && toCol == fromCol && board[5][fromCol].symbol == EMPTY && board[4][fromCol].symbol == EMPTY) {
        return true;
    }
    else if (!isWhite && fromRow == 1 && toRow == 3 && toCol == fromCol && board[2][fromCol].symbol == EMPTY && board[3][fromCol].symbol == EMPTY) {
        return true;
    }

    // Пешка может съесть фигуру на диагонали
    if (isWhite && toRow == fromRow - 1 && (toCol == fromCol + 1 || toCol == fromCol - 1) && islower(board[toRow][toCol].symbol)) {
        return true;
    }
    else if (!isWhite && toRow == fromRow + 1 && (toCol == fromCol + 1 || toCol == fromCol - 1) && isupper(board[toRow][toCol].symbol)) {
        return true;
    }

    return false;
}

bool isValidKingMove(const std::vector<std::vector<Piece>>& board, int fromRow, int fromCol, int toRow, int toCol, bool isWhite) {
    int dr = std::abs(toRow - fromRow);
    int dc = std::abs(toCol - fromCol);
    return dr <= 1 && dc <= 1;
}

bool isValidBishopMove(const std::vector<std::vector<Piece>>& board, int fromRow, int fromCol, int toRow, int toCol, bool isWhite) {
    // Проверка, что начальные и конечные индексы находятся в пределах доски
    if (!isValidCell(fromRow, fromCol) || !isValidCell(toRow, toCol)) {
        return false;
    }

    char piece = std::toupper(board[fromRow][fromCol].symbol);

    int dr = toRow - fromRow;
    int dc = toCol - fromCol;

    // Проверка, что ход по диагонали
    if (std::abs(dr) != std::abs(dc)) {
        return false;
    }

    int rowStep = (dr > 0) ? 1 : -1;
    int colStep = (dc > 0) ? 1 : -1;

    int row = fromRow + rowStep;
    int col = fromCol + colStep;

    // Проверка, что нет преград на пути
    while (row != toRow || col != toCol) {
        // Проверяем, не выходит ли row за пределы доски
        if (row < 0 || row >= BOARD_SIZE) {
            return false;
        }

        // Проверяем, не выходит ли col за пределы доски
        if (col < 0 || col >= BOARD_SIZE) {
            return false;
        }

        if (board[row][col].symbol != EMPTY) {
            return false;
        }
        row += rowStep;
        col += colStep;
    }


    return true;
}

bool isValidRookMove(const std::vector<std::vector<Piece>>& board, int fromRow, int fromCol, int toRow, int toCol, bool isWhite) {
    // Проверка, что начальные и конечные индексы находятся в пределах доски
    if (!isValidCell(fromRow, fromCol) || !isValidCell(toRow, toCol)) {
        return false;
    }

    char piece = std::toupper(board[fromRow][fromCol].symbol);

    int dr = toRow - fromRow;
    int dc = toCol - fromCol;

    // Проверка, что ход вертикальный или горизонтальный
    if (dr != 0 && dc != 0) {
        return false;
    }

    int rowStep = (dr > 0) ? 1 : (dr < 0) ? -1 : 0;
    int colStep = (dc > 0) ? 1 : (dc < 0) ? -1 : 0;

    int row = fromRow + rowStep;
    int col = fromCol + colStep;

    // Проверка, что нет преград на пути
    while (row != toRow || col != toCol) {
        if (board[row][col].symbol != EMPTY) {
            return false;  // Есть преграда на пути
        }
        row += rowStep;
        col += colStep;
    }

    return true;
}

bool isValidKnightMove(const std::vector<std::vector<Piece>>& board, int fromRow, int fromCol, int toRow, int toCol, bool isWhite) {
    int dr = std::abs(toRow - fromRow);
    int dc = std::abs(toCol - fromCol);
    return (dr == 1 && dc == 2) || (dr == 2 && dc == 1);
}

bool isValidMove(const std::vector<std::vector<Piece>>& board, int fromRow, int fromCol, int toRow, int toCol, bool isWhite) {
    if (!isValidCell(fromRow, fromCol) || !isValidCell(toRow, toCol)) {
        return false;
    }

    if (board[toRow][toCol].isWhite == isWhite) {
        return false;  // Фигура того же цвета
    }
    
    if (board[toRow][toCol].symbol == std::toupper(KING) || board[toRow][toCol].symbol == std::tolower(KING)) {
        return false;  // Нельзя есть короля
    }

    const Piece& movingPiece = board[fromRow][fromCol];
    const Piece& targetPiece = board[toRow][toCol];

    if (movingPiece.symbol == EMPTY || movingPiece.isWhite != true) {
        return false;
    }

    char piece = std::toupper(movingPiece.symbol);

    if (!targetPiece.isWhite) {
        char targetPieceSymbol = std::tolower(targetPiece.symbol);

        if (piece == PAWN) {
            return isValidPawnMove(board, fromRow, fromCol, toRow, toCol, true);
        }
        else if (piece == KING) {
            return isValidKingMove(board, fromRow, fromCol, toRow, toCol, true);
        }
        else if (piece == BISHOP) {
            return isValidBishopMove(board, fromRow, fromCol, toRow, toCol, true);
        }
        else if (piece == ROOK) {
            return isValidRookMove(board, fromRow, fromCol, toRow, toCol, true);
        }
        else if (piece == KNIGHT) {
            return isValidKnightMove(board, fromRow, fromCol, toRow, toCol, true);
        }
    }
    else if (targetPiece.symbol == EMPTY || targetPiece.isWhite == true) {
        if (piece == PAWN) {
            return isValidPawnMove(board, fromRow, fromCol, toRow, toCol, true);
        }
        else if (piece == KING) {
            return isValidKingMove(board, fromRow, fromCol, toRow, toCol, true);
        }
        else if (piece == BISHOP) {
            return isValidBishopMove(board, fromRow, fromCol, toRow, toCol, true);
        }
        else if (piece == ROOK) {
            return isValidRookMove(board, fromRow, fromCol, toRow, toCol, true);
        }
        else if (piece == KNIGHT) {
            return isValidKnightMove(board, fromRow, fromCol, toRow, toCol, true);
        }
    }

    return false;
}

bool isCheck(const std::vector<std::vector<Piece>>& board, int fromRow, int fromCol, bool isWhite) {
    // Поиск короля текущего цвета
    char kingSymbol = isWhite ? 'K' : 'k';
    int kingRow = -1;
    int kingCol = -1;

    for (int row = 0; row < BOARD_SIZE; ++row) {
        for (int col = 0; col < BOARD_SIZE; ++col) {
            if (board[row][col].symbol == kingSymbol) {
                kingRow = row;
                kingCol = col;
                break;
            }
        }
        if (kingRow != -1) {
            break;
        }
    }

    // Проверка, угрожает ли королю опасность
    if (board[fromRow][fromCol].isWhite != isWhite && board[fromRow][fromCol].symbol != EMPTY) {
        if (isValidMove(board, fromRow, fromCol, kingRow, kingCol, !isWhite)) {
            return true;  // Король под шахом
        }
     }

    return false;  // Король не находится под шахом
}

bool isCheckMate(std::vector<std::vector<Piece>>& board, int fromRow, int fromCol, bool isWhite) {
    // Проверка на мат
    if (!isCheck(board, fromRow, fromCol, isWhite)) {
        return false;  // Король не находится под шахом, нет матовой ситуации
    }

    // Попробуем выполнить все возможные ходы и проверить, остается ли король под шахом
    if (board[fromRow][fromCol].isWhite == isWhite) {
        for (int toRow = 0; toRow < BOARD_SIZE; ++toRow) {
            for (int toCol = 0; toCol < BOARD_SIZE; ++toCol) {
                if (isValidMove(board, fromRow, fromCol, toRow, toCol, !isWhite)) {
                    // Выполняем временный ход
                    Piece originalPiece = board[toRow][toCol];
                    board[toRow][toCol] = board[fromRow][fromCol];
                    board[fromRow][fromCol] = Piece(EMPTY, true);

                    // Проверяем, остается ли король под шахом после хода
                    if (!isCheck(board, fromRow, fromCol, isWhite)) {
                        // Отменяем временный ход и возвращаем false
                        board[fromRow][fromCol] = board[toRow][toCol];
                        board[toRow][toCol] = originalPiece;
                        return false;  // Существует хотя бы один ход, который не оставляет короля под шахом
                    }

                    // Отменяем временный ход
                    board[fromRow][fromCol] = board[toRow][toCol];
                    board[toRow][toCol] = originalPiece;
                }
            }
        }
    }

    return true;  // Нет возможных ходов, которые могли бы спасти короля от шаха (мат)
}

bool canCapturePiece(const std::vector<std::vector<Piece>>& board, int row, int col, bool isWhite) {
    // Проверяем, может ли фигура съесть фигуру на заданной клетке

    if (!isValidCell(row, col) || board[row][col].symbol == EMPTY) {
        return false;  // Неверная клетка или пустая клетка
    }

    if (board[row][col].isWhite == isWhite) {
        return false;  // Фигура того же цвета
    }

    // В данной реализации предполагается, что все фигуры могут съесть фигуру на клетке (кроме короля)
    char attackingPiece = std::toupper(board[row][col].symbol);
    char targetPiece = std::toupper(board[row][col].symbol);

    if (attackingPiece == KING) {
        return false;  // Король не может съесть фигуру
    }

    return attackingPiece != targetPiece;  // Сравниваем типы фигур, чтобы король не мог съесть фигуру того же типа
}

bool canSavePiece(const std::vector<std::vector<Piece>>& board, int row, int col, bool isWhite) {
    // Проверяем, может ли фигура спасти другую фигуру от угрозы съесть

    if (!isValidCell(row, col) || board[row][col].symbol == EMPTY || board[row][col].isWhite != isWhite) {
        return false;  // Неверная клетка, пустая клетка или фигура другого цвета
    }

    if (board[row][col].isWhite == isWhite) {
        return false;  // Фигура того же цвета
    }

    // В данной реализации предполагается, что все фигуры могут спасти другую фигуру (кроме короля)
    char piece = std::toupper(board[row][col].symbol);
    return piece != KING;  // Король не может спасти фигуру
}

int getPieceValue(char pieceSymbol) {
    switch (pieceSymbol) {
    case PAWN:
        return 1;
    case KNIGHT:
        return 3;
    case BISHOP:
        return 3;
    case ROOK:
        return 5;
    case KING:
        return 1000;
    default:
        return 0;  // Неизвестная фигура или пустая клетка
    }
}

bool isValidCoordinate(int row, int col) {
    return row >= 0 && row < BOARD_SIZE && col >= 0 && col < BOARD_SIZE;
}

int evaluateMove(std::vector<std::vector<Piece>> board, int fromRow, int fromCol, int toRow, int toCol, bool isWhite) {
    if (!isValidCoordinate(fromRow, fromCol) || !isValidCoordinate(toRow, toCol)) {
        // Проверка на допустимые координаты
        return -1000; // Или другое значение, указывающее на ошибку
    }

    // Выполняем временный ход на копии доски
    Piece originalPiece = board[toRow][toCol];
    board[toRow][toCol] = board[fromRow][fromCol];
    board[fromRow][fromCol] = Piece(EMPTY, true);

    // Проверяем, оставляет ли ход короля под матом
    if (isCheckMate(board, fromRow, fromCol, isWhite)) {
        if (board[toRow][toCol].isWhite != isWhite) { // Проверка цвета фигуры
            // Отменяем временный ход и возвращаем значение, указывающее на шах
            board[fromRow][fromCol] = board[toRow][toCol];
            board[toRow][toCol] = originalPiece;
            return 5000; // Ход оставляет короля под шахом
        }
    }

    // Проверяем, оставляет ли ход короля под шахом
    if (isCheck(board, fromRow, fromCol, isWhite)) {
        if (board[toRow][toCol].isWhite != isWhite) { // Проверка цвета фигуры
            // Отменяем временный ход и возвращаем значение, указывающее на шах
            board[fromRow][fromCol] = board[toRow][toCol];
            board[toRow][toCol] = originalPiece;
            return 2000; // Ход оставляет короля под шахом
        }
    }

    // Проверяем, съедает ли ход фигуру оппонента и остаемся в безопасности
    if (canCapturePiece(board, toRow, toCol, isWhite) && !isCheck(board, fromRow, fromCol, isWhite)) {
        if (board[toRow][toCol].isWhite != isWhite) { // Проверка цвета фигуры
            // Отменяем временный ход и возвращаем высокий балл за съедание фигуры
            board[fromRow][fromCol] = board[toRow][toCol];
            board[toRow][toCol] = originalPiece;
            return 1000; // Ход съедает фигуру оппонента и остается в безопасности
        }
    }

    // Проверяем, съедает ли ход фигуру оппонента, но нас могут съесть после этого
    if (canCapturePiece(board, toRow, toCol, isWhite) && isCheck(board, fromRow, fromCol, !isWhite)) {
        if (board[toRow][toCol].isWhite != isWhite) { // Проверка цвета фигуры
            if (board[fromRow][fromCol].symbol == KING) return 0;
            // Отменяем временный ход и возвращаем средний балл
            board[fromRow][fromCol] = board[toRow][toCol];
            board[toRow][toCol] = originalPiece;
            return 100; // Ход съедает фигуру оппонента, но нас могут съесть после этого
        }
    }

    // Проверяем, спасает ли ход фигуру
    if (canSavePiece(board, toRow, toCol, isWhite)) {
        if (board[toRow][toCol].isWhite != isWhite) { // Проверка цвета фигуры
            // Отменяем временный ход и возвращаем высокий балл за спасение фигуры
            board[fromRow][fromCol] = board[toRow][toCol];
            board[toRow][toCol] = originalPiece;
            return 500; // Ход спасает фигуру
        }
    }

    if (board[toRow][toCol].isWhite != isWhite) {
        // Если ход не подпадает под вышеуказанные критерии, возвращаем средний балл
        board[fromRow][fromCol] = board[toRow][toCol];
        board[toRow][toCol] = originalPiece;
        return 0; // Обычный ход без особых приоритетов
    }

    // Ужасный ход
    return -100;
}

std::vector<std::pair<int, int>> findBestMoves(const std::vector<std::vector<Piece>>& board, bool isWhite) {
    // Создание вектор для хранения всех возможных ходов
    std::vector<std::pair<int, int>> allMoves;

    // Перебираем все фигуры и их возможные ходы
    for (int fromRow = 0; fromRow < BOARD_SIZE; ++fromRow) {
        for (int fromCol = 0; fromCol < BOARD_SIZE; ++fromCol) {
            if (board[fromRow][fromCol].isWhite == isWhite) {
                for (int toRow = 0; toRow < BOARD_SIZE; ++toRow) {
                    for (int toCol = 0; toCol < BOARD_SIZE; ++toCol) {
                        if (isValidMove(board, fromRow, fromCol, toRow, toCol, isWhite) && board[fromRow][fromCol].symbol != '.') {
                            allMoves.push_back(std::make_pair(fromRow * 10 + fromCol, toRow * 10 + toCol));
                        }
                    }
                }
            }
        }
    }

    // Сортировка ходов по оценке, используя evaluateMove
    std::sort(allMoves.begin(), allMoves.end(), [&](const std::pair<int, int>& move1, const std::pair<int, int>& move2) {
        int eval1 = evaluateMove(board, move1.first / 10, move1.first % 10, move1.second / 10, move1.second % 10, isWhite);
        int eval2 = evaluateMove(board, move2.first / 10, move2.first % 10, move2.second / 10, move2.second % 10, isWhite);
        return eval1 > eval2;
    });

    // Выбор лучших ходов
    std::vector<std::pair<int, int>> bestMoves;
    for (size_t i = 0; i < count_of_moves; ++i) {
        if (i < allMoves.size()) {
            bestMoves.push_back(allMoves[i]);
        }
        else break; // Плохая генерация
    }

    return bestMoves;
}

void printMoves(const std::vector<std::vector<Piece>>& board, const std::vector<std::pair<int, int>>& moves) {
    int move_index = 1;
    for (const auto& move : moves) {
        int fromRow = move.first / 10;
        int fromCol = move.first % 10;
        int toRow = move.second / 10;
        int toCol = move.second % 10;

        const char colLetters[8]{'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H'};

        if (isValidCell(fromRow, fromCol) && isValidCell(toRow, toCol)) {
            std::cout << move_index << ") " << board[fromRow][fromCol].symbol << ": " << colLetters[fromCol] << fromRow + 1 << " - " << colLetters[toCol] << toRow + 1 << std::endl;
            move_index++;
        }
        else {
            // Вывести сообщение об ошибке, если координаты недопустимы
            std::cout << "Ошибка: Недопустимые координаты хода" << std::endl;
        }
    }
}

int main() {
    setlocale(LC_ALL, "Russian");
    
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    std::vector<std::pair<int, int>> bestMoves;
    bool isWhiteToMove, success;
    int fromRow, fromCol, fr, fc, tr, tc, move_i;
    const char colLetters[8]{ 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H' };

    std::vector<std::vector<Piece>> board(BOARD_SIZE, std::vector<Piece>(BOARD_SIZE));

    // Определение цвета игрока
    isWhiteToMove = true; // Играют белые

    fromRow = 0;
    fromCol = 0;

    generateRandomPosition(board);

    // Поиск и вывод девяти лучших ходов

    bestMoves = findBestMoves(board, isWhiteToMove);

    while (fromRow < BOARD_SIZE) {
        success = true;
        while (fromCol < BOARD_SIZE) {
            if (isCheck(board, fromRow, fromCol, !isWhiteToMove) || isCheck(board, fromRow, fromCol, isWhiteToMove) || bestMoves.size() < count_of_moves) {
                generateRandomPosition(board);
                bestMoves = findBestMoves(board, isWhiteToMove);
                fromRow = 0;
                fromCol = 0;
                success = false;
            }
            else fromCol++;
        }
        if (success) {
            fromCol = 0;
            fromRow++;
        }
    }

    std::vector<std::pair<int, int>> origBM(bestMoves);
    std::vector<std::pair<int, int>> madeMoves;
    
    madeMoves.push_back(origBM[0]);
    madeMoves.push_back(origBM[1]);
    madeMoves.push_back(origBM[2]);

    std::cout << "Сгенерированное шахматное поле: " << std::endl;
    printBoard(board);

    std::cout << std::endl;
    std::cout << "Лучшие ходы:" << std::endl;
    printMoves(board, bestMoves);
    std::cout << std::endl << std::endl;

    for (int i = 0; i < 3; ++i) {

        std::vector<std::vector<Piece>> temp_board(board);

        std::cout << "РАЗВИВАЕМ ХОД НОМЕР " << i + 1 << ": " << std::endl << std::endl;

        fr = origBM[i].first / 10;
        fc = origBM[i].first % 10;
        tr = origBM[i].second / 10;
        tc = origBM[i].second % 10;

        std::cout << temp_board[fr][fc].symbol << ": " << colLetters[fc] << fr + 1 << " - " << colLetters[tc] << tr + 1 << std::endl;

        temp_board[tr][tc].symbol = temp_board[fr][fc].symbol;
        temp_board[tr][tc].isWhite = true;
        temp_board[fr][fc].symbol = '.';

        printBoard(temp_board);
        std::cout << std::endl << std::endl;

        for (int j = 0; j < 2; ++j) {

            bestMoves = findBestMoves(temp_board, isWhiteToMove);
            
            move_i = 0;
            
            while (count(madeMoves.begin(), madeMoves.end(), bestMoves[move_i]) > 0) {
            	move_i++;
            }
            
            madeMoves.push_back(bestMoves[move_i]);

            fr = bestMoves[move_i].first / 10;
            fc = bestMoves[move_i].first % 10;
            tr = bestMoves[move_i].second / 10;
            tc = bestMoves[move_i].second % 10;

            std::cout << temp_board[fr][fc].symbol << ": " << colLetters[fc] << fr + 1 << " - " << colLetters[tc] << tr + 1 << std::endl;

            temp_board[tr][tc].symbol = temp_board[fr][fc].symbol;
            temp_board[tr][tc].isWhite = true;
            temp_board[fr][fc].symbol = '.';

            printBoard(temp_board);
            std::cout << std::endl << std::endl;

        }

    }

    return 0;
}
