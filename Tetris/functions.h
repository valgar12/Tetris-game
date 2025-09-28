#pragma once
#include <vector>
#include <array>
#include "imgui/imgui.h"
#include <chrono>
#include <thread>
#include <set>
#include <iostream>
#include <random>
#include <algorithm>
#include <mutex>
#include <string>

struct Vec2 {
    float x{ 0.f }, y{ 0.f };

    constexpr Vec2(float x = 0, float y = 0) : x(x), y(y) {}

    constexpr Vec2 operator+(const Vec2& other) const {
        return Vec2{ x + other.x, y + other.y };
    }

    constexpr Vec2 operator-(const Vec2& other) const {
        return Vec2{ x - other.x, y - other.y };
    }

    constexpr Vec2 operator*(float value) const {
        return Vec2{ x * value, y * value };
    }

    constexpr Vec2 operator/(float value) const {
        return Vec2{ x / value, y / value };
    }

    bool operator<(const Vec2& other) const {
        return (x == other.x) ? (y < other.y) : (x < other.x);
    }

    constexpr bool operator==(const Vec2& other) const {
        return x == other.x && y == other.y;
    }
};

namespace Colors {
    ImColor Red(1.0f, 0.0f, 0.0f);
    ImColor Green(0.0f, 1.0f, 0.0f);
    ImColor Blue(0.0f, 0.0f, 1.0f);
    ImColor Yellow(1.0f, 1.0f, 0.0f);
    ImColor Orange(1.0f, 0.647f, 0.0f);
    ImColor Purple(0.5f, 0.0f, 0.5f);
    ImColor Cyan(0.0f, 1.0f, 1.0f);
    ImColor Magenta(1.0f, 0.0f, 1.0f);
    ImColor LightGrey(0.752f, 0.752f, 0.752f);
    ImColor Grey(0.6f, 0.6f, 0.6f);
    ImColor DarkGrey(0.25f, 0.25f, 0.25f);
    ImColor Black(0.0f, 0.0f, 0.0f);
    ImColor White(1.0f, 1.0f, 1.0f);
    ImColor Brown(0.545f, 0.271f, 0.075f);
    ImColor Indigo(0.294f, 0.0f, 0.509f);
    ImColor Pink(1.0f, 0.078f, 0.576f);
    ImColor Lime(0.196f, 0.803f, 0.196f);
    ImColor Chocolate(0.824f, 0.412f, 0.118f);
    ImColor SteelBlue(0.275f, 0.509f, 0.706f);
    ImColor Orchid(0.855f, 0.439f, 0.839f);
}

struct Square {
    float size = 20.f;
    Vec2 center{ 0.f, 0.f };
    ImColor color = Colors::Red;
};

std::array<std::array<std::array<std::array<int, 3>, 3>, 8>, 4> TetrisPieces{
    std::array<std::array<std::array<int, 3>, 3>, 8>{
        std::array<std::array<int, 3>, 3>{std::array<int, 3>{0, 0, 0}, std::array<int, 3>{1, 1, 1}, std::array<int, 3>{0, 0, 0}},
        std::array<std::array<int, 3>, 3>{std::array<int, 3>{0, 0, 0}, std::array<int, 3>{1, 0, 0}, std::array<int, 3>{1, 1, 1}},
        std::array<std::array<int, 3>, 3>{std::array<int, 3>{0, 0, 0}, std::array<int, 3>{0, 0, 1}, std::array<int, 3>{1, 1, 1}},
        std::array<std::array<int, 3>, 3>{std::array<int, 3>{0, 0, 0}, std::array<int, 3>{0, 1, 1}, std::array<int, 3>{0, 1, 1}},
        std::array<std::array<int, 3>, 3>{std::array<int, 3>{0, 0, 0}, std::array<int, 3>{0, 1, 1}, std::array<int, 3>{1, 1, 0}},
        std::array<std::array<int, 3>, 3>{std::array<int, 3>{0, 0, 0}, std::array<int, 3>{0, 1, 0}, std::array<int, 3>{1, 1, 1}},
        std::array<std::array<int, 3>, 3>{std::array<int, 3>{0, 0, 0}, std::array<int, 3>{1, 1, 0}, std::array<int, 3>{0, 1, 1}},
        std::array<std::array<int, 3>, 3>{std::array<int, 3>{0, 0, 0}, std::array<int, 3>{1, 1, 1}, std::array<int, 3>{0, 0, 0}}
    },
    std::array<std::array<std::array<int, 3>, 3>, 8>{
        std::array<std::array<int, 3>, 3>{std::array<int, 3>{0, 1, 0}, std::array<int, 3>{0, 1, 0}, std::array<int, 3>{0, 1, 0}},
        std::array<std::array<int, 3>, 3>{std::array<int, 3>{1, 1, 0}, std::array<int, 3>{0, 1, 0}, std::array<int, 3>{0, 1, 0}},
        std::array<std::array<int, 3>, 3>{std::array<int, 3>{1, 0, 0}, std::array<int, 3>{1, 0, 0}, std::array<int, 3>{1, 1, 0}},
        std::array<std::array<int, 3>, 3>{std::array<int, 3>{0, 0, 0}, std::array<int, 3>{0, 1, 1}, std::array<int, 3>{0, 1, 1}},
        std::array<std::array<int, 3>, 3>{std::array<int, 3>{0, 1, 0}, std::array<int, 3>{0, 1, 1}, std::array<int, 3>{0, 0, 1}},
        std::array<std::array<int, 3>, 3>{std::array<int, 3>{1, 0, 0}, std::array<int, 3>{1, 1, 0}, std::array<int, 3>{1, 0, 0}},
        std::array<std::array<int, 3>, 3>{std::array<int, 3>{0, 1, 0}, std::array<int, 3>{1, 1, 0}, std::array<int, 3>{1, 0, 0}},
        std::array<std::array<int, 3>, 3>{std::array<int, 3>{0, 1, 0}, std::array<int, 3>{0, 1, 0}, std::array<int, 3>{0, 1, 0}}
    },
    std::array<std::array<std::array<int, 3>, 3>, 8>{
        std::array<std::array<int, 3>, 3>{std::array<int, 3>{0, 0, 0}, std::array<int, 3>{1, 1, 1}, std::array<int, 3>{0, 0, 0}},
        std::array<std::array<int, 3>, 3>{std::array<int, 3>{0, 0, 1}, std::array<int, 3>{1, 1, 1}, std::array<int, 3>{0, 0, 0}},
        std::array<std::array<int, 3>, 3>{std::array<int, 3>{1, 1, 1}, std::array<int, 3>{1, 0, 0}, std::array<int, 3>{0, 0, 0}},
        std::array<std::array<int, 3>, 3>{std::array<int, 3>{0, 0, 0}, std::array<int, 3>{0, 1, 1}, std::array<int, 3>{0, 1, 1}},
        std::array<std::array<int, 3>, 3>{std::array<int, 3>{0, 1, 1}, std::array<int, 3>{1, 1, 0}, std::array<int, 3>{0, 0, 0}},
        std::array<std::array<int, 3>, 3>{std::array<int, 3>{1, 1, 1}, std::array<int, 3>{0, 1, 0}, std::array<int, 3>{0, 0, 0}},
        std::array<std::array<int, 3>, 3>{std::array<int, 3>{0, 0, 0}, std::array<int, 3>{1, 1, 0}, std::array<int, 3>{0, 1, 1}},
        std::array<std::array<int, 3>, 3>{std::array<int, 3>{0, 0, 0}, std::array<int, 3>{1, 1, 1}, std::array<int, 3>{0, 0, 0}}
    },
    std::array<std::array<std::array<int, 3>, 3>, 8>{
        std::array<std::array<int, 3>, 3>{std::array<int, 3>{0, 1, 0}, std::array<int, 3>{0, 1, 0}, std::array<int, 3>{0, 1, 0}},
        std::array<std::array<int, 3>, 3>{std::array<int, 3>{1, 0, 0}, std::array<int, 3>{1, 0, 0}, std::array<int, 3>{1, 1, 0}},
        std::array<std::array<int, 3>, 3>{std::array<int, 3>{0, 1, 1}, std::array<int, 3>{0, 0, 1}, std::array<int, 3>{0, 0, 1}},
        std::array<std::array<int, 3>, 3>{std::array<int, 3>{0, 0, 0}, std::array<int, 3>{0, 1, 1}, std::array<int, 3>{0, 1, 1}},
        std::array<std::array<int, 3>, 3>{std::array<int, 3>{1, 0, 0}, std::array<int, 3>{1, 1, 0}, std::array<int, 3>{0, 1, 0}},
        std::array<std::array<int, 3>, 3>{std::array<int, 3>{0, 0, 1}, std::array<int, 3>{0, 1, 1}, std::array<int, 3>{0, 0, 1}},
        std::array<std::array<int, 3>, 3>{std::array<int, 3>{0, 0, 1}, std::array<int, 3>{0, 1, 1}, std::array<int, 3>{0, 1, 0}},
        std::array<std::array<int, 3>, 3>{std::array<int, 3>{0, 1, 0}, std::array<int, 3>{0, 1, 0}, std::array<int, 3>{0, 1, 0}}
    }
};

namespace gl {
    int SecondsToWaitGravity = 1000;
    Vec2 DisplaySize{};
    ImDrawList* drawList{};
    const int columns = 10;
    const int rows = 20;
    std::array<std::array<int, columns>, rows> board{ 0 };
    std::array<std::array<int, columns>, rows> StaticPieces{ 0 };
    std::array<std::array<int, columns>, rows> ColorBoard{ 0 };
    float PieceSize = 40.f;
    bool inPlay = false;
    Vec2 CurrentPiece{};
    int CurrentRot = 0;
    int NextRot = 0;
    int CurrentPieceType{};
    int NextPieceType = 0;
    bool GameFinished = false;
    std::mt19937 gen{};
    std::mutex mtx;
    int score = 0;
    ImFont* TetrisFont = nullptr;
    bool FirstTime = true;
}

ImColor GetColor(int index) {
    switch (index) {
    case 0: return Colors::Red;
    case 1: return Colors::Yellow;
    case 2: return Colors::Orange;
    case 3: return Colors::Pink;
    case 4: return Colors::Green;
    case 5: return Colors::Purple;
    case 6: return Colors::White;
    case 7: return Colors::Red;
    default: return Colors::White;
    }
}

namespace Tetris {
    void DrawSquare(Vec2 pos, ImColor color) {
        gl::drawList->AddRectFilled(ImVec2(pos.x, pos.y), ImVec2(pos.x + gl::PieceSize, pos.y + gl::PieceSize), color);
        gl::drawList->AddRect(ImVec2(pos.x, pos.y), ImVec2(pos.x + gl::PieceSize, pos.y + gl::PieceSize), Colors::Blue, 0.f, 0, 6.f);
    }

    void Render() {
        for (int i = 0; i < gl::rows; i++) {
            for (int j = 0; j < gl::columns; j++) {
                if (gl::board[i][j] == 1) {
                    DrawSquare(Vec2{ gl::PieceSize * j, gl::PieceSize * i }, GetColor(gl::ColorBoard[i][j]));
                }
            }
        }

        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (TetrisPieces[gl::NextRot][gl::NextPieceType][i][j] != 1) continue;

                float aX = (gl::PieceSize * gl::columns + 10.f + 75.f + 25.f) + (j * (gl::PieceSize));
                float aY = 100.f + (i * (gl::PieceSize));
                float bX = aX + (gl::PieceSize);
                float bY = aY + (gl::PieceSize);

                gl::drawList->AddRectFilled(ImVec2(aX, aY), ImVec2(bX, bY), GetColor(gl::NextPieceType));
                gl::drawList->AddRect(ImVec2(aX, aY), ImVec2(bX, bY), Colors::Blue, 0.f, 0, 5.f);

                aX = (gl::PieceSize * gl::columns + 10.f + 75.f + 10.f);
                aY = 85.f;
                bX = (gl::PieceSize * gl::columns + 10.f + 75.f + 40.f) + (2 * (gl::PieceSize)) + (gl::PieceSize);
                bY = 115.f + (2 * (gl::PieceSize)) + (gl::PieceSize);

                gl::drawList->AddRect(ImVec2(aX, aY), ImVec2(bX, bY), Colors::White, 0.f, 0, 5.f);
            }
        }
    }

    void CheckRowComplete() {
        for (int i = 0; i < gl::rows; i++) {
            if (std::all_of(gl::StaticPieces[i].begin(), gl::StaticPieces[i].end(), [](int cell) { return cell == 1; })) {
                std::lock_guard<std::mutex> lock(gl::mtx);
                std::fill(gl::board[i].begin(), gl::board[i].end(), 0);
                std::fill(gl::StaticPieces[i].begin(), gl::StaticPieces[i].end(), 0);
                for (int k = i; k > 0; k--) {
                    gl::board[k] = gl::board[k - 1];
                    gl::ColorBoard[k] = gl::ColorBoard[k - 1];
                    gl::StaticPieces[k] = gl::StaticPieces[k - 1];
                }
                gl::score += 100;
                gl::SecondsToWaitGravity *= 0.97;
            }
        }

        for (int i = 0; i < gl::rows; i++) {
            for (int j = 0; j < gl::columns; j++) {
                if (gl::StaticPieces[i][j] == 1 && gl::board[i][j] == 0) {
                    gl::StaticPieces[i][j] = 0;
                }
            }
        }
    }

    bool CanMove(Vec2 offset) {
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (TetrisPieces[gl::CurrentRot][gl::CurrentPieceType][i][j] == 1) {
                    int newX = static_cast<int>(gl::CurrentPiece.x + j + offset.x);
                    int newY = static_cast<int>(gl::CurrentPiece.y + i + offset.y);
                    if (newX < 0 || newX >= gl::columns || newY >= gl::rows || (newY >= 0 && gl::StaticPieces[newY][newX] == 1)) {
                        return false;
                    }
                }
            }
        }
        return true;
    }

    void MovePiece(Vec2 offset) {
        if (!CanMove(offset)) return;

        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (TetrisPieces[gl::CurrentRot][gl::CurrentPieceType][i][j] == 1) {
                    std::lock_guard<std::mutex> lock(gl::mtx);
                    gl::board[static_cast<int>(gl::CurrentPiece.y + i)][static_cast<int>(gl::CurrentPiece.x + j)] = 0;
                }
            }
        }

        {
            std::lock_guard<std::mutex> lock(gl::mtx);
            gl::CurrentPiece = gl::CurrentPiece + offset;
        }

        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (TetrisPieces[gl::CurrentRot][gl::CurrentPieceType][i][j] == 1) {
                    std::lock_guard<std::mutex> lock(gl::mtx);
                    gl::board[static_cast<int>(gl::CurrentPiece.y + i)][static_cast<int>(gl::CurrentPiece.x + j)] = 1;
                    gl::ColorBoard[static_cast<int>(gl::CurrentPiece.y + i)][static_cast<int>(gl::CurrentPiece.x + j)] = gl::CurrentPieceType;
                }
            }
        }
    }

    void RenderPredict() {
        int index = 0;
        while (CanMove({ 0, static_cast<float>(index) })) {
            index++;
        }
        index--;

        auto tempArray = gl::StaticPieces;

        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (TetrisPieces[gl::CurrentRot][gl::CurrentPieceType][i][j] == 1) {
                    tempArray[gl::CurrentPiece.y + index + i][gl::CurrentPiece.x + j] = 1;

                    gl::drawList->AddRect(
                        ImVec2(
                            (gl::CurrentPiece.x * gl::PieceSize) + (gl::PieceSize * j),
                            (gl::CurrentPiece.y * gl::PieceSize) + (gl::PieceSize * i) + (index * gl::PieceSize)
                        ),
                        ImVec2(
                            (gl::CurrentPiece.x * gl::PieceSize) + (gl::PieceSize * j) + gl::PieceSize,
                            (gl::CurrentPiece.y * gl::PieceSize) + (gl::PieceSize * i) + gl::PieceSize + (index * gl::PieceSize)
                        ),
                        Colors::Cyan, 0.f, 0, 1.f);
                }
            }
        }

        for (int i = 0; i < gl::rows; i++) {
            if (std::all_of(tempArray[i].begin(), tempArray[i].end(), [](int cell) { return cell == 1; })) {
                for (int j = 0; j < gl::columns; j++) {
                    gl::drawList->AddRect(ImVec2(j * gl::PieceSize, i * gl::PieceSize),
                        ImVec2(j * gl::PieceSize + gl::PieceSize, i * gl::PieceSize + gl::PieceSize),
                        Colors::Cyan, 0.f, 0, 5.f);
                }
            }
        }
    }

    void MovePieceBottom() {
        int index = 0;
        while (CanMove({ 0, static_cast<float>(index) })) {
            index++;
        }
        index--;

        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (TetrisPieces[gl::CurrentRot][gl::CurrentPieceType][i][j] == 1) {
                    gl::StaticPieces[gl::CurrentPiece.y + i][gl::CurrentPiece.x + j] = 0;
                    gl::board[gl::CurrentPiece.y + i][gl::CurrentPiece.x + j] = 0;
                    gl::ColorBoard[gl::CurrentPiece.y + i][gl::CurrentPiece.x + j] = 0;

                    gl::StaticPieces[gl::CurrentPiece.y + i + index][gl::CurrentPiece.x + j] = 1;
                    gl::board[gl::CurrentPiece.y + i + index][gl::CurrentPiece.x + j] = 1;
                    gl::ColorBoard[gl::CurrentPiece.y + i + index][gl::CurrentPiece.x + j] = gl::CurrentPieceType;

                    gl::inPlay = false;
                }
            }
        }
    }

    void ApplyGravity() {
        while (true) {
            if (CanMove({ 0, 1 })) {
                MovePiece({ 0, 1 });
            }
            else {
                for (int i = 0; i < 3; i++) {
                    for (int j = 0; j < 3; j++) {
                        if (TetrisPieces[gl::CurrentRot][gl::CurrentPieceType][i][j] == 1) {
                            std::lock_guard<std::mutex> lock(gl::mtx);
                            gl::StaticPieces[static_cast<int>(gl::CurrentPiece.y + i)][static_cast<int>(gl::CurrentPiece.x + j)] = 1;
                            gl::ColorBoard[static_cast<int>(gl::CurrentPiece.y + i)][static_cast<int>(gl::CurrentPiece.x + j)] = gl::CurrentPieceType;
                        }
                    }
                }
                gl::inPlay = false;
                CheckRowComplete();
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(gl::SecondsToWaitGravity));
        }
    }

    void GeneratePiece() {
        while (true) {
            while (gl::inPlay || gl::GameFinished) {
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }

            std::lock_guard<std::mutex> lock(gl::mtx);

            std::uniform_int_distribution<> dis(0, 7);

            if (gl::FirstTime) {
                gl::CurrentPieceType = dis(gl::gen);
            }
            else {
                gl::CurrentPieceType = gl::NextPieceType;
            }

            gl::NextPieceType = dis(gl::gen);
            gl::CurrentPiece = { 4, 0 };

            std::uniform_int_distribution<> dis2(0, 3);
            gl::CurrentRot = gl::NextRot;
            gl::NextRot = dis2(gl::gen);

            if (!CanMove({ 0, 0 })) {
                gl::GameFinished = true;
                continue;
            }

            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 3; j++) {
                    if (TetrisPieces[gl::CurrentRot][gl::CurrentPieceType][i][j] == 1) {
                        gl::board[static_cast<int>(gl::CurrentPiece.y + i)][static_cast<int>(gl::CurrentPiece.x + j)] = 1;
                        gl::ColorBoard[static_cast<int>(gl::CurrentPiece.y + i)][static_cast<int>(gl::CurrentPiece.x + j)] = gl::CurrentPieceType;
                    }
                }
            }

            if (gl::FirstTime) {
                gl::FirstTime = false;
                if (gl::board[2][0] == 1 && gl::board[2][1] == 1 && gl::board[2][2] == 1) {
                    gl::board[2][0] = 0;
                    gl::board[2][1] = 0;
                    gl::board[2][2] = 0;
                }
            }

            gl::inPlay = true;
        }
    }

    void HandleInput(int keyPressed) {
        if (keyPressed == VK_LEFT) {
            MovePiece({ -1, 0 });
        }
        else if (keyPressed == VK_RIGHT) {
            MovePiece({ 1, 0 });
        }
        else if (keyPressed == VK_UP) {
            int NewRot = (gl::CurrentRot + 1) % 4;

            bool canMove = true;
            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 3; j++) {
                    if (TetrisPieces[NewRot][gl::CurrentPieceType][i][j] == 1) {
                        int newX = static_cast<int>(gl::CurrentPiece.x + j);
                        int newY = static_cast<int>(gl::CurrentPiece.y + i);
                        if (newX < 0 || newX >= gl::columns || newY >= gl::rows || (newY >= 0 && gl::StaticPieces[newY][newX] == 1)) {
                            canMove = false;
                            break;
                        }
                    }
                }
                if (!canMove) break;
            }

            if (!canMove) return;

            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 3; j++) {
                    if (TetrisPieces[gl::CurrentRot][gl::CurrentPieceType][i][j] == 1) {
                        gl::ColorBoard[static_cast<int>(gl::CurrentPiece.y + i)][static_cast<int>(gl::CurrentPiece.x + j)] = 0;
                        gl::board[static_cast<int>(gl::CurrentPiece.y + i)][static_cast<int>(gl::CurrentPiece.x + j)] = 0;
                    }
                }
            }

            gl::CurrentRot = NewRot;

            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 3; j++) {
                    if (TetrisPieces[gl::CurrentRot][gl::CurrentPieceType][i][j] == 1) {
                        gl::ColorBoard[static_cast<int>(gl::CurrentPiece.y + i)][static_cast<int>(gl::CurrentPiece.x + j)] = gl::CurrentPieceType;
                        gl::board[static_cast<int>(gl::CurrentPiece.y + i)][static_cast<int>(gl::CurrentPiece.x + j)] = 1;
                    }
                }
            }
        }
        else if (keyPressed == VK_DOWN) {
            if (!CanMove({ 0, 1 })) {
                for (int i = 0; i < 3; i++) {
                    for (int j = 0; j < 3; j++) {
                        if (TetrisPieces[gl::CurrentRot][gl::CurrentPieceType][i][j] == 1) {
                            std::lock_guard<std::mutex> lock(gl::mtx);
                            gl::StaticPieces[static_cast<int>(gl::CurrentPiece.y + i)][static_cast<int>(gl::CurrentPiece.x + j)] = 1;
                            gl::ColorBoard[static_cast<int>(gl::CurrentPiece.y + i)][static_cast<int>(gl::CurrentPiece.x + j)] = gl::CurrentPieceType;
                        }
                    }
                }
                gl::inPlay = false;
                CheckRowComplete();
            }
            else {
                MovePiece({ 0, 1 });
            }
        }
        else if (keyPressed == VK_SPACE) {
            MovePieceBottom();
        }
    }
}